#pragma once
#ifndef SCL_INSTANCE_RESOLVER_H
#define SCL_INSTANCE_RESOLVER_H

#include <ModelKit/Featuring/Access/InstanceGuard.h>
#include <ModelKit/Featuring/Access/ValueGuard.h>
#include "Traits.h"

namespace SclPrivate
{
    template < typename _Instance, typename _OtherRefer >
    struct InstanceResolverHelper;
}

/*!
 * Вычислитель необходимого способа предоставления доступа к экземпляру значения
 * посредством метода resolve().
 *
 * Если типы _Instance и _Other между собой являются совместимыми,
 * или _Other совместим с любой внутренней частью _Instance,
 *
 * то операция выполняется между внутренними представлениями Instance.
 * Если данный тип совместим с внутренней частью другого, то другой
 * "разрешается" до типа совместимого с этим. Во всех других случаях
 * другой "разрешается" до базового внутреннего значения.
 */
namespace SclPrivate
{
    template < typename _Instance, typename _OtherRefer >
    using InstanceResolver = typename ::SclPrivate::InstanceResolverHelper< _Instance, _OtherRefer >::Type;
}

namespace SclPrivate
{
    template < typename _Instance, typename _OtherRefer >
    class InstanceCompatibleResolver;

    template < typename _Instance, typename _OtherRefer >
    class InstanceOtherPathOfThisResolver;

    template < typename _Instance, typename _OtherRefer >
    class InstanceThisPathOfOtherResolver;

    template < typename _Instance, typename _OtherRefer >
    class InstanceValueResolver;
}

/*
 * NOTE: Можно было бы реализовать случай преобразования одного вида Holder
 * в другой с помощью InstanceConvertResolver. Однако это нарушит инкапсуляцию
 * Holder и позволит получить доступ к его внутреннему содержимому,
 * что в свою очередь может изменить ожидаемое поведение.
 */
namespace SclPrivate
{
    template < typename _Instance, typename _OtherRefer >
    struct InstanceResolverHelper
    {
        using Instance = _Instance;
        using OtherRefer = _OtherRefer;
        using OtherInstance = ::std::decay_t< _OtherRefer >;

        using Type = ::std::conditional_t<
            ::Scl::is_compatible< Instance, OtherInstance >,
            ::SclPrivate::InstanceCompatibleResolver< Instance, OtherRefer >,
            ::std::conditional_t<
                ::Scl::is_this_part_of_other< OtherInstance, Instance >,
                ::SclPrivate::InstanceOtherPathOfThisResolver< Instance, OtherRefer >,
                ::std::conditional_t<
                    ::Scl::is_this_part_of_other< Instance, OtherInstance >,
                    ::SclPrivate::InstanceThisPathOfOtherResolver< Instance, OtherRefer >,
                    ::SclPrivate::InstanceValueResolver< Instance, OtherRefer > > > >;
    };
}

namespace SclPrivate
{
    /*!
     * Используется, если _Other совместим с _Instance.
     */
    template < typename _Instance, typename _OtherRefer >
    class InstanceCompatibleResolver
    {
    public:
        using Instance = _Instance;
        using OtherRefer = _OtherRefer;
        using OtherInstance = ::std::decay_t< OtherRefer >;
        using OtherHolder = typename OtherInstance::Holder;
        using OtherHolderRefer = ::Scl::SimilarRefer< OtherHolder, OtherRefer >;
        using AccessRefer = OtherHolderRefer;

    private:
        OtherRefer m_other_refer;

    public:
        InstanceCompatibleResolver ( OtherRefer other )
            : m_other_refer( ::std::forward< OtherRefer >( other ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::SclPrivate::instanceHolder< OtherRefer >( ::std::forward< OtherRefer >( m_other_refer ) );
        }
    };
}

namespace SclPrivate
{
    /*!
     * Используется, если _Other совместим c вложенной частью _Instance.
     */
    template < typename _Instance, typename _OtherRefer >
    class InstanceOtherPathOfThisResolver
    {
    public:
        using Instance = _Instance;
        using OtherRefer = _OtherRefer;
        using AccessRefer = OtherRefer;

    private:
        OtherRefer m_other_refer;

    public:
        InstanceOtherPathOfThisResolver ( OtherRefer other )
            : m_other_refer( ::std::forward< OtherRefer >( other ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( m_other_refer );
        }
    };
}

namespace SclPrivate
{
    /*!
     * Используется, если _Instance совместим с вложенной частью _Other.
     */
    template < typename _Instance, typename _OtherRefer >
    class InstanceThisPathOfOtherResolver
    {
    public:
        using Instance = _Instance;
        using OtherRefer = _OtherRefer;
        using OtherInstanceGuard = ::SclPrivate::InstanceGuard< OtherRefer >;
        using OtherInstance = ::std::decay_t< OtherRefer >;
        using OtherValue = typename OtherInstance::Value;
        using OtherValueRefer = ::Scl::SimilarRefer< OtherValue, OtherRefer >;
        using NextResolver = ::SclPrivate::InstanceResolver< Instance, OtherValueRefer >;
        using AccessRefer = typename NextResolver::AccessRefer;

        static_assert( ::Scl::is_instance< Instance >, "The template parameter _Instance must to be a Instance type!" );
        static_assert( ::std::is_reference< OtherRefer >::value, "The template parameter _OtherRefer must to be a reference type." );
        static_assert( ::Scl::is_instance< OtherInstance >, "The template parameter _OtherRefer must to be a Instance type reference!" );
        static_assert( ::Scl::is_similar< OtherRefer, OtherValueRefer >, "The OtherRefer and OtherValueRefer must to be similar types!" );

    private:
        OtherInstanceGuard m_instance_guard;
        NextResolver m_next_resolver;

    public:
        InstanceThisPathOfOtherResolver ( OtherRefer other )
            : m_instance_guard( ::std::forward< OtherRefer >( other ) )
            , m_next_resolver( ::std::forward< OtherValueRefer >( m_instance_guard.instanceAccess() ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( m_next_resolver.resolve() );
        }
    };
}

namespace SclPrivate
{
    /*!
     * Используется, если _Instance не совместим с _Other.
     */
    template < typename _Instance, typename _OtherRefer >
    class InstanceValueResolver
    {
    public:
        using Instance = _Instance;
        using OtherRefer = _OtherRefer;
        using OtherValueGuard = ::Scl::ValueGuard< OtherRefer >;
        using AccessRefer = typename OtherValueGuard::ValueRefer;

        static_assert( ::Scl::is_instance< Instance >, "The template parameter _Instance must to be a instance!" );
        static_assert( ::std::is_reference< OtherRefer >::value, "The template parameter _OtherRefer must to be a reference type." );

    private:
        OtherValueGuard m_value_guard;

    public:
        InstanceValueResolver ( OtherRefer other )
            : m_value_guard( ::std::forward< OtherRefer >( other ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( m_value_guard.valueAccess() );
        }
    };
}

#endif

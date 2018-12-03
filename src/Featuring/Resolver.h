#pragma once
#ifndef INSTANCE_RESOLVER_H
#define INSTANCE_RESOLVER_H

#include "Access/InstanceGuard.h"
#include "Access/ValueGuard.h"
#include "Traits.h"

namespace Private
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
template < typename _Instance, typename _OtherRefer >
using InstanceResolver = typename Private::InstanceResolverHelper< _Instance, _OtherRefer >::Type;

namespace Private
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
namespace Private
{
    template < typename _Instance, typename _OtherRefer >
    struct InstanceResolverHelper
    {
        using Instance = _Instance;
        using OtherRefer = _OtherRefer;
        using OtherInstance = ::std::decay_t< _OtherRefer >;

        using Type = ::std::conditional_t<
            is_compatible< Instance, OtherInstance >,
            InstanceCompatibleResolver< Instance, OtherRefer >,
            ::std::conditional_t<
                is_this_part_of_other< OtherInstance, Instance >,
                InstanceOtherPathOfThisResolver< Instance, OtherRefer >,
                ::std::conditional_t<
                    is_this_part_of_other< Instance, OtherInstance >,
                    InstanceThisPathOfOtherResolver< Instance, OtherRefer >,
                    InstanceValueResolver< Instance, OtherRefer > > > >;
    };
}

namespace Private
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
        using OtherHolderRefer = ::SimilarRefer< OtherHolder, OtherRefer >;
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
            return ::std::forward< AccessRefer >( m_other_refer.m_holder );
        }
    };
}

namespace Private
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

namespace Private
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
        using OtherInstanceGuard = InstanceGuard< OtherRefer >;
        using OtherInstance = ::std::decay_t< OtherRefer >;
        using OtherValue = typename OtherInstance::Value;
        using OtherValueRefer = ::SimilarRefer< OtherValue, OtherRefer >;
        using NextResolver = InstanceResolver< Instance, OtherValueRefer >;
        using AccessRefer = typename NextResolver::AccessRefer;

        static_assert( ::is_featured< Instance >, "The template parameter _Instance must be a featured!" );
        static_assert( ::std::is_reference< OtherRefer >::value, "The template parameter _OtherRefer must be a reference!" );
        static_assert( ::is_featured< OtherInstance >, "The template parameter _OtherRefer must be a featured type reference!" );
        static_assert( ::is_similar< OtherRefer, OtherValueRefer >, "The OtherRefer and OtherValueRefer must be similar types!" );

    private:
        OtherInstanceGuard m_featured_pointer;
        NextResolver m_next_resolver;

    public:
        InstanceThisPathOfOtherResolver ( OtherRefer other )
            : m_featured_pointer( ::std::forward< OtherRefer >( other ) )
            , m_next_resolver( ::std::forward< OtherValueRefer >( m_featured_pointer.value() ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( m_next_resolver.resolve() );
        }
    };
}

namespace Private
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
        using OtherValueGuard = ValueGuard< OtherRefer >;
        using AccessRefer = typename OtherValueGuard::AccessRefer;

        static_assert( ::is_featured< Instance >, "The template parameter _Instance must be a featured!" );
        static_assert( ::std::is_reference< OtherRefer >::value, "The template parameter _OtherRefer must be a reference!" );

    private:
        OtherValueGuard m_value_guard;

    public:
        InstanceValueResolver ( OtherRefer other )
            : m_value_guard( ::std::forward< OtherRefer >( other ) )
        {
        }

        AccessRefer resolve () const
        {
            return ::std::forward< AccessRefer >( *m_value_guard );
        }
    };
}

#endif

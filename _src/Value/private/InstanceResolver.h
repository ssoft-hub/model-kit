#pragma once

#include <ModelKit/Common/FeaturePointer.h>
#include <ModelKit/Common/ValuePointer.h>
#include <ModelKit/Common/ValueTrait.h>

template < typename _Instance, typename _OtherRefer >
class InstanceCompatibleResolver;

template < typename _Instance, typename _OtherRefer >
class InstanceFeatureResolver;

template < typename _Instance, typename _OtherRefer >
class InstanceValueResolver;

/*!
 * Помошник вычисляет необходимый способ предоставления доступа к значению.
 * Если типы Instance совместимы или другой совместим с внутренней частью этого,
 * то операция выполняется между внутренними представлениями Instance.
 * Если данный тип совместим с внутренней частью другого, то другой
 * "разрешается" до типа совместимого с этим. Во всех других случаях
 * другой "разрешается" до базового внутреннего значения.
 */
template < typename _Instance, typename _OtherRefer >
struct InstanceResolverHelper
{
    using Instance = _Instance;
    using OtherRefer = _OtherRefer;
    using OtherInstance = ::std::decay_t< _OtherRefer >;

    using type = ::std::conditional_t<
        is_compatible< Instance, OtherInstance >
            || is_this_part_of_other< OtherInstance, Instance >,
        InstanceCompatibleResolver< Instance, OtherRefer >,
        ::std::conditional_t<
            is_this_part_of_other< Instance, OtherInstance >,
            InstanceFeatureResolver< Instance, OtherRefer >,
            InstanceValueResolver< Instance, OtherRefer > > >;
};

template < typename _Instance, typename _OtherRefer >
using InstanceResolver = typename InstanceResolverHelper< _Instance, _OtherRefer >::type;

/*!
 * Используется для совместимых типов.
 * Предоставляет метод resolve(), возвращающий ссылку на holder другого типа.
 */
template < typename _Instance, typename _OtherRefer >
class InstanceCompatibleResolver
{
public:
    using Instance = _Instance;
    using OtherRefer = _OtherRefer;
    using OtherInstance = ::std::remove_reference_t< OtherRefer >;
    using OtherHolderRefer = ::std::conditional_t<
        ::std::is_const< OtherInstance >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherRefer >::value,
            const typename OtherInstance::Holder &&,
            const typename OtherInstance::Holder & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherRefer >::value,
            typename OtherInstance::Holder &&,
            typename OtherInstance::Holder & > >;

private:
    OtherRefer m_other_refer;

public:
    InstanceCompatibleResolver ( OtherRefer other )
        : m_other_refer( ::std::forward< OtherRefer >( other ) )
    {
    }

    OtherHolderRefer resolve () const
    {
        return ::std::forward< OtherHolderRefer >( m_other_refer.m_holder );
    }
};

/*!
 * Используется, если имеется совместимость типа с любой вложенной частью в другой.
 */
template < typename _Instance, typename _OtherRefer >
class InstanceFeatureResolver
{
public:
    using Instance = _Instance;
    using OtherRefer = _OtherRefer;
    using OtherFeaturePointer = FeaturePointer< OtherRefer >;
    using OtherInstance = ::std::remove_reference_t< OtherRefer >;
    using OtherValueRefer = ::std::conditional_t<
        ::std::is_const< OtherInstance >::value,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherRefer >::value,
            const typename OtherInstance::Value &&,
            const typename OtherInstance::Value & >,
        ::std::conditional_t<
            ::std::is_rvalue_reference< OtherRefer >::value,
            typename OtherInstance::Value &&,
            typename OtherInstance::Value & > >;

    using NextResolver = InstanceResolver< _Instance, OtherValueRefer >;

private:
    OtherFeaturePointer m_feature_guard;
    NextResolver m_next_resolver;

public:
    InstanceFeatureResolver ( OtherRefer other )
        : m_feature_guard( ::std::forward< OtherRefer >( other ) )
        , m_next_resolver( ::std::forward< OtherValueRefer >( m_feature_guard.value() ) )
    {
    }

    decltype(auto) resolve () const
    {
        return m_next_resolver.resolve();
    }
};

/*!
 *
 */
template < typename _Instance, typename _OtherRefer >
class InstanceValueResolver
{
public:
    using Instance = _Instance;
    using OtherRefer = _OtherRefer;
    using OtherValuePointer = ValuePointer< OtherRefer >;

private:
    OtherValuePointer m_value_guard;

public:
    InstanceValueResolver ( OtherRefer other )
        : m_value_guard( ::std::forward< OtherRefer >( other ) )
    {
    }

    decltype(auto) resolve () const
    {
        return *m_value_guard;
    }
};

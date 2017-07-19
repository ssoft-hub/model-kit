#pragma once

#include <Helper/ValueGuard.h>
#include <Helper/InstanceTrait.h>

enum InstanceBuildSwitchType
{
    DefaultInstanceBuild,
    CompatibleInstanceBuild,
    OtherPartOfThisInstanceBuild
};

template < bool _compatible, bool _other_part_of_this >
struct InstanceBuildTypeValue {};

template <>
struct InstanceBuildTypeValue< false, false >
: public ::std::integral_constant< InstanceBuildSwitchType, DefaultInstanceBuild >
{
};

template <>
struct InstanceBuildTypeValue< true, false >
    : public ::std::integral_constant< InstanceBuildSwitchType, CompatibleInstanceBuild >
{
};

template <>
struct InstanceBuildTypeValue< false, true >
    : public ::std::integral_constant< InstanceBuildSwitchType, OtherPartOfThisInstanceBuild >
{
};

template < typename _ThisType, typename _OtherType >
struct InstanceBuildTypeDefiner {};

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuildTypeDefiner< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >
    : public ::std::integral_constant< InstanceBuildSwitchType, InstanceBuildTypeValue<
        IsCompatible< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value,
        IsPartOf< Instance< _OtherType, _OtherTool >, Instance< _ThisType, _ThisTool > >::value >::value >
{
};

/*!
 * Класс формирования экземпляра значения Instance с помощью конструктора
 * с переменным количеством аргументов.
 */
template < typename _ThisType, typename _ThisTool, typename ... _Arguments >
struct InstanceBuilder
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( _Arguments && ... arguments )
    {
        return _ThisTool:: template makeHolder< _ThisType >( ::std::forward< _Arguments >( arguments ) ... );
    }
};

/*!
 * Eсли необходимый Instance совместим с внутренней частью исходного,
 * то необходимо сформировать необходимый Instance на основе внутреннего
 * значения other.
 */
template < InstanceBuildSwitchType >
struct InstanceBuildSwither
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    // DefaultInstanceBuild
    // ThisPartOfOtherInstanceBuild

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return  InstanceBuilder< _ThisType, _ThisTool, _OtherType >::construct(
                _OtherTool:: template value< _OtherType >( featureGuard( other ).access().m_holder ) );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        return  InstanceBuilder< _ThisType, _ThisTool, _OtherType >::construct(
            _OtherTool:: template value< _OtherType >( featureGuard(
                    ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ).access().m_holder ) );
    }
};

/*!
 * Если экземпляры Instance< _ThisType, _ThisTool > и Instance< _OtherType, _OtherTool >
 * совместимы или одинаковы, то значение формируется путем копирования или переноса
 * holder с помощью методов, предоставляемых инструментарием _ThisTool.
 */
template <>
struct InstanceBuildSwither< CompatibleInstanceBuild >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return _ThisTool:: template copyHolder< _ThisType >( cFGet( other ).m_holder );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        using OtherInstanceType = Instance< _OtherType, _OtherTool >;
        return _ThisTool:: template moveHolder< _ThisType >( featureGuard(
                ::std::forward< OtherInstanceType >( other ) ).access().m_holder );
    }
};

/*!
 * Если внутренняя часть необходимого экземпляра Instance< _ThisType, _ThisTool > совметима
 * с исходным экземпляром Instance< _OtherType, _OtherTool >, то необходимо выполнить
 * формирование holder на осове значения other с помощью методов, предоставляемых
 * инструментарием _ThisTool.
 */
template <>
struct InstanceBuildSwither< OtherPartOfThisInstanceBuild >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return _ThisTool:: template makeHolder< _ThisType >( other );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        return _ThisTool:: template makeHolder< _ThisType >( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) );
    }
};

/*!
 * Специализация класса формирования экземпляра значения Instance< _ThisType, _ThisTool >
 * на основе другого экземпляра Instance< _OtherType, _OtherTool >.
 * В зависимости от совместимости или вложенности типов Instance< _ThisType, _ThisTool >
 * и Instance< _OtherType, _OtherTool > вычисляется оптимальный способ формирования
 * экземпляра значения Instance< _ThisType, _ThisTool >.
 */
template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return InstanceBuildSwither< InstanceBuildTypeDefiner< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value >
            :: template construct< _ThisType, _ThisTool, _OtherType, _OtherTool >( other );
    }

    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        return InstanceBuildSwither< InstanceBuildTypeDefiner< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value >
            :: template construct< _ThisType, _ThisTool, _OtherType, _OtherTool >( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) );
    }
};

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > & >
    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
{
};

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > && >
    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
{
};

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuilder< _ThisType, _ThisTool, const Instance< _OtherType, _OtherTool > & >
    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
{
};

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuilder< _ThisType, _ThisTool, const Instance< _OtherType, _OtherTool > && >
    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
{
};

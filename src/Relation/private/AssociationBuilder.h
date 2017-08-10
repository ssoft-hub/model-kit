#pragma once

#include <ModelKit/Common/ValueGuard.h>
#include <ModelKit/Common/InstanceTrait.h>

enum class InstanceBuildSwitchType
{
    Default,
    Compatible,
    OtherPartOfThis
};

template < bool _compatible, bool _other_part_of_this >
struct InstanceBuildTypeValue
    : public ::std::integral_constant< InstanceBuildSwitchType, InstanceBuildSwitchType::Default >
{
};

template <>
struct InstanceBuildTypeValue< true, false >
    : public ::std::integral_constant< InstanceBuildSwitchType, InstanceBuildSwitchType::Compatible >
{
};

template <>
struct InstanceBuildTypeValue< false, true >
    : public ::std::integral_constant< InstanceBuildSwitchType, InstanceBuildSwitchType::OtherPartOfThis >
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
//    static constexpr decltype(auto) resolve (...)
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType resolve ( _Arguments && ... arguments )
    {
        return typename Instance< _ThisType, _ThisTool >::HolderType( ::std::forward< _Arguments >( arguments ) ... );
    }
};

template < typename _ThisType, typename _ThisTool, typename _Type >
struct InstanceBuilder< _ThisType, _ThisTool, _Type >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) resolve (...)
    static constexpr _Type && resolve ( _Type && other )
    {
        return ::std::forward< _Type >( other );
    }

    static constexpr const _Type & resolve ( const _Type & other )
    {
        return other;
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
//    static constexpr decltype(auto) resolve (...)

    // DefaultInstanceBuild
    // ThisPartOfOtherInstanceBuild

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType resolve ( const Instance< _OtherType, _OtherTool > & other )
    {
        using OtherInstanceReferType = const Instance< _OtherType, _OtherTool > &;
        return  InstanceBuilder< _ThisType, _ThisTool, _OtherType >::resolve(
            _OtherTool:: template value< _OtherType >( (*FeatureGuard< OtherInstanceReferType >( other )).m_holder ) );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType resolve ( Instance< _OtherType, _OtherTool > && other )
    {
        using OtherInstanceReferType = Instance< _OtherType, _OtherTool > &&;
        using OtherInstanceType = Instance< _OtherType, _OtherTool >;
        return  InstanceBuilder< _ThisType, _ThisTool, _OtherType >::resolve(
            _OtherTool:: template value< _OtherType >( (*FeatureGuard< OtherInstanceReferType >(
                    ::std::forward< OtherInstanceType >( other ) )).m_holder ) );
    }
};

/*!
 * Если экземпляры Instance< _ThisType, _ThisTool > и Instance< _OtherType, _OtherTool >
 * совместимы или одинаковы, то значение формируется путем копирования или переноса
 * holder с помощью методов, предоставляемых инструментарием _ThisTool.
 */
template <>
struct InstanceBuildSwither< InstanceBuildSwitchType::Compatible >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) resolve (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr const typename Instance< _OtherType, _OtherTool >::HolderType & resolve ( const Instance< _OtherType, _OtherTool > & other )
    {
        using OtherInstanceReferType = const Instance< _OtherType, _OtherTool > &;
        return (*FeatureGuard< OtherInstanceReferType >( other )).m_holder;
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _OtherType, _OtherTool >::HolderType && resolve ( Instance< _OtherType, _OtherTool > && other )
    {
        using OtherInstanceReferType = Instance< _OtherType, _OtherTool > &&;
        using OtherInstanceType = Instance< _OtherType, _OtherTool >;
        return (*FeatureGuard< OtherInstanceReferType >(
            ::std::forward< OtherInstanceType >( other ) )).m_holder;
    }
};

/*!
 * Если внутренняя часть необходимого экземпляра Instance< _ThisType, _ThisTool > совместима
 * с исходным экземпляром Instance< _OtherType, _OtherTool >, то необходимо выполнить
 * формирование holder на осове значения other с помощью методов, предоставляемых
 * инструментарием _ThisTool.
 */
template <>
struct InstanceBuildSwither< InstanceBuildSwitchType::OtherPartOfThis >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) resolve (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr const Instance< _OtherType, _OtherTool > & resolve ( const Instance< _OtherType, _OtherTool > & other )
    {
        return other;
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr Instance< _OtherType, _OtherTool > && resolve ( Instance< _OtherType, _OtherTool > && other )
    {
        return ::std::forward< Instance< _OtherType, _OtherTool > >( other );
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
    //! Тип возвращаемого значения зависит от вычисленного варианта операции,
    /// поэтому в определении используется decltype(auto).

    static constexpr decltype(auto) resolve ( const Instance< _OtherType, _OtherTool > & other )
    {
        return InstanceBuildSwither< InstanceBuildTypeDefiner< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value >
            :: template resolve< _ThisType, _ThisTool, _OtherType, _OtherTool >( other );
    }

    static constexpr decltype(auto) resolve ( Instance< _OtherType, _OtherTool > && other )
    {
        return InstanceBuildSwither< InstanceBuildTypeDefiner< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value >
            :: template resolve< _ThisType, _ThisTool, _OtherType, _OtherTool >( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) );
    }
};

//template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
//struct InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > & >
//    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
//{
//};

//template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
//struct InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > && >
//    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
//{
//};

//template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
//struct InstanceBuilder< _ThisType, _ThisTool, const Instance< _OtherType, _OtherTool > & >
//    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
//{
//};

//template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
//struct InstanceBuilder< _ThisType, _ThisTool, const Instance< _OtherType, _OtherTool > && >
//    : public InstanceBuilder< _ThisType, _ThisTool, Instance< _OtherType, _OtherTool > >
//{
//};

#pragma once

#include <Helper/ValueGuard.h>
#include <Helper/InstanceTrait.h>

enum InstanceBuildSwitchType
{
    DefaultInstanceBuild,
    CompatibleInstanceBuild,
    ThisPartOfOtherInstanceBuild,
    OtherPartOfThisInstanceBuild
};

template < bool _compatible, bool _this_part_of_other, bool _other_part_of_this >
struct InstanceBuildTypeValue
    : public ::std::integral_constant< InstanceBuildSwitchType, DefaultInstanceBuild >
{
};

template <>
struct InstanceBuildTypeValue< true, false, false >
    : public ::std::integral_constant< InstanceBuildSwitchType, CompatibleInstanceBuild >
{
};

template <>
struct InstanceBuildTypeValue< false, true, false >
    : public ::std::integral_constant< InstanceBuildSwitchType, ThisPartOfOtherInstanceBuild >
{
};

template <>
struct InstanceBuildTypeValue< false, false, true >
    : public ::std::integral_constant< InstanceBuildSwitchType, OtherPartOfThisInstanceBuild >
{
};

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuildTypeDefiner
    : public ::std::integral_constant< InstanceBuildSwitchType, InstanceBuildTypeValue<
        IsCompatible< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value,
        IsPartOf< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value,
        IsPartOf< Instance< _OtherType, _OtherTool >, Instance< _ThisType, _ThisTool > >::value >::value >
{
};

template < InstanceBuildSwitchType >
struct InstanceBuildSwither
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > & other )
    {
        return _ThisTool:: template makeHolder< _ThisType >( cGet( other ) );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return _ThisTool:: template makeHolder< _ThisType >( cGet( other ) );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        return _ThisTool:: template makeHolder< _ThisType >( mGet( other ) );
    }
};


/*!
 * Если Instance совместимы или одинаковы, то значение формируется путем
 * копирования или переноса holder с помощью методов, предоставляемых
 * инструментарием _ThisTool.
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
        return _ThisTool:: template moveHolder< _ThisType >(
            ::std::forward< typename Instance< _OtherType, _OtherTool >::HolderType >( mFGet( other ).m_holder ) );
    }
};

/*!
 * Eсли необходимый Instance совместим с внутренней частью исходного,
 * то необходимо сформировать необходимый Instance на основе внутреннего
 * значения other.
 */
template <>
struct InstanceBuildSwither< ThisPartOfOtherInstanceBuild >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        using Subtype = _OtherType;
        using _SubValueType = typename Subtype::ValueType;
        using _SubValueTool = typename Subtype::ValueTool;
        return  InstanceBuildSwither< InstanceBuildTypeDefiner< _ThisType, _ThisTool, _SubValueType, _SubValueTool >::value >
            :: template construct< _ThisType, _ThisTool, _SubValueType, _SubValueTool >(
                _OtherTool:: template featureGuard< _OtherType >( cFGet( other ).m_holder ).access() );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        using Subtype = _OtherType;
        using SubValueType = typename Subtype::ValueType;
        using SubValueTool = typename Subtype::ValueTool;
        using OtherHolderType = typename Instance< _OtherType, _OtherTool >::HolderType;
        return  InstanceBuildSwither< InstanceBuildTypeDefiner< _ThisType, _ThisTool, SubValueType, SubValueTool >::value >
            :: template construct< _ThisType, _ThisTool, SubValueType, SubValueTool >( ::std::forward< Subtype >(
                _OtherTool:: template featureGuard< _OtherType >( ::std::forward< OtherHolderType >( mFGet( other ).m_holder ) ).access() ) );
    }
};

/*!
 * Если внутренняя часть необходимого Instance совметима с исходным,
 * то необходимо выполнить формирование holder на осове значения other с помощью
 * методов, предоставляемых инструментарием _ThisTool.
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

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct InstanceBuilder
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return InstanceBuildSwither< InstanceBuildTypeDefiner< _ThisType, _ThisTool, _OtherType, _OtherTool >::value >
            :: template construct< _ThisType, _ThisTool, _OtherType, _OtherTool >( other );
    }

    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        return InstanceBuildSwither< InstanceBuildTypeDefiner< _ThisType, _ThisTool, _OtherType, _OtherTool >::value >
            :: template construct< _ThisType, _ThisTool, _OtherType, _OtherTool >( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) );
    }
};

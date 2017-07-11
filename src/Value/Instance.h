#pragma once
#include <Helper/AccessHelper.h>
#include <Helper/InstanceHelper.h>
#include <memory>



enum AssignmentVariant
{
    DefaultVariant,
    CompatibleVariant,
    ThisPartOfOtherVariant,
    OtherPartOfThisVariant
};

template < bool _compatible, bool _this_part_of_other, bool _other_part_of_this >
struct VariantDefiner
    : public ::std::integral_constant< AssignmentVariant, DefaultVariant >
{
};

template <>
struct VariantDefiner< true, false, false >
    : public ::std::integral_constant< AssignmentVariant, CompatibleVariant >
{
};

template <>
struct VariantDefiner< false, true, false >
    : public ::std::integral_constant< AssignmentVariant, ThisPartOfOtherVariant >
{
};

template <>
struct VariantDefiner< false, false, true >
    : public ::std::integral_constant< AssignmentVariant, OtherPartOfThisVariant >
{
};

template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
struct VariantDefiner2
    : public ::std::integral_constant< AssignmentVariant, VariantDefiner<
        IsCompatible< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value,
        IsPartOf< Instance< _ThisType, _ThisTool >, Instance< _OtherType, _OtherTool > >::value,
        IsPartOf< Instance< _OtherType, _OtherTool >, Instance< _ThisType, _ThisTool > >::value >::value >
{
};

template < AssignmentVariant >
struct AssignmentHelper
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
struct AssignmentHelper< CompatibleVariant >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > & other )
    {
        return _ThisTool:: template copyHolder< _ThisType >( cGuard( other ).value().m_holder );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return _ThisTool:: template copyHolder< _ThisType >( cGuard( other ).value().m_holder );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > && other )
    {
        return _ThisTool:: template moveHolder< _ThisType >(
            ::std::forward< typename Instance< _OtherType, _OtherTool >::HolderType >(
                mGuard( other ).value().m_holder ) );
    }
};

/*!
 * Eсли необходимый Instance совместим с внутренней частью исходного,
 * то необходимо сформировать необходимый Instance на основе внутреннего
 * значения other.
 */
template <>
struct AssignmentHelper< ThisPartOfOtherVariant >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr Instance< _ThisType, _ThisTool > construct ( Instance< _OtherType, _OtherTool > & other )
    {
        return Instance< _ThisType, _ThisTool >( _OtherTool:: template readableValueGuard< _OtherType >( cGuard( other ).value().m_holder ).value() );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr Instance< _ThisType, _ThisTool > construct ( const Instance< _OtherType, _OtherTool > & other )
    {
        return Instance< _ThisType, _ThisTool >( _OtherTool:: template readableValueGuard< _OtherType >( cGuard( other ).value().m_holder ).value() );
    }

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr Instance< _ThisType, _ThisTool > construct ( Instance< _OtherType, _OtherTool > && other )
    {
        return Instance< _ThisType, _ThisTool >( _OtherTool:: template movableValueGuard< _OtherType >(
            ::std::forward< _OtherType >( ::std::forward< typename _OtherTool::HolderType >( mGuard( other ).value().m_holder ) ).value() ) );
    }
};

/*!
 * Если внутренняя часть необходимого Instance совметима с исходным,
 * то необходимо выполнить формирование holder на осове значения other с помощью
 * методов, предоставляемых инструментарием _ThisTool.
 */
template <>
struct AssignmentHelper< OtherPartOfThisVariant >
{
//    можно использовать в определениях методов
//    static constexpr decltype(auto) construct (...)

    template < typename _ThisType, typename _ThisTool, typename _OtherType, typename _OtherTool >
    static constexpr typename Instance< _ThisType, _ThisTool >::HolderType construct ( Instance< _OtherType, _OtherTool > & other )
    {
        return _ThisTool:: template makeHolder< _ThisType >( other );
    }

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
 * Класс для формирования экземпляра значения, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством заданного
 * _ValueTool.
 */
template < typename _ValueType, typename _ValueTool >
class Instance
{
    template < typename >
    friend struct FeatureGuard;

    template < AssignmentVariant >
    friend struct AssignmentHelper;

    template < typename, typename >
    friend class Instance;

    using ThisType = Instance< _ValueType, _ValueTool >;

public:
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using HolderType = typename ValueTool:: template HolderType< _ValueType >;

    enum InitializeFlag { NotInitialized };

private:
    HolderType m_holder;

private:
    constexpr Instance ( HolderType && holder )
    : m_holder( ::std::forward< HolderType >( holder ) )
    {
    }

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать
    /// \sa isNull()
    constexpr Instance ( InitializeFlag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    constexpr Instance ( _Arguments && ... arguments )
    : Instance ( ValueTool:: template makeHolder< ValueType >(
        ::std::forward< _Arguments >( arguments ) ... ) )
    {
        // в этот конструктор нельзя передать Instance, поэтому
        // нет необходимости защищать входящие аргументы
    }

    template < typename _Type >
    ThisType & operator = ( _Type & other )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        vGuard( *this ).value().m_holder
            = ValueTool:: template makeHolder< ValueType >( cGet( other ) );
        return *this;
    }

    template < typename _Type >
    ThisType & operator = ( _Type && other )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        vGuard( *this ).value().m_holder
            = ValueTool:: template makeHolder< ValueType >(
                ::std::forward< _Type >( other ) );
        return *this;
    }

    template < typename _Type >
    ThisType & operator = ( const _Type & other )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        vGuard( *this ).value().m_holder
            = ValueTool:: template makeHolder< ValueType >( other );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    constexpr Instance ( Instance< _OtherType, _OtherTool > & other )
    : Instance( AssignmentHelper< VariantDefiner2< _ValueType, _ValueTool, _OtherType, _OtherTool >::value >
        :: template construct< _ValueType, _ValueTool, _OtherType, _OtherTool >( other ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > & other )
    {
        // TODO: реализовать оптимальный вариант
        return *this = cGet( other );
    }

    template < typename _OtherType, typename _OtherTool >
    constexpr Instance ( Instance< _OtherType, _OtherTool > && other )
    : Instance( AssignmentHelper< VariantDefiner2< _ValueType, _ValueTool, _OtherType, _OtherTool >::value >
        :: template construct< _ValueType, _ValueTool, _OtherType, _OtherTool >(
            ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > && other )
    {
        return *this = mGet( other );
    }

    template < typename _OtherType, typename _OtherTool >
    constexpr Instance ( const Instance< _OtherType, _OtherTool > & other )
    : Instance( AssignmentHelper< VariantDefiner2< _ValueType, _ValueTool, _OtherType, _OtherTool >::value >
        :: template construct< _ValueType, _ValueTool, _OtherType, _OtherTool >( other ) )
    {
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( const Instance< _OtherType, _OtherTool > & other )
    {
        // TODO: реализовать оптимальный вариант
        return *this = cGet( other );
    }

    //! Деструктор.
    ~Instance ()
    {
        ValueTool::destroyHolder( m_holder );
    }
};

/*!
 * Специализация FeatureGuard для Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ReferType = Instance< _ValueType, _ValueTool > &;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( refer )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardWritableHolder( m_refer.m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardWritableHolder( m_refer.m_holder );
    }

    constexpr ReferType value ()
    {
        return m_refer;
    }

    constexpr FeatureGuard< ValueType & > operator -> ()
    {
        return ValueTool::writableValueGuard( m_refer.m_holder );
    }
};

/*!
 * Специализация FeatureGuard для const Instance< _ValueType, _ValueTool > &.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< const Instance< _ValueType, _ValueTool > & >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > & >;

public:
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ReferType = const Instance< _ValueType, _ValueTool > &;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( refer )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardReadableHolder( m_refer.m_holder );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardReadableHolder( m_refer.m_holder );
    }

    constexpr ReferType value ()
    {
        return m_refer;
    }

    constexpr FeatureGuard< const ValueType & > operator -> ()
    {
        return ValueTool::readableValueGuard( m_refer.m_holder );
    }
};

/*!
 * Специализация FeatureGuard для Instance< _ValueType, _ValueTool > &&.
 */
template < typename _ValueType, typename _ValueTool >
struct FeatureGuard< Instance< _ValueType, _ValueTool > && >
{
    using ThisType = FeatureGuard< Instance< _ValueType, _ValueTool > && >;

public:
    using InstanceType = Instance< _ValueType, _ValueTool >;
    using HolderType = typename InstanceType::HolderType;
    using ReferType = Instance< _ValueType, _ValueTool > &&;
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;

private:
    ReferType m_refer;

public:
    FeatureGuard ( ReferType refer )
    : m_refer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
        ValueTool::guardMovableHolder(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }

    FeatureGuard ( ThisType && other )
    : m_refer( ::std::forward< ReferType >( other.m_refer ) ) {}

    FeatureGuard ( const ThisType & other ) = delete;

    ~FeatureGuard ()
    {
        ValueTool::unguardMovableHolder(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }

    constexpr ReferType value ()
    {
        return ::std::forward< ReferType >( m_refer );
    }

    constexpr FeatureGuard< ValueType && > operator -> ()
    {
        return ValueTool::movableValueGuard(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }
};


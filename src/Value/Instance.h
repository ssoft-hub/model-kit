#pragma once
#include <Helper/AccessHelper.h>
#include <memory>

/*!
 * Класс для формирования экземпляра значения, наделенными дополнительными
 * свойствами. Дополнительные свойства реализуются посредством заданного
 * _ValueTool.
 */
template < typename _ValueType, typename _ValueTool >
class Instance
{
    template < typename _WrapperType >
    friend struct FeatureGuard;

    using ThisType = Instance< _ValueType, _ValueTool >;

public:
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using HolderType = typename ValueTool:: template HolderType< _ValueType >;

    enum InitializeFlag { NotInitialized };

private:
    HolderType m_holder;

public:
    //! Конструктор без специальной инициализации значения.
    /// В зависимости от инструмента, значение может не существовать
    /// \sa isNull()
    constexpr Instance ( InitializeFlag )
    {
    }

    //! Конструктор инициализации значения по заданным параметрам
    template < typename ... _Arguments >
    Instance ( _Arguments && ... arguments )
    : m_holder( ValueTool:: template makeValueHolder< ValueType >(
        ::std::forward< _Arguments >( arguments ) ... ) )
    {
        // в этот конструктор нельзя передать Instance, поэтому
        // нет необходимости защищать входящие аргументы
    }

    template < typename _Type >
    ThisType & operator = ( _Type & /*other*/ )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        return *this;
    }

    template < typename _Type >
    ThisType & operator = ( _Type && /*other*/ )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        return *this;
    }

    template < typename _Type >
    ThisType & operator = ( const _Type & /*other*/ )
    {
        // в этот оператор нельзя передать Instance,
        // поэтому необходимо защищать только *this.
        return *this;
    }

    // TODO: защитить аргументы конструкторов и операторов

    Instance ( ThisType && /*other*/ )
    : m_holder( /*::std::forward< HolderType >( other.m_holder )*/ )
    {
        // требуется защитить other
    }

    ThisType & operator = ( ThisType && /*other*/ )
    {
        // требуется защитить *this и other
        //m_holder = ::std::forward< HolderType >( other.m_holder );
        return *this;
    }

    Instance ( ThisType & /*other*/ )
    : m_holder( /*other.m_holder*/ )
    {
        // требуется защитить other
    }

    ThisType & operator = ( ThisType & /*other*/ )
    {
        // требуется защитить *this и other
//        m_holder = other.m_holder;
        return *this;
    }

    Instance ( const ThisType & /*other*/ )
    : m_holder( /*other.m_holder*/ )
    {
        // требуется защитить other
    }

    ThisType & operator = ( const ThisType & /*other*/ )
    {
        // требуется защитить *this и other
        //m_holder = other.m_holder;
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > && /*other*/ )
    : m_holder( /*other.m_holder*/ )
    {
        // требуется защитить other
        // затем other нужно раскрыть либо до ThisType, либо до _ValueType
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > && /*other*/ )
    {
        // требуется защитить *this и other
        // затем other нужно раскрыть либо до ThisType, либо до _ValueType
        //m_holder = ::std::forward< HolderType >( other.m_holder );
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( Instance< _OtherType, _OtherTool > & /*other*/ )
    : m_holder( /*other.m_holder*/ )
    {
        // требуется защитить other
        // затем other нужно раскрыть либо до ThisType, либо до _ValueType
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( Instance< _OtherType, _OtherTool > & /*other*/ )
    {
        // требуется защитить *this и other
        // затем other нужно раскрыть либо до ThisType, либо до _ValueType
        //m_holder = other.m_holder;
        return *this;
    }

    template < typename _OtherType, typename _OtherTool >
    Instance ( const Instance< _OtherType, _OtherTool > & /*other*/ )
    : m_holder( /*other.m_holder*/ )
    {
        // требуется защитить other
        // затем other нужно раскрыть либо до ThisType, либо до _ValueType
    }

    template < typename _OtherType, typename _OtherTool >
    ThisType & operator = ( const Instance< _OtherType, _OtherTool > & /*other*/ )
    {
        // требуется защитить *this и other
        // затем other нужно раскрыть либо до ThisType, либо до _ValueType
        //m_holder = other.m_holder;
        return *this;
    }

    //! Деструктор.
    ~Instance ()
    {
        ValueTool::destroyValueHolder( m_holder );
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
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using ReferType = Instance< _ValueType, _ValueTool > &;

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

    constexpr FeatureGuard< ValueType & > operator -> ()
    {
        return ValueTool:: template getWritableGuard< ValueType >( m_refer.m_holder );
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
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using ReferType = const Instance< _ValueType, _ValueTool > &;

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

    constexpr FeatureGuard< const ValueType & > operator -> ()
    {
        return ValueTool::getReadableGuard( m_refer.m_holder );
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
    using ValueType = _ValueType;
    using ValueTool = _ValueTool;
    using ReferType = Instance< _ValueType, _ValueTool > &&;
    using HolderType = typename Instance< _ValueType, _ValueTool >::HolderType;

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

    constexpr FeatureGuard< ValueType && > operator -> ()
    {
        return ValueTool::getMovableGuard(
            ::std::forward< HolderType >( m_refer.m_holder ) );
    }
};

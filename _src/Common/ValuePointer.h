#pragma once

#include <ModelKit/Common/FeaturePointer.h>

/*!
 * Класс - защитник всех свойств для экземпляра значения.
 * Семантика защитника соответствует семантике работы с указателем.
 * Предоставляет доступ к экземпляру значения посредством унарного оператора "*",
 * а доступ к членам класса - посредством оператора "->".
 */
template < typename _ReferType >
class DefaultValuePointer
{
    using ThisType = DefaultValuePointer< _ReferType >;

public:
    using ReferType = _ReferType;
    using PointerType = FeaturePointer< _ReferType >;
    using AccessType = ReferType;

private:
    PointerType m_feature_guard;

public:
    constexpr DefaultValuePointer ()
            : m_feature_guard()
    {
    }

    DefaultValuePointer ( ReferType refer )
            : m_feature_guard( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    DefaultValuePointer ( PointerType && other )
            : m_feature_guard( ::std::forward< PointerType >( other ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    DefaultValuePointer ( ThisType && other )
            : m_feature_guard( ::std::forward< PointerType >( other.m_feature_guard ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    //constexpr bool operator ! () const
    //{
    //    return !m_feature_guard;
    //}

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( *m_feature_guard );
    }

    constexpr const PointerType & operator -> () const
    {
        return m_feature_guard;
    }
};

/*!
 * Специализация данного шаблона определеяет каким образом защищается значение.
 * По умолчанию для всех видов ссылок используется DefaultValuePointer.
 */
template < typename _ReferType >
struct ValuePointerHelper
{
    using type = DefaultValuePointer< _ReferType >;
};

/*!
 * Класс - защитник свойства значения. Введен для удобства использования,
 * переопределяет typename ValuePointerHelper_t< _ReferType >.
 */
template < typename _ReferType >
using ValuePointer = typename ValuePointerHelper< _ReferType >::type;

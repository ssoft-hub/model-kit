#pragma once

#include <ModelKit/Common/ReferPointer.h>
#include <utility>

/*!
 * Класс - защитник свойства верхнего уровня для экземпляра значения.
 * Семантика защитника соответствует семантике работы с указателем.
 * Предоставляет доступ к экземпляру значения посредством унарного оператора "*",
 * а доступ к членам класса - посредством оператора "->".
 */
template < typename _ReferType >
class DefaultFeaturePointer
{
    using ThisType = DefaultFeaturePointer< _ReferType >;

public:
    using ReferType = _ReferType;
    using AccessType = _ReferType;
    using ValuePointer = ReferPointer< ReferType >;

private:
    ValuePointer m_pointer;

public:
    constexpr DefaultFeaturePointer ()
        : m_pointer()
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    DefaultFeaturePointer ( ReferType refer )
        : m_pointer( ::std::forward< ReferType >( refer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    DefaultFeaturePointer ( ThisType && other )
        : m_pointer( ::std::forward< ValuePointer >( other.m_pointer ) )
    {
        static_assert( ::std::is_reference< ReferType >::value
            , "The template parameter must be a reference." );
    }

    constexpr bool operator ! () const
    {
        return !m_pointer;
    }

    constexpr AccessType operator * () const
    {
        return ::std::forward< AccessType >( *m_pointer );
    }

    constexpr const ValuePointer & operator -> () const
    {
        return m_pointer;
    }
};

/*!
 * Специализация данного шаблона определеяет каким образом защищается значение.
 * По умолчанию для всех видов ссылок используется DefaultFeaturePointer.
 */
template < typename _ReferType >
struct FeaturePointerHelper
{
    using type = DefaultFeaturePointer< _ReferType >;
};

/*!
 * Класс - защитник свойства значения. Введен для удобства использования,
 * переопределяет typename FeaturePointerHelper< _ReferType >::type.
 */
template < typename _ReferType >
using FeaturePointer = typename FeaturePointerHelper< _ReferType >::type;

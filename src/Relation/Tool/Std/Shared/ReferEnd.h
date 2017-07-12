#pragma once
#include <Helper/FeatureGuard.h>
#include <Relation/ReferEnd.h>
#include <Relation/SharedEnd.h>
#include <Relation/UniqueEnd.h>
#include <Relation/Tool/Std/Shared/RelationTool.h>

/*!
 * Полюс ассоциативной связи не агрегации.
 * Специализированная реализация на основе ::std::weak_ptr.
 *
 * Возможны реализации: &, ::std::weak_ptr, QWeakPointer и др.
 */
template < typename _Type >
class ReferEnd< _Type, ::Std::Shared::RelationTool >
//    : public AssociationEnd< _Type, ::Std::Shared::RelationTool >
{
public:
    using ToolType = ::Std::Shared::RelationTool;

private:
    using ThisType = ReferEnd< _Type, ToolType >;
    using ParentType = AssociationEnd< _Type, ToolType >;

public:
    using Holder = typename ToolType:: template ReferHolder< _Type >;

private:
    Holder m_holder;

private:
    ReferEnd ( Holder && holder )
    : m_holder( ::std::forward< Holder >( holder ) )
    {
    }

public:
    /*constexpr*/ ReferEnd ()
    : m_holder()
    {
    }

    ReferEnd ( ThisType && other )
    : m_holder( ::std::forward< Holder >( other.m_holder ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< Holder >( other.m_holder );
        return *this;
    }

    template < typename _Other, typename _Tool >
    ReferEnd ( ReferEnd< _Other, _Tool > && other )
    // TODO: other не обязан иметь m_holder
    : m_holder( ::std::forward< typename ReferEnd< _Other, _Tool >::Holder >( other.m_holder ) )
    {
    }

    template < typename _Other, typename _Tool >
    ThisType & operator = ( ReferEnd< _Other, _Type > && other )
    {
        // TODO: other не обязан иметь m_holder
        m_holder = ::std::forward< typename ReferEnd< _Other, _Tool >::Holder >( other.m_holder );
        return *this;
    }

    ReferEnd ( const ThisType & ) = delete;
    ThisType & operator = ( const ThisType & ) = delete;

//public:
//    virtual AnyType any () const
//    {
//        return AnyType(); /*ToolType::makeForNone< _Type >( m_holder )*/;
//    }

public:
    template < typename _Other, typename _Tool >
    static ThisType refer ( const AssociationEnd< _Other, _Tool > & /*other*/ )
    {
        // TODO: other не обязан иметь m_holder
        return ReferEnd( /*Holder( other.m_holder )*/ );
    }
};

/*!
 * Специализация шаблоного класса для доступа к внутреннему значению обертки
 * полюса ассоциативной связи не являющегося агрегацией, реализованной на основе ::std::weak_ptr.
 *
 * Прямой доступ к внутреннему значению запрещен.
 * Требуется вспомогательный объект Accessor.
 */
//template < typename _Type >
//struct FeatureGuard< ReferEnd< _Type, ::Std::Shared::RelationTool > > {};

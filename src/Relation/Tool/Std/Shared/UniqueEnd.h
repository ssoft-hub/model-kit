#pragma once
#include <Helper/FeatureGuard.h>
#include <Relation/SharedEnd.h>
#include <Relation/Tool/Std/Shared/RelationTool.h>

/*!
 * Полюс ассоциативной связи уникальной агрегации (композиции).
 * Специализированная реализация на основе ::std::shared_ptr.
 *
 * Возможны реализации: inplace, ::std::unique_ptr, ::std::shared_ptr, QSharedPointer, QScopedPointer и др.
 */
template < typename _Type >
class UniqueEnd< _Type, ::Std::Shared::RelationTool >
//    : public AggregationEnd< _Type, ::Std::Shared::RelationTool >
{
public:
    using ToolType = ::Std::Shared::RelationTool;

private:
    using ThisType = UniqueEnd< _Type, ToolType >;
    using ParentType = AggregationEnd< _Type, ToolType >;

public:
    using Holder = typename ToolType:: template SharedHolder< _Type >;
//    using AnyType = typename ParentType::AnyType;
    // TODO: убрать friend struct SharedEnd
    template < typename _Other, typename _Tool > friend struct UniqueEnd;

private:
    Holder m_holder;

private:
    UniqueEnd ( Holder && holder )
    : m_holder( ::std::forward< Holder >( holder ) )
    {
    }

public:
    /*constexpr*/ UniqueEnd ()
    : m_holder()
    {
    }

    UniqueEnd ( ThisType && other )
    : m_holder( ::std::forward< Holder >( other.m_holder ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< Holder >( other );
        return *this;
    }

    template < typename _Other, typename _Tool >
    UniqueEnd ( UniqueEnd< _Other, _Tool > && other )
    // TODO: other не обязан иметь m_holder
    : m_holder( ::std::forward< typename UniqueEnd< _Other, _Tool >::Holder >( other.m_holder ) )
    {
    }

    template < typename _Other, typename _Tool >
    ThisType & operator = ( UniqueEnd< _Other, _Tool > && other )
    {
        // TODO: other не обязан иметь m_holder
        m_holder = ::std::forward< typename UniqueEnd< _Other, _Tool >::Holder >( other.m_holder );
        return *this;
    }

    UniqueEnd ( const ThisType & ) = delete;
    ThisType & operator = ( const ThisType & ) = delete;

//public:
//    virtual AnyType any () const
//    {
//        return AnyType();
//    }

public:
    template < typename ... _Arguments >
    static ThisType make ( _Arguments && ... arguments )
    {
        return ThisType( ::std::make_shared< _Type >( ::std::forward< _Arguments >( arguments ) ... ) );
    }
};

/*!
 * Специализация шаблоного класса для доступа к внутреннему значению обертки
 * полюса ассоциативной связи уникальной агрегации (композиции),
 * реализованной на основе ::std::shared_ptr.
 *
 * Так как сама обертка не изменяется во время доступа к её внутреннему значению,
 * то возвращаемый тип Type & для writable и readable доступа.
 */
//template < typename _Type >
//struct ValueGuard< UniqueEnd< _Type, ::Std::Shared::RelationTool > >
//{
//    using Wrapper = UniqueEnd< _Type, ::Std::Shared::RelationTool >;
//    using Holder = typename Wrapper::Holder;

//    template < typename _Value >
//    static /*constexpr*/ _Value & writable ( Wrapper & wrapper )
//    {
//        return get< _Value >( *wrapper.m_holder.m_pointer.get() );
//    }

//    template < typename _Value >
//    static /*constexpr*/ _Value & readable ( const Wrapper & wrapper )
//    {
//        return get< _Value >( *wrapper.m_holder.m_pointer.get() );
//    }

//    template < typename _Value >
//    static /*constexpr*/ _Value && writable ( Wrapper && wrapper )
//    {
//        return get< _Value >( ::std::forward< Holder >( *wrapper.m_holder.m_pointer.get() ) );
//    }
//};


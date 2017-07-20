#pragma once
#include <Helper/FeatureGuard.h>
#include <Relation/SharedEnd.h>
#include <Relation/Tool/Std/Shared/RelationTool.h>

/*!
 * Полюс ассоциативной связи обобщенной агрегации.
 * Специализированная реализация на основе ::std::shared_ptr.
 *
 * Возможны реализации: ::std::shared_ptr, QSharedPointer и др.
 */
template < typename _Type >
class SharedEnd< _Type, ::Std::Shared::RelationTool >
//    : public AggregationEnd< _Type, ::Std::Shared::RelationTool >
{
public:
    using ToolType = ::Std::Shared::RelationTool;

private:
    using ThisType = SharedEnd< _Type, ToolType >;
//    using ParentType = AggregationEnd< _Type, ToolType >;

public:
    using Holder = typename ToolType:: template SharedHolder< _Type >;
    // TODO: убрать friend struct SharedEnd
    template < typename _Other, typename _Tool > friend struct SharedEnd;

private:
    Holder m_holder;

private:
    SharedEnd ( Holder && holder )
    : m_holder( ::std::forward< Holder >( holder ) )
    {
    }

public:
    /*constexpr*/ SharedEnd ()
    : m_holder()
    {
    }

    SharedEnd ( ThisType && other )
    : m_holder( ::std::forward< Holder >( other.m_holder ) )
    {
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< Holder >( other.m_holder );
        return *this;
    }

    template < typename _Other, typename _Tool >
    SharedEnd ( SharedEnd< _Other, _Type > && other )
    // TODO: other не обязан иметь m_holder
    : m_holder( ::std::forward< typename SharedEnd< _Other, _Type >::Holder >( other.m_holder ) )
    {
    }

    template < typename _Other, typename _Tool >
    ThisType & operator = ( SharedEnd< _Other, _Type > && other )
    {
        // TODO: other не обязан иметь m_holder
        m_holder = ::std::forward< typename SharedEnd< _Other, _Type >::Holder >( other.m_holder );
        return *this;
    }

    SharedEnd ( const ThisType & ) = delete;
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

    template < typename _Other, typename _Tool >
    static ThisType share ( const SharedEnd< _Other, _Tool > & other )
    {
        // TODO: other не обязан иметь m_holder
        return ThisType( Holder( other.m_holder ) );
    }
};

/*!
 * Специализация шаблоного класса для доступа к внутреннему значению обертки
 * полюса ассоциативной связи обобщенной агрегации, реализованной на основе ::std::shared_ptr.
 *
 * Так как сама обертка не изменяется во время доступа к её внутреннему значению,
 * то возвращаемый тип Type & для writable и readable доступа.
 */
//template < typename _Type >
//struct ValueGuard< SharedEnd< _Type, ::Std::Shared::RelationTool > >
//{
//    using Wrapper = SharedEnd< _Type, ::Std::Shared::RelationTool >;
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


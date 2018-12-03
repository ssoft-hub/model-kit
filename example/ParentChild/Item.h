#pragma once

#include <ModelKit.h>
#include <list>

using _Key = int;

template < typename _Type > using ReturnCreate = _Type;
template < typename _Type > using ReturnRead = const _Type &;
template < typename _Type > using ReturnUpdate = _Type &;
template < typename _Type > using InRead = const _Type &;
template < typename _Type > using InUpdate = _Type &;
template < typename _Type > using InOutUpdate = _Type &;
template < typename _Type > using InOutMove = _Type &&;

class Item
{
private:
//    typedef Item< _Key, _Composition, _Aggregation > ThisType;
    using ThisType = Item;

public:
    using UniqueItemEnd = Item *;//CompositeEnd< Item >;
    using NoneItemEnd = Item *;//NoneEnd< Item >;
    typedef ::std::list< UniqueItemEnd > UniqueItemEnds;
    typedef ::std::list< NoneItemEnd > NoneItemEnds;

private:
    Instance< NoneItemEnd > m_parent_end;
    Instance< UniqueItemEnds > m_child_ends;

public:
    Item ()
        : m_parent_end()
    , m_child_ends()
    {
    }

    // Копирование объектов запрещено
    Item ( InRead< Item > other ) = delete;
    Item & operator = ( InRead< Item > other ) = delete;

    // Перемещение объектов разрешено
    Item ( InOutMove< Item > other )
        : m_parent_end( ::std::move( other.m_parent_end ) )
    , m_child_ends( ::std::move( other.m_child_ends ) )
    {
        // TODO: заменить this у m_parent_end.
    }

    Item & operator = ( InOutMove< Item > other )
    {
        m_parent_end = ::std::move( other.m_parent_end );
        m_child_ends = ::std::move( other.m_child_ends );

        // TODO: заменить this у m_parent_end.

        return *this;
    }

    void setParent ( InUpdate< NoneItemEnd > /*parent*/ )
    {
        // TODO:
    }

    ReturnUpdate< NoneItemEnd > parent ()
    {
        return *&m_parent_end;
    }

    ReturnRead< NoneItemEnd > parent () const
    {
        return *&m_parent_end;
    }

    bool appendChild ( InOutUpdate< UniqueItemEnd > /*child*/ )
    {
//        if ( !isValid( child ) )
//            return;
//        ::Memory::wtbl( child ).m_parent = m_self;
//        Key child_key = ::Memory::rdbl( child ).key();
//        m_children[ child_key ] = ::std::forward< Value >( child );
        return false;
    }

    bool removeChild ( InOutUpdate< NoneItemEnd > /*child*/ )
    {
//        auto iter = m_children.find( key );
//        if ( iter == ::std::end( m_children ) )
//            return Value();

//        Value child = ::std::move( (*iter).second );
//            ::Memory::wtbl( child ).m_parent = Reference();
//        m_children.erase( iter );
//        return child;
        return false;
    }

    ReturnUpdate< UniqueItemEnds > children ()
    {
        return *&m_child_ends;
    }

    ReturnRead< UniqueItemEnds > children () const
    {
        return *&m_child_ends;
    }

    // проверка наличия
    ReturnCreate< bool > contains ( InRead< NoneItemEnd > item ) const
    {
//        return m_child_ends.count( item );
        return false;
    }
};

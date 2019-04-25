#include <iostream>
#include "Item.h"

//#include <list>


//template < typename _Item, typename _SubItem >
void parentChildTest ()
{
//    typedef _Item TestItem;
//    typedef _SubItem TestSubItem;
//    typedef typename TestItem::Key Key;
//    typedef typename TestItem::Value Value;

//    using TestItem = Item;
//    using NoneItemEnd = Item::NoneItemEnd;
//    using UniqueItemEnd = Item::UniqueItemEnd;


    ::Scl::Instance< int > child_count = 10;
    ::std::cout
        << ::std::endl << "Begin test" << *&child_count << ::std::endl;

//    Instance< UniqueItemEnd > parent = UniqueItemEnd::make();

//    ::std::cout
//        << "Put children into parent" << ::std::endl
//        << "Children count before: " << (*&parent).children().size()  << ::std::endl;

//    // помещаем элементы в родителя
//    for ( int i = 0; i < child_count; ++i )
//    {
//        Key key = Key( "Child " ) + Key::number( i );
//        writable( parent ).appendChild( TestItem::template make< TestSubItem >( key ) );
//    }
//    ::std::cout
//        << "Children count after: " << readable( parent ).children().size()  << ::std::endl;

//    ::std::cout
//        << "Access children from parent" << ::std::endl;

//    // проверяем доступ
//    for ( int i = 0; i < child_count; ++i )
//    {
//        Key key = Key( "Child " ) + Key::number( i );
//        ::std::cout
//            << "Parent access "
//            << static_cast< const char * >( readable( parent ).child( key ).key() )
//            << ::std::endl;
//    }

//    ::std::cout
//        << "Move children from parent to ::std::list" << ::std::endl
//        << "Children count before: " << readable( parent ).children().size()  << ::std::endl;

//    ::std::list< Value > item_list;
//    // перемещаем элементы в item_list
//    for ( int i = 0; i < child_count; ++i )
//    {
//        Key key = Key( "Child " ) + Key::number( i );
//        item_list.push_back( writable( parent ).removeChild( key ) );
//    }
//    ::std::cout
//        << "Children count after: " << readable( parent ).children().size()  << ::std::endl;

//    ::std::cout
//        << "Access items from ::std::list" << ::std::endl;

//    for ( auto & item : item_list )
//    {
//        ::std::cout
//            << "List access "
//            << static_cast< const char * >( readable( item ).key() )
//            << ::std::endl;
//    }
}

//struct Value
//{
//    void barMutable () {}
//    void barConst () const {}
//};
//typedef const Value ConstValue;

//template < typename  _Value >
//using Collection = ::std::unique_ptr< _Value >;

//void fooInOutUpdate ( Collection< Value > & value ) // InOutUpdate || InOutDelete
//{
//    value.reset(); // забрали владение, положили ничто
//}

//void fooInOutUpdateR ( Collection< Value > && value ) // InOutUpdate || InOutDelete
//{
//    value.reset(); // забрали владение, положили ничто
//}

//void fooInUpdate ( const Collection< Value > & value ) // InUpdate || InDelete
//{
//    value.get()->barConst();
//    value.get()->barMutable();
//}

//void foo ( Collection< Value > & value ) // InOutUpdate || InOutDelete
//{
//    fooInOutUpdate( value );
//    fooInOutUpdateR( ::std::forward< Collection< Value > >( value ) );
//}

////void foo ( Collection< Value > && value ) // InOutUpdate || InOutDelete
////{
////    fooInOutUpdateR( ::std::forward< Collection< Value > >( value ) );
////}

//void foo ( const Collection< Value > & value ) // InOutUpdate || InOutDelete
//{
//    fooInUpdate( value );
//}

////void fooInOutRead ( Collection< const Value > & value ) // InOutRead
////{
////}

////void fooInRead ( const Collection< const Value > & value ) // // InRead
////{
////}

//void goo ( ConstValue * value )
//{
//    value = new ConstValue();
//}

int main ( int /*narg*/, char ** /*varg*/ )
{
//    Collection< Value > value;
//    Collection< Value > & lvalue = value;
//    const Collection< Value > & cvalue = value;
//    fooInOutUpdate( value );
//    fooInUpdate( value );
////    fooInOutRead( value );
////    fooInRead( value );

//    fooInOutUpdateR( ::std::move( value ) ); // static_cast
//    fooInOutUpdateR( ::std::forward< Collection< Value > >( value ) ); //  static_cast
//    fooInOutUpdateR( ::std::forward< Collection< Value > >( lvalue ) ); // static_cast

////    fooInOutUpdate( Collection< Value >() ); // ERROR
//    fooInOutUpdateR( Collection< Value >() ); // OK
//    fooInUpdate( Collection< Value >() );
////    fooInOutRead( Collection< Value >() );
////    fooInRead( Collection< Value >() );

//    foo ( value );
//    foo ( lvalue );
//    foo ( cvalue );
//    foo ( Collection< Value >() );

//    Collection< Value >().reset();


//    return 0;

//    typedef QByteArray Key;

//    // организация композиции и агрегации
////    typedef Item< Key, ::Memory::Inplace::Composition, ::Memory::Reference::Aggregation > InplaceItem;
////    parentChildTest< InplaceItem, InplaceItem >();
////    typedef SubItem< Key, ::Memory::Inplace::Composition, ::Memory::Reference::Aggregation > InplaceSubItem;
////    parentChildTest< InplaceItem, InplaceSubItem >();

//    typedef Item< Key, ::Memory::StdUnique::Composition, ::Memory::Reference::Aggregation > UniqueRefItem;
//    parentChildTest< UniqueRefItem, UniqueRefItem >();
//    typedef SubItem< Key, ::Memory::StdUnique::Composition, ::Memory::Reference::Aggregation > UniqueRefSubItem;
//    parentChildTest< UniqueRefItem, UniqueRefSubItem >();

//    typedef Item< Key, ::Memory::StdShared::Composition, ::Memory::Reference::Aggregation > SharedRefItem;
//    parentChildTest< SharedRefItem, SharedRefItem >();
//    typedef SubItem< Key, ::Memory::StdShared::Composition, ::Memory::Reference::Aggregation > SharedRefSubItem;
//    parentChildTest< SharedRefItem, SharedRefSubItem >();

//    typedef Item< Key, ::Memory::StdShared::Composition, ::Memory::StdWeak::Aggregation > SharedWeakItem;
//    parentChildTest< SharedWeakItem, SharedWeakItem >();
//    typedef SubItem< Key, ::Memory::StdShared::Composition, ::Memory::StdWeak::Aggregation > SharedWeakSubItem;
//    parentChildTest< SharedWeakItem, SharedWeakSubItem >();

////    // неверный вариант Aggregation
////    typedef Item< Key, ::Memory::StdShared::Composition, ::Memory::StdShared::Composition > SharedSharedItem;
////    parentChildTest< SharedSharedItem, SharedSharedItem >();
////    typedef SubItem< Key, ::Memory::StdShared::Composition, ::Memory::StdShared::Composition > SharedSharedSubItem;
////    parentChildTest< SharedSharedItem, SharedSharedSubItem >();

//    // ссылка на объект может стать невалидной, так не работает (и не должно)
////    typedef Item< Key, ::Memory::StdImplicitShared::Composition, ::Memory::Reference::Aggregation > ImplicitSharedRefItem;
////    parentChildTest< ImplicitSharedRefItem, ImplicitSharedRefItem >();
////    typedef SubItem< Key, ::Memory::StdImplicitShared::Composition, ::Memory::Reference::Aggregation > ImplicitSharedRefSubItem;
////    parentChildTest< ImplicitSharedRefItem, ImplicitSharedRefSubItem >();

//    // Для Implicit shared не существует ассоциативной связи weak типа!!!
////    typedef Item< Key, ::Memory::StdImplicitShared::Composition, ::Memory::StdWeak::Aggregation > ImplicitSharedWeakItem;
////    parentChildTest< ImplicitSharedWeakItem, ImplicitSharedWeakItem >();
////    typedef SubItem< Key, ::Memory::StdImplicitShared::Composition, ::Memory::StdWeak::Aggregation > ImplicitSharedWeakSubItem;
////    parentChildTest< ImplicitSharedWeakItem, ImplicitSharedWeakSubItem >();

//    typedef Item< Key, ::Memory::QtScoped::Composition, ::Memory::Reference::Aggregation > QtScopedRefItem;
//    parentChildTest< QtScopedRefItem, QtScopedRefItem >();
//    typedef SubItem< Key, ::Memory::QtScoped::Composition, ::Memory::Reference::Aggregation > QtScopedRefSubItem;
//    parentChildTest< QtScopedRefItem, QtScopedRefSubItem >();

//    typedef Item< Key, ::Memory::QtShared::Composition, ::Memory::Reference::Aggregation > QtSharedRefItem;
//    parentChildTest< QtSharedRefItem, QtSharedRefItem >();
//    typedef SubItem< Key, ::Memory::QtShared::Composition, ::Memory::Reference::Aggregation > QtSharedRefSubItem;
//    parentChildTest< QtSharedRefItem, QtSharedRefSubItem >();

//    typedef Item< Key, ::Memory::QtShared::Composition, ::Memory::QtWeak::Aggregation > QtSharedWeakItem;
//    parentChildTest< QtSharedWeakItem, QtSharedWeakItem >();
//    typedef SubItem< Key, ::Memory::QtShared::Composition, ::Memory::QtWeak::Aggregation > QtSharedWeakSubItem;
//    parentChildTest< QtSharedWeakItem, QtSharedWeakSubItem >();

////    typedef Item< Key, ::Memory::QtSharedData::Composition, ::Memory::Reference::Aggregation > QtSharedDataRefItem;
////    parentChildTest< QtSharedDataRefItem, QtSharedDataRefItem >();
////    typedef SubItem< Key, ::Memory::QtSharedData::Composition, ::Memory::Reference::Aggregation > QtSharedDataRefSubItem;
////    parentChildTest< QtSharedDataRefItem, QtSharedDataRefSubItem >();

////    typedef Item< Key, ::Memory::QtImplicitShared::Composition, ::Memory::Reference::Aggregation > QtImplicitSharedRefItem;
////    parentChildTest< QtImplicitSharedRefItem, QtImplicitSharedRefItem >();
//////    typedef SubItem< Key, ::Memory::QtImplicitShared::Composition, ::Memory::Reference::Aggregation > QtImplicitSharedRefSubItem;
//////    parentChildTest< QtImplicitSharedRefItem, QtImplicitSharedRefSubItem >();

////    typedef Item< Key, ::Memory::QtImplicitShared::Composition, ::Memory::QtWeak::Aggregation > QtImplicitSharedWeakItem;
////    parentChildTest< QtImplicitSharedWeakItem, QtImplicitSharedWeakItem >();
//////    typedef SubItem< Key, ::Memory::QtImplicitShared::Composition, ::Memory::Weakerence::Aggregation > QtImplicitSharedWeakSubItem;
//////    parentChildTest< QtImplicitSharedWeakItem, QtImplicitSharedWeakSubItem >();

    return 0;
}


//typedef int Type;
//typedef const Type ConstType;

//////////////////////////////////////////////////////////////////////////////////
/////
/////
//template < typename _Type >
//struct Wtbl {};

//template < typename _Type >
//Type & wtbl ( _Type & value ) { return Wtbl< _Type >::wtbl( value ); }

//////////////////////////////////////////////////////////////////////////////////
/////
/////
//template < typename _Type >
//struct Rdbl {};

//template < typename _Type >
//const Type & rdbl ( _Type & value ) { return Rdbl< _Type >::rdbl( value ); }

//////////////////////////////////////////////////////////////////////////////////
/////
/////
//template < typename _Type >
//void create ( _Type * value ) { value = new _Type; }

//template < typename _Type >
//void destroy ( _Type * value ) { delete value; }

//template < typename _Type >
//void reset ( _Type * value ) { value = 0; }

//template < typename _Type >
//_Type & link ( _Type & value ) { return value; }

//template < typename _Type >
//const _Type & link ( const _Type & value ) { return value; }


////#define SCL_SIMPLEST
//#define SCL_WRIPERS

//#ifdef SIMPLEST
//    typedef Type * TypeInstance;
//    typedef ConstType * ConstTypeInstance;
//    typedef const TypeInstance TypeConstInstance;
//    typedef const ConstTypeInstance ConstTypeConstInstance;

//    typedef Type * TypeReference;
//    typedef ConstType * ConstTypeReference;
//    typedef const TypeReference TypeConstReference;
//    typedef const ConstTypeReference ConstTypeConstReference;

//    typedef ConstTypeConstInstance InRead;
//    typedef TypeConstInstance InUpdate;
//    typedef TypeConstInstance InDelete;
//    typedef TypeInstance InOutCreate;
//    typedef ConstTypeInstance InOutRead;
//    typedef TypeInstance InOutUpdate;
//    typedef TypeInstance InOutDelete;
//    typedef TypeInstance OutCreate;
//    typedef ConstTypeInstance OutRead;
//    typedef TypeInstance OutUpdate;
//    typedef TypeInstance ReturnCreate;
//    typedef ConstTypeInstance ReturnRead;
//    typedef TypeInstance ReturnUpdate;

//    template < typename _Type >
//    struct Wtbl< _Type * > { static _Type & wtbl ( _Type * value ) { return *value; } };

//    template < typename _Type >
//    struct Wtbl< _Type * const > { static _Type & wtbl ( _Type * const value ) { return *value; } };

//    template < typename _Type >
//    struct Rdbl< _Type * > { static const _Type & rdbl ( _Type * value ) { return *value; } };

//    template < typename _Type >
//    struct Rdbl< const _Type * > { static const _Type & rdbl ( const _Type * value ) { return *value; } };

//    template < typename _Type >
//    struct Rdbl< _Type * const > { static const _Type & rdbl ( _Type * const value ) { return *value; } };

//    template < typename _Type >
//    struct Rdbl< const _Type * const > { static const _Type & rdbl ( const _Type * const value ) { return *value; } };
//#endif

//#ifdef WRIPERS

//    typedef ::Memory::Wrapper< Type, ::Memory::StdUnique::Composition > TypeInstance;
//    typedef ::Memory::Wrapper< const Type, ::Memory::StdUnique::Composition > ConstTypeInstance;
//    typedef const TypeInstance TypeConstInstance;
//    typedef const ConstTypeInstance ConstTypeConstInstance;

//    typedef ::Memory::Wrapper< Type, ::Memory::StdUnique::Composition > TypeReference;
//    typedef ::Memory::Wrapper< const Type, ::Memory::StdUnique::Composition > ConstTypeReference;
//    typedef const TypeReference TypeConstReference;
//    typedef const ConstTypeReference ConstTypeConstReference;

////    typedef ::Memory::Wrapper< Type, ::Memory::Reference::Aggregation > TypeReference;
////    typedef ::Memory::Wrapper< const Type, ::Memory::Reference::Aggregation > ConstTypeReference;
////    typedef const TypeReference TypeConstReference;
////    typedef const ConstTypeReference ConstTypeConstReference;

//    template < typename _Type, template < typename _Type > class _AggregationKids >
//    struct Rdbl< ::Memory::Wrapper< _Type, _AggregationKids > >
//    {
//        static const _Type & rdbl ( const ::Memory::Wrapper< _Type, _AggregationKids > & value )
//        {
//            return ::Memory::Wrapper< _Type, _AggregationKids >::readable( value );
//        }
//    };

//    template < typename _Type, template < typename _Type > class _AggregationKids >
//    struct Wtbl< ::Memory::Wrapper< _Type, _AggregationKids > >
//    {
//        static _Type & wtbl ( ::Memory::Wrapper< _Type, _AggregationKids > & value )
//        {
//            return ::Memory::Wrapper< _Type, _AggregationKids >::writable( value );
//        }
//    };

//    template < typename _Type, template < typename _Type > class _AggregationKids >
//    struct InReadT
//    {
//        typedef ::Memory::Wrapper< _Type, _AggregationKids > TypeWrapper;
////        typedef ::Memory::Wrapper< const _Type, _AggregationKids > ConstTypeWrapper;

//        InReadT ( const TypeWrapper & value )
//            : m_reference( value )
//        {}
////        InReadT ( const ConstTypeWrapper & value )
////            : m_reference( value )
////        {}
//        const TypeWrapper & m_reference;
//    };

//    template < typename _Type, template < typename _Type > class _AggregationKids >
//    struct Rdbl< InReadT< _Type, _AggregationKids > >
//    {
//        static const _Type & rdbl ( const InReadT< _Type, _AggregationKids > & value )
//        {
//            return ::Memory::Wrapper< _Type, _AggregationKids >::readable( value.m_reference );
//        }
//    };

//    template < typename _Type, template < typename _Type > class _AggregationKids >
//    struct Wtbl< InReadT< _Type, _AggregationKids > >
//    {
//        static _Type & wtbl ( const ::Memory::Wrapper< _Type, _AggregationKids > & value )
//        {
//            return ::Memory::Wrapper< _Type, _AggregationKids >::writable( value );
//        }
//    };

//    typedef InReadT< Type, ::Memory::StdUnique::Composition > InRead;

//#endif
///*
//struct Test
//{
//    Composition m_strong;
//    Aggregation m_weak;

//    // возможно чтение данных по связи
//    void setReadableValue ( InRead value )
//    {
//        ::wtbl( m_strong ) = ::rdbl( value );
//        ::wtbl( m_weak ) = ::rdbl( value );
//    }

//    // возможно изменение данных по связи
//    void correctValue ( InUpdate value )
//    {
//        setReadableValue( value ); // иногда может быть InRead

//        wtbl( value ) = rdbl( m_strong );
//        wtbl( value ) = rdbl( m_weak );
//    }

//    // возможно удаление данных по связи
//    void destroyValue ( InDelete value )
//    {
//        setReadableValue( value ); // иногда может быть InRead
//        correctValue( value ); // иногда может быть InUpdate

//        destroy( value );
//        //reset( value ); // ERROR
//    }

//    // возможно чтение данных по связи
//    // возможна новая связь с новыми данными
//    void setAndCreateValue ( InOutCreate value )
//    {
//        setReadableValue( value ); // всегда может быть InRead
//        setAndGetReadableValue( value ); // иногда может быть InOutRead
//        correctAndGetWritableValue( value ); // иногда может быть InOutUpdate

//        create( value );
//    }

//    // возможна новая связь с данными для чтения
//    void setAndGetReadableValue ( InOutRead value )
//    {
//        setReadableValue( value ); // всегда может быть InRead
//        getReadableValue( value ); // всегда может быть OutRead
//    }

//    // возможна новая связь с данными для записи
//    void correctAndGetWritableValue ( InOutUpdate value )
//    {
//        correctValue( value ); // всегда может быть InUpdate
//        getWritableValue( value ); // всегда может быть InUpdate
//    }

//    // возможно удаление данных по связи
//    // и новая связь с данными
//    void destroyAndGetValue ( InOutDelete value )
//    {
//        setAndGetReadableValue( value ); // иногда может быть InOutRead
//        correctAndGetWritableValue( value ); // иногда может быть InOutUpdate
//        destroyValue( value ); // всегда может быть InDelete
//        reset( value );
//    }

//    // возможно созание новой связи с новыми данными
//    void createValue ( OutCreate value )
//    {
//        create( value );
//    }

//    // возможна новая связь с данными для чтения
//    void getReadableValue ( OutRead value )
//    {
//        link( value ) = link( m_strong );
//        link( value ) = link( m_weak );
//    }

//    // возможна новая связь с данными для записи
//    void getWritableValue ( InOutUpdate value )
//    {
//        link( value ) = link( m_strong );
//        link( value ) = link( m_weak );
//    }

//    ReturnCreate createValue () const
//    {
//        return ReturnCreate();
//    }
//    ReturnRead getValue () const
//    {
//        return m_strong;
//        return m_weak;
//    }
//    ReturnUpdate getValue ()
//    {
//        return m_strong;
//        return m_weak;
//    }
//};

//void foo ()
//{
//    Test test;
//    TypeInstance type_instance;
//    InRead in_read( const_type_instance );
////    ConstTypeInstance const_type_instance;

//    test.setReadableValue( type_instance );
//    test.setReadableValue( in_read );
////    test.setReadableValue( in_read );
//}
//*/

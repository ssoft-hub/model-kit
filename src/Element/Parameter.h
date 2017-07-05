#pragma once

//template < typename _Type >
//using ReturnCreate = Inplace< typename InplaceHelper< _Type >::Type >;

//template < typename _Type >
//using ReturnRead = Inplace< const typename InplaceHelper< _Type >::Type & >;

//template < typename _Type >
//using ReturnUpdate = Inplace< typename InplaceHelper< _Type >::Type & >;


template < typename _Type >
class ReturnCreate;

template < typename _Type >
class ReturnRead;

template < typename _Type >
class ReturnUpdate;

template < typename _Type >
class InRead;

template < typename _Type >
class InUpdate;

template < typename _Type >
class InDelete;

template < typename _Relation >
class InOutCreate;

template < typename _Relation >
class InOutRead;

template < typename _Relation >
class InOutUpdate;

template < typename _Relation >
class InOutDelete;

template < typename _Relation >
class OutCreate;

template < typename _Relation >
class OutRead;

template < typename _Relation >
class OutUpdate;

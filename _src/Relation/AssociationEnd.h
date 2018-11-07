#pragma once

#include <ModelKit/Common/AggregationKind.h>
#include <ModelKit/Common/AsConst.h>
#include <ModelKit/Common/InitializeType.h>
#include "private/AssociationFeaturePointer.h"
#include "private/AssociationOperation.h"
#include "private/AssociationValuePointer.h"

/*!
 * Полюс ассоциативной связи со значением. Подразумевает возможность измненения
 * внутреннего значения без изменения самого полюса.
 *
 * Первый параметр определяет тип значения, второй - средства реализации полюса
 * ассоциативной связи.
 *
 * Ассоциативные связи, реализованные посредством разных инструментов не
 * являются совместимыми. Однако в некоторых случаях можно реализовать
 * преобразование из одной реализации в другую.
 */
template < typename _ValueType, AggregationKind _kind, typename _RelationTool >
class AssociationEnd
{
    template < typename >
    friend struct AssociationValuePointer;

    template < typename >
    friend struct AssociationFeaturePointer;

    using ThisType = AssociationEnd< _ValueType, _kind, _RelationTool >;

public:
    using ValueType = _ValueType;
    using RelationTool = _RelationTool;
    using HolderType = typename RelationTool:: template HolderType< _ValueType >;

private:
    HolderType m_holder;

private:
    //! Конструктор инициализации значения по заданным параметрам.
    template < typename ... _Arguments >
    constexpr AssociationEnd ( InitializeType, _Arguments && ... arguments )
        : m_holder( ::std::forward< _Arguments >( arguments ) ... )
    {
    }

public:
    //! Конструктор по умолчанию формрует пустой конец ассоциативной связи,
    /// без привязки к значению ( /sa make).
    constexpr AssociationEnd ()
    {
    }

    //! Конструкторы перемещения
    AssociationEnd ( ThisType && other )
        : m_holder( ::std::forward< HolderType >( other.m_holder ) )
    {
    }

    template < typename _OtherType, AggregationKind _other_kind, typename _OtherTool >
    AssociationEnd ( AssociationEnd< _OtherType, _other_kind, _OtherTool > && /*other*/ )
//        : m_holder( AssociationBuilder< _ValueType, _ValueTool, Instance< _OtherType, _OtherTool > && >
//        ::resolve( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ) )
    {
    }

    //! Конструкторы копирования
    AssociationEnd ( const ThisType & other )
        : m_holder( other.m_holder )
    {
    }

    template < typename _OtherType, AggregationKind _other_kind, typename _OtherTool >
    AssociationEnd ( const AssociationEnd< _OtherType, _other_kind, _OtherTool > & /*other*/ )
//        : m_holder( AssociationBuilder< _ValueType, _ValueTool, Instance< _OtherType, _OtherTool > && >
//        ::resolve( ::std::forward< Instance< _OtherType, _OtherTool > >( other ) ) )
    {
    }

    //! Операторы присвоения rvalue значений
    template < typename _Type >
    ThisType & operator = ( _Type && other )
    {
        // ??
        m_holder = ::std::forward< _Type >( other );
        return *this;
    }

    ThisType & operator = ( ThisType && other )
    {
        m_holder = ::std::forward< HolderType >( other.m_holder );
        return *this;
    }

    template < typename _OtherType, AggregationKind _other_kind, typename _OtherTool >
    ThisType & operator = ( AssociationEnd< _OtherType, _other_kind, _OtherTool > && /*other*/ )
    {
//        using OtherType = Instance< _OtherType, _OtherTool >;
//        m_holder = InstanceBuilder< _ValueType, _ValueTool, OtherType && >
//            ::resolve( ::std::forward< OtherType >( other ) );
        return *this;
    }

    /// Операторы присвоения lvalue значения
    template < typename _Type >
    ThisType & operator = ( const _Type & other )
    {
        // ??
        return *this;
    }

    ThisType & operator = ( const ThisType & other )
    {
        m_holder = other.m_holder;
        return *this;
    }

    /// Операторы преобразования к типу
    operator AssociationEnd< const _ValueType, _kind, _RelationTool > & ()
    {
        return reinterpret_cast< AssociationEnd< const _ValueType, _kind, _RelationTool > & >( *this );
    }

    operator const AssociationEnd< const _ValueType, _kind, _RelationTool > & () const
    {
        return reinterpret_cast< const AssociationEnd< const _ValueType, _kind, _RelationTool > & >( *this );
    }

    // NOTE: Используется доступ через оператор "->", но семантически необходим ".".
    // TODO: Заменить на оператор "." с внедрением N4477 или P0352 в стандарт C++.
    ValuePointer< ThisType & > operator -> ()
    {
        return *this;
    }

    ValuePointer< const ThisType & > operator -> () const
    {
        return *this;
    }

    ValuePointer< ThisType & > operator & ()
    {
        return *this;
    }

    ValuePointer< const ThisType & > operator & () const
    {
        return *this;
    }

public:
    template < typename ... _Arguments >
    static constexpr ThisType make ( _Arguments && ... arguments )
    {
        static_assert( _kind != AggregationKind::None, "Make not supported for none aggregation type." );
        return ThisType( InitializeType::PrivateInitialization, ::std::forward< _Arguments >( arguments ) ... );
    }

    template < typename _OtherType >
    static ThisType copy ( const _OtherType & other )
    {
        return copy( FeaturePointer< const _OtherType & >( other ).value() );
    }

    template < typename _OtherType >
    static ThisType share ( const _OtherType & other )
    {
        return share( FeaturePointer< const _OtherType & >( other ).value() );
    }

    template < typename _OtherType >
    static ThisType refer ( const _OtherType & other )
    {
        return refer( FeaturePointer< const _OtherType & >( other ).value() );
    }

    template < typename _OtherType, AggregationKind _other_kind, typename _OtherTool >
    static ThisType copy ( const AssociationEnd< _OtherType, _other_kind, _OtherTool > & /*other*/ )
    {
        return ThisType();
    }

    template < typename _OtherType, AggregationKind _other_kind, typename _OtherTool >
    static ThisType share ( const AssociationEnd< _OtherType, _other_kind, _OtherTool > & /*other*/ )
    {
        static_assert( _kind == AggregationKind::Shared && _other_kind == AggregationKind::Shared,
            "Share not supported for not shared aggregation type." );
        return ThisType();
    }

    template < typename _OtherType, AggregationKind _other_kind, typename _OtherTool >
    static ThisType refer ( const AssociationEnd< _OtherType, _other_kind, _OtherTool > & /*other*/ )
    {
        static_assert( _kind == AggregationKind::None, "Refer operation is supported for none aggregation type only." );
        return ThisType();
    }
};

#include <ModelKit/Relation/Tool/Cpp/Inplace/DefaultTool.h>

template < typename _ValueType, typename _RelationTool = ::Relation::Cpp::Inplace::DefaultTool >
using NoneEnd = AssociationEnd< _ValueType, AggregationKind::None, _RelationTool >;

template < typename _ValueType, typename _RelationTool = ::Relation::Cpp::Inplace::DefaultTool >
using SharedEnd = AssociationEnd< _ValueType, AggregationKind::Shared, _RelationTool >;

template < typename _ValueType, typename _RelationTool = ::Relation::Cpp::Inplace::DefaultTool >
using CompositeEnd = AssociationEnd< _ValueType, AggregationKind::Composite, _RelationTool >;

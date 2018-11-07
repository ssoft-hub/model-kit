#pragma once
#include <ModelKit/Common/FeaturePointer.h>
#include <Relation/ReferEnd.h>
#include <Relation/Tool/Cpp/Raw/RelationTool.h>

/*!
 * Полюс ассоциативной связи не агрегации.
 * Специализированная реализация на основе ::std::weak_ptr.
 *
 * Возможны реализации: &, ::std::weak_ptr, QWeakPointer и др.
 */
template < typename _Type >
class ReferEnd< _Type, ::Cpp::Raw::RelationTool >
    : public AssociationEnd< _Type, ::Cpp::Raw::RelationTool >
{
private:
    using ToolType = ::Cpp::Raw::RelationTool;
    using ThisType = ReferEnd< _Type, ToolType >;
    using ParentType = AssociationEnd< _Type, ToolType >;

public:
    using Holder = typename ToolType:: template ReferHolder< _Type >;
//    using AnyType = typename ParentType::AnyType;

private:
    Holder m_holder;

public:
//    virtual AnyType any () const
//    {
//        return ToolType::makeForNone< _Type >( m_holder );
//    }
};

/*!
 * Специализация шаблоного класса для доступа к внутреннему значению обертки
 * полюса ассоциативной связи не являющегося агрегацией, реализованной на основе ::std::weak_ptr.
 *
 * Прямой доступ к внутреннему значению запрещен.
 * Требуется вспомогательный объект Accessor.
 */
//template < typename _Type >
//struct ValuePointer< ReferEnd< _Type, ::Cpp::Raw::RelationTool > > {};

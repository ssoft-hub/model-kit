#pragma once
#ifndef SCL_GUARD_FEATURE_POINTER_H
#define SCL_GUARD_FEATURE_POINTER_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include <ModelKit/Featuring/Traits.h>
#include "ReferPointer.h"

// Предопределение
template < typename _Value, typename _Tool >
class Instance;

namespace Private
{
    template < typename _Refer >
    struct InstanceGuardHelper;
}

/*!
 * Указатель на значение, к которому применено дополнительное свойство.
 */
template < typename _Refer >
using InstanceGuard = typename Private::InstanceGuardHelper< _Refer >::Type;

namespace Private
{
    /*!
     * Указатель на экземпляр вложенного в Instance значения, к которому применена
     * особенность, реализуемая данным Instance. Данный указатель применяется, если
     * тип вложенного экземпляра значения сам не является Instance.
     */
    template < typename _Refer >
    class DefaultInstanceGuard
    {
        using ThisType = DefaultInstanceGuard< _Refer >;

    public:
        using Refer = _Refer;
        using ReferPointer = ::ReferPointer< Refer >;

        using ValueAccess = Refer;
        using PointerAccess = ReferPointer const &;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( !::is_instance< ::std::decay_t< Refer > >, "The template parameter _Refer must to be a not Instance type reference!" );

    private:
        ReferPointer m_refer_pointer;

    public:
        DefaultInstanceGuard ( Refer refer )
            : m_refer_pointer( ::std::forward< Refer >( refer ) )
        {
        }

        DefaultInstanceGuard ( ThisType && other )
            : m_refer_pointer( ::std::forward< ReferPointer >( other.m_refer_pointer ) )
        {
        }

        ValueAccess valueAccess () const
        {
            return ::std::forward< ValueAccess >( *m_refer_pointer );
        }

        PointerAccess pointerAccess () const
        {
            return m_refer_pointer;
        }
    };
}

#include <QtGlobal>

namespace Private
{
    /*!
     * Гарант применения особенностей к экземпляру вложенного в Instance значения.
     * Данный вариант применяется, если тип вложенного экземпляра значения сам является Instance.
     */
    template < typename _Refer >
    class SpecialInstanceGuard
    {
        using ThisType = SpecialInstanceGuard< _Refer >;

    public:
        using InstanceRefer = _Refer;
        using Instance = ::std::decay_t< InstanceRefer >;
        using Value = typename Instance::Value;
        using ValueRefer = ::SimilarRefer< Value, InstanceRefer >;
        using Holder = typename Instance::Holder;
        using HolderRefer = ::SimilarRefer< Holder, InstanceRefer >;
        using ReferPointer = ::ReferPointer< InstanceRefer >;

        using InstanceAccess = ValueRefer;
        using HolderAccess = HolderRefer;

        static_assert( ::std::is_reference< InstanceRefer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( ::is_instance< Instance >, "The template parameter _Refer must to be a Instance type reference!" );
        //static_assert( ::is_similar< ValueRefer, InstanceRefer >, "The Refer and ValueRefer must to be similar types!" );
        static_assert( ::is_similar< HolderRefer, InstanceRefer >, "The Refer and HolderRefer must to be similar types!" );

    private:
        ReferPointer m_refer_pointer;

    private:
        SpecialInstanceGuard ( const ThisType & other ) = delete;

    public:
        SpecialInstanceGuard ( InstanceRefer refer )
            : m_refer_pointer( ::std::forward< InstanceRefer >( refer ) )
        {
            static_assert( ::HolderInternal::is_value_method_exists< Holder, HolderRefer >
                , "There are no appropriate access methods for Holder." );
            ::HolderInternal::guard< HolderRefer >( ::std::forward< HolderRefer >( m_refer_pointer->m_holder ) );
        }

        SpecialInstanceGuard ( ThisType && other )// = delete;
            : m_refer_pointer( ::std::forward< ReferPointer >( other.m_refer_pointer ) )
        {
        }

        ~SpecialInstanceGuard ()
        {
            if ( !!m_refer_pointer )
                ::HolderInternal::unguard< HolderRefer >( ::std::forward< HolderRefer >( m_refer_pointer->m_holder ) );
        }

        InstanceAccess instanceAccess () const
        {
            assert( m_refer_pointer );
            return ::HolderInternal::value< HolderRefer >( ::std::forward< HolderRefer >( m_refer_pointer->m_holder ) );
        }

        HolderAccess holderAccess () const
        {
            assert( m_refer_pointer );
            return ::std::forward< HolderAccess >( m_refer_pointer->m_holder );
        }
    };
}

namespace Private
{
    struct ValueCase;
    struct InstanceCase;

    template < typename _Refer >
    struct InstanceGuardCaseHelper
    {
        using Type = ::std::conditional_t< ::is_instance< ::std::decay_t< _Refer > >,
            InstanceCase,
            ValueCase >;
    };

    template < typename _Refer >
    using InstanceGuardSwitchCase = typename InstanceGuardCaseHelper< _Refer >::Type;

    template < typename, typename >
    struct InstanceSwith;

    template < typename _Refer >
    struct InstanceSwith< ValueCase, _Refer >
    {
        using Type = ::Private::DefaultInstanceGuard< _Refer >;
    };

    template < typename _Refer >
    struct InstanceSwith< InstanceCase, _Refer >
    {
        using Type = ::Private::SpecialInstanceGuard< _Refer >;
    };

    template < typename _Refer >
    struct InstanceGuardHelper
    {
        static_assert( ::std::is_reference< _Refer >::value, "The template parameter _Refer must to be a reference type." );
        using Type = typename InstanceSwith< InstanceGuardSwitchCase< _Refer >, _Refer >::Type;
    };
}

#endif

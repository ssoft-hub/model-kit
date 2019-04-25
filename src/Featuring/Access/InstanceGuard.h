#pragma once
#ifndef SCL_GUARD_FEATURE_POINTER_H
#define SCL_GUARD_FEATURE_POINTER_H

#include <ModelKit/Featuring/Access/HolderInternal.h>
#include "ReferPointer.h"

namespace SclPrivate { template < typename _Value, typename _Tool > class Instance; }
namespace SclPrivate { template < typename _Refer > struct InstanceGuardHelper; }

namespace SclPrivate
{
    template < typename _Refer >
    using InstanceGuard = typename ::SclPrivate::InstanceGuardHelper< _Refer >::Type;
}

namespace SclPrivate
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
        using ReferPointer = ::SclPrivate::ReferPointer< Refer >;

        using ValueAccess = Refer;
        using PointerAccess = ReferPointer const &;

        static_assert( ::std::is_reference< Refer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( !::Scl::is_instance< ::std::decay_t< Refer > >, "The template parameter _Refer must to be a not Instance type reference!" );

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

namespace SclPrivate
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
        using ValueRefer = ::Scl::SimilarRefer< Value, InstanceRefer >;
        using Holder = typename Instance::Holder;
        using HolderRefer = ::Scl::SimilarRefer< Holder, InstanceRefer >;
        using ReferPointer = ::SclPrivate::ReferPointer< InstanceRefer >;

        using InstanceAccess = ValueRefer;
        using HolderAccess = HolderRefer;

        static_assert( ::std::is_reference< InstanceRefer >::value, "The template parameter _Refer must to be a reference type." );
        static_assert( ::Scl::is_instance< Instance >, "The template parameter _Refer must to be a Instance type reference!" );
        //static_assert( ::Scl::is_similar< ValueRefer, InstanceRefer >, "The Refer and ValueRefer must to be similar types!" );
        static_assert( ::Scl::is_similar< HolderRefer, InstanceRefer >, "The Refer and HolderRefer must to be similar types!" );

    private:
        ReferPointer m_refer_pointer;

    private:
        SpecialInstanceGuard ( const ThisType & other ) = delete;

    public:
        SpecialInstanceGuard ( InstanceRefer refer )
            : m_refer_pointer( ::std::forward< InstanceRefer >( refer ) )
        {
            static_assert( ::SclPrivate::HolderInternal::is_value_method_exists< Holder, HolderRefer >
                , "There are no appropriate access methods for Holder." );
            ::SclPrivate::HolderInternal::guard< HolderRefer >( ::SclPrivate::instanceHolder< InstanceRefer >( *m_refer_pointer ) );
        }

        SpecialInstanceGuard ( ThisType && other )// = delete;
            : m_refer_pointer( ::std::forward< ReferPointer >( other.m_refer_pointer ) )
        {
        }

        ~SpecialInstanceGuard ()
        {
            if ( !!m_refer_pointer )
                ::SclPrivate::HolderInternal::unguard< HolderRefer >( ::SclPrivate::instanceHolder< InstanceRefer >( *m_refer_pointer ) );
        }

        InstanceAccess instanceAccess () const
        {
            assert( m_refer_pointer );
            return ::SclPrivate::HolderInternal::value< HolderRefer >( ::SclPrivate::instanceHolder< InstanceRefer >( *m_refer_pointer ) );
        }

        HolderAccess holderAccess () const
        {
            assert( m_refer_pointer );
            return ::SclPrivate::instanceHolder< InstanceRefer >( *m_refer_pointer );
        }
    };
}

namespace SclPrivate
{
    struct ValueCase;
    struct InstanceCase;

    template < typename _Refer >
    struct InstanceGuardCaseHelper
    {
        using Type = ::std::conditional_t< ::Scl::is_instance< ::std::decay_t< _Refer > >,
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
        using Type = ::SclPrivate::DefaultInstanceGuard< _Refer >;
    };

    template < typename _Refer >
    struct InstanceSwith< InstanceCase, _Refer >
    {
        using Type = ::SclPrivate::SpecialInstanceGuard< _Refer >;
    };

    template < typename _Refer >
    struct InstanceGuardHelper
    {
        static_assert( ::std::is_reference< _Refer >::value, "The template parameter _Refer must to be a reference type." );
        using Type = typename InstanceSwith< InstanceGuardSwitchCase< _Refer >, _Refer >::Type;
    };
}

#endif

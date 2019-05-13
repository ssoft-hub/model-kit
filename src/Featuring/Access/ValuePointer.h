#pragma once
#ifndef SCL_FEATURING_ACCESS_VALUE_POINTER_H
#define SCL_FEATURING_ACCESS_VALUE_POINTER_H

#include <ModelKit/Featuring/Access/ValueGuard.h>

namespace ScL
{
    /*!
     * Указатель на экземпляр вложенного в Instance базового значения, к которому применены
     * все особенности, реализуемые посредством используемых Instance.
     */
    template < typename _InstanceRefer >
    class ValuePointer
    {
        template < typename, typename > friend class ::SclPrivate::Instance;
        using ThisType = ValuePointer< _InstanceRefer >;

    public:
        using InstanceRefer = _InstanceRefer;
        using ValueGuard = ::ScL::ValueGuard< InstanceRefer >;
        using PointerAccess = typename ValueGuard::PointerAccess;
        using ValueAccess = typename ValueGuard::ValueAccess;

    private:
        ValueGuard m_value_guard;

    private:
        ValuePointer ( InstanceRefer refer )
            : m_value_guard( ::std::forward< InstanceRefer >( refer ) )
        {
        }

    public:
        ValuePointer ( ThisType && other )
            : m_value_guard( ::std::forward< ThisType && >( other ).m_value_guard )
        {}

        ValuePointer ( const ThisType & ) = delete;
        void operator = ( const ThisType & ) = delete;

        ValueAccess operator * () const
        {
            return m_value_guard.valueAccess();;
        }

        PointerAccess operator -> () const
        {
            return m_value_guard.pointerAccess();
        }
    };
}

#endif

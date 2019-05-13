#pragma once
#ifndef SCL_META_VALUE_OF_H

namespace ScL
{
    namespace Meta
    {
        namespace Detail
        {
            template < typename _Type >
            struct ValueOf;
                //!< This struct must be specialized.
        }

        template < typename _Type >
        auto constexpr value_of = Detail::ValueOf< _Type >::value;
    }
}

#endif

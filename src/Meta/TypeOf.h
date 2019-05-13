#pragma once
#ifndef SCL_META_TYPE_OF_H

namespace ScL
{
    namespace Meta
    {
        namespace Detail
        {
            template < typename _Type >
            struct TypeOf;
                //!< This struct must be specialized.
        }

        template < typename _Type >
        using TypeOf = typename Detail::TypeOf< _Type >::Type;
    }
}

#endif

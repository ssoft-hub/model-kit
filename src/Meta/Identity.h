#pragma once
#ifndef SCL_META_IDENTITY_H

#include "Sequence.h"

namespace ScL
{
    namespace Meta
    {
        template < typename _Type >
        using Identity = Sequence< _Type >;
            //!< Define type 'type' as _Type (collection for only one type).
    }
}

#endif

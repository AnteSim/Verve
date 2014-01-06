//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TYPERANGE_H_
#define _VT_TYPERANGE_H_

#ifndef _TORQUE_TYPES_H_
#include "platform/types.h"
#endif

class Range
{
public:

    Range( void ) :
        Min( 0.f ),
        Max( 1.f ),
        Delta( 1.f )
    {
        // Void.
    };

    Range( F32 pMin, F32 pMax ) :
        Min( pMin ),
        Max( pMax ),
        Delta( pMax - pMin )
    {
        // Void.
    };

    F32         Min;
    F32         Max;
    F32         Delta;
};

#endif // _VT_TYPERANGE_H_
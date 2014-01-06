//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSHAREDENUM_H_
#define _VT_VSHAREDENUM_H_

#ifndef _DYNAMIC_CONSOLETYPES_H_
#include "console/dynamicTypes.h"
#endif

namespace VSharedEnum
{
    enum eActionToggle
    {
        k_ActionTurnOn,
        k_ActionTurnOff,
    };
};

//-----------------------------------------------------------------------------

// Define Types.
typedef VSharedEnum::eActionToggle VActionToggle;

// Declare Enum Types.
DefineEnumType( VActionToggle );

//-----------------------------------------------------------------------------

#endif // _VT_VSHAREDENUM_H_
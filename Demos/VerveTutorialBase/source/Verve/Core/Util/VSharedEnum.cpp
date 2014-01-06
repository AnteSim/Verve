//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VSharedEnum.h"

//-----------------------------------------------------------------------------

// Implement the Action enum list.
ImplementEnumType( VActionToggle, "" )
    { VSharedEnum::k_ActionTurnOn,  "ON"  },
    { VSharedEnum::k_ActionTurnOff, "OFF" },
EndImplementEnumType;
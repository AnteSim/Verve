//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSPAWNSPHEREGROUP_H_
#define _VT_VSPAWNSPHEREGROUP_H_

#ifndef _VT_VSCENEOBJECTGROUP_H_
#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#endif

//-----------------------------------------------------------------------------

class VSpawnSphereGroup : public VSceneObjectGroup
{
    typedef VSceneObjectGroup Parent;

public:

    VSpawnSphereGroup( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VSpawnSphereGroup );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSPAWNSPHEREGROUP_H_
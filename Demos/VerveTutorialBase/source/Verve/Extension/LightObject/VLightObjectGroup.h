//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VLIGHTOBJECTGROUP_H_
#define _VT_VLIGHTOBJECTGROUP_H_

#ifndef _VT_VSCENEOBJECTGROUP_H_
#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#endif

//-----------------------------------------------------------------------------

class VLightObjectGroup : public VSceneObjectGroup
{
    typedef VSceneObjectGroup Parent;

public:

    VLightObjectGroup( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VLightObjectGroup );
};

//-----------------------------------------------------------------------------

#endif // _VT_VLIGHTOBJECTGROUP_H_
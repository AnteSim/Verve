//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSPAWNSPHERESPAWNTARGETEVENT_H_
#define _VT_VSPAWNSPHERESPAWNTARGETEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

#ifndef _VT_TORQUE_SPAWNSPHERE_H_
#include "Verve/Torque/TSpawnSphere.h"
#endif

//-----------------------------------------------------------------------------

class VSpawnSphereSpawnTargetEvent : public VSceneObjectEvent
{
    typedef VEvent Parent;
public:

    VSpawnSphereSpawnTargetEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VSpawnSphereSpawnTargetEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSPAWNSPHERESPAWNTARGETEVENT_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSPAWNSPHERESPAWNTARGETTRACK_H_
#define _VT_VSPAWNSPHERESPAWNTARGETTRACK_H_

#ifndef _VT_VSCENEOBJECTTRACK_H_
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"
#endif

#ifndef _VT_TORQUE_LIGHTOBJECT_H_
#include "Verve/Torque/TLightObject.h"
#endif

//-----------------------------------------------------------------------------

class VSpawnSphereSpawnTargetTrack : public VSceneObjectTrack
{
    typedef VSceneObjectTrack Parent;

protected:

    SimSet mSpawnList;

    bool mDespawnOnStop;
    bool mDespawnOnLoop;

public:

    VSpawnSphereSpawnTargetTrack( void );

    static void initPersistFields( void );

    // Controller Methods.

    virtual bool      onControllerEvent( VController::eControllerEventType pEvent );

    // Spawn Methods.

    virtual void      spawnTarget( void );
    virtual void      despawnTargets( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VSpawnSphereSpawnTargetTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSPAWNSPHERESPAWNTARGETTRACK_H_
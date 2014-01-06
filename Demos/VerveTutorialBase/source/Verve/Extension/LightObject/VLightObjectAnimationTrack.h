//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VLIGHTOBJECTANIMATIONTRACK_H_
#define _VT_VLIGHTOBJECTANIMATIONTRACK_H_

#ifndef _VT_VSCENEOBJECTTRACK_H_
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"
#endif

#ifndef _VT_TORQUE_LIGHTOBJECT_H_
#include "Verve/Torque/TLightObject.h"
#endif

//-----------------------------------------------------------------------------

class VLightObjectAnimationTrack : public VSceneObjectTrack
{
    typedef VSceneObjectTrack Parent;

public:

    VLightObjectAnimationTrack( void );

    // Controller Methods.

    virtual bool    onControllerEvent( VController::eControllerEventType pEvent );
    virtual void    onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VLightObjectAnimationTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VLIGHTOBJECTANIMATIONTRACK_H_
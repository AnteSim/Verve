//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VMOTIONEVENT_H_
#define _VT_VMOTIONEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

#ifndef _VT_TORQUE_MOTION_H_
#include "Verve/Torque/TMotion.h"
#endif

//-----------------------------------------------------------------------------

class VMotionEvent : public VSceneObjectEvent
{
    typedef VSceneObjectEvent Parent;

public:

    VMotionEvent( void );

    // Event Methods.

    virtual void                       onTrigger( const S32 &pTime, const S32 &pDelta );

    // Reference Methods.

    virtual VTorque::PathObjectType   *getPath( void );
    F32                                getObjectSpeed( void );
    S32                                getNodeIndex( const S32 &pDelta = 0 );

    // Console Declaration.

    DECLARE_CONOBJECT( VMotionEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VMOTIONEVENT_H_
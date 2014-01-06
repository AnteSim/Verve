//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VLIGHTOBJECTANIMATIONEVENT_H_
#define _VT_VLIGHTOBJECTANIMATIONEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

#ifndef _VT_TORQUE_LIGHTOBJECT_H_
#include "Verve/Torque/TLightObject.h"
#endif

//-----------------------------------------------------------------------------

class VLightObjectAnimationEvent : public VSceneObjectEvent
{
    typedef VEvent Parent;

public:

    SimObjectPtr<VTorque::LightAnimationDataType>    mAnimationData;

public:

    VLightObjectAnimationEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );
    virtual void    onComplete( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VLightObjectAnimationEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VLIGHTOBJECTANIMATIONEVENT_H_

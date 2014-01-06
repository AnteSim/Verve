//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSHAPEANIMATIONEVENT_H_
#define _VT_VSHAPEANIMATIONEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

#ifndef _VT_TORQUE_ANIMATION_H_
#include "Verve/Torque/TAnimation.h"
#endif

//-----------------------------------------------------------------------------

class VShapeAnimationEvent : public VSceneObjectEvent
{
    typedef VSceneObjectEvent Parent;

public:

    bool            mAutoDuration;
    String          mAnimationData;
    
public:

    VShapeAnimationEvent( void );

    static void     initPersistFields( void );

    // Callback Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );
    virtual void    onComplete( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VShapeAnimationEvent );

public:

    // Property Methods.

    F32             getAnimationPosition( const S32 &pTime );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSHAPEANIMATIONEVENT_H_
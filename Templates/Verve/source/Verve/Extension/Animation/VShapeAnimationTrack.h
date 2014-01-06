//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSHAPEANIMATIONTRACK_H_
#define _VT_VSHAPEANIMATIONTRACK_H_

#ifndef _VT_VSCENEOBJECTTRACK_H_
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"
#endif

#ifndef _VT_TORQUE_ANIMATION_H_
#include "Verve/Torque/TAnimation.h"
#endif

//-----------------------------------------------------------------------------

class VShapeAnimationTrack : public VSceneObjectTrack
{
    typedef VSceneObjectTrack Parent;

    U32             mThreadIndex;

public:

    VShapeAnimationTrack( void );

    static void     initPersistFields( void );

    // Controller Methods.

    virtual bool    onControllerEvent( VController::eControllerEventType pEvent );
    virtual void    onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VShapeAnimationTrack );

public:

    inline U32     &getThreadIndex( void ) { return mThreadIndex; };
};

//-----------------------------------------------------------------------------

#endif // _VT_VSHAPEANIMATIONTRACK_H_
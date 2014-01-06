//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TORQUE_ANIMATION_H_
#define _VT_TORQUE_ANIMATION_H_

#ifndef _VT_TORQUE_SCENEOBJECT_H_
#include "Verve/Torque/TSceneObject.h"
#endif

namespace VTorque
{
    bool                isAnimationLooping( SceneObjectType *pObject, const char *pData );

    String              getAnimation( SceneObjectType *pObject, const U32 &pThreadIndex );
    F32                 getAnimationDuration( SceneObjectType *pObject, const char *pData );
    void                setAnimationPosition( SceneObjectType *pObject, const U32 &pThreadIndex, const F32 &pPosition );
    void                setAnimationTimeScale( SceneObjectType *pObject, const U32 &pThreadIndex, const F32 &pTimeScale );

    void                playAnimation( SceneObjectType *pObject, const U32 &pThreadIndex, const char *pData );
    void                playAnimation( SceneObjectType *pObject, const U32 &pThreadIndex );
    void                stopAnimation( SceneObjectType *pObject, const U32 &pThreadIndex );
    void                pauseAnimation( SceneObjectType *pObject, const U32 &pThreadIndex );
};

#endif // _VT_TORQUE_ANIMATION_H_
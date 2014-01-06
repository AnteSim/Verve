//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TORQUE_CAMERA_H_
#define _VT_TORQUE_CAMERA_H_

#ifndef _VT_TORQUE_SCENEOBJECT_H_
#include "Verve/Torque/TSceneObject.h"
#endif

#ifndef _CAMERAFXMGR_H_
#include "T3D/fx/cameraFXMgr.h"
#endif

namespace VTorque
{
    bool            isCamera( SceneObjectType *pObject );
    void            setCamera( SceneObjectType *pObject );

    void            startCameraShake( const F32 &pDuration, const F32 &pFalloff, const VectorF &pAmplitude, const VectorF &pFrequency );
    void            stopCameraShake( void );
};

#endif // _VT_TORQUE_CAMERA_H_
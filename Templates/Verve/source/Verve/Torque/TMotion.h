//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TORQUE_MOTION_H_
#define _VT_TORQUE_MOTION_H_

#ifndef _VT_TORQUE_SCENEOBJECT_H_
#include "Verve/Torque/TSceneObject.h"
#endif

#ifndef _VT_VPATH_H_
#include "Verve/VPath/VPath.h"
#endif

namespace VTorque
{
    typedef VPath   PathObjectType;

    bool            isMovable( SimObject *pObject );
    bool            isPath( SimObject *pObject );
    bool            isPathObjectAttached( PathObjectType *pPath, SceneObjectType *pObject );

    F32             getPathNodeLength( PathObjectType *pPath, const S32 &pNode );

    void            attachPathObject( PathObjectType *pPath, SceneObjectType *pObject, const bool &pForward, const bool &pRelative, const S32 &pStartNodeIndex, const S32 &pEndNodeIndex, const String &pOrientation, const String &pOrientationData );
    void            detachPathObject( PathObjectType *pPath, SceneObjectType *pObject );

    void            setPathObjectActive( PathObjectType *pPath, SceneObjectType *pObject, const bool &pActive );
    void            setPathObjectInterp( PathObjectType *pPath, SceneObjectType *pObject, const F32 &pInterp );
    void            setPathObjectOffset( PathObjectType *pPath, SceneObjectType *pObject, const Point3F &pOffset );
    void            setPathObjectSpeed( PathObjectType *pPath, SceneObjectType *pObject, const F32 &pSpeed );
    void            setPathObjectOrientation( PathObjectType *pPath, SceneObjectType *pObject, const String &pOrientation, const String &pOrientationData = String::EmptyString );
    void            setPathObjectForward( PathObjectType *pPath, SceneObjectType *pObject, const bool &pForward );
    void            setPathObjectNode( PathObjectType *pPath, SceneObjectType *pObject, const S32 &pNode );
    void            setPathObjectEndNode( PathObjectType *pPath, SceneObjectType *pObject, const S32 &pNode );
};

#endif // _VT_TORQUE_MOTION_H_
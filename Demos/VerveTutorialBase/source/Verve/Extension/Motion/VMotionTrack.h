//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VMOTIONTRACK_H_
#define _VT_VMOTIONTRACK_H_

#ifndef _VT_VSCENEOBJECTTRACK_H_
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"
#endif

#ifndef _VT_TORQUE_MOTION_H_
#include "Verve/Torque/TMotion.h"
#endif

//-----------------------------------------------------------------------------

class VMotionTrack : public VSceneObjectTrack
{
    typedef VSceneObjectTrack Parent;

public:

    // Reference Members.

    String          mDataReference;

    // Path Members.

    String         mOrientationMode;
    String         mOrientationData;
    bool           mRelative;

public:

    VMotionTrack( void );

    static void                        initPersistFields( void );

    // Controller Methods.

    virtual bool                       onControllerEvent( VController::eControllerEventType pEvent );
    virtual void                       onControllerReset( const S32 &pTime, const bool &pForward );

    // Reference Methods.

    VTorque::PathObjectType           *getPath( void );
    void                               attachObject( void );
    void                               detachObject( void );

    void                               resetObject( const S32 &pTime );

    // Console Declaration.

    DECLARE_CONOBJECT( VMotionTrack );

protected:

    // Static Field Methods.

    static bool                        setOrientationMode( void *pObject, const char *pArray, const char *pData );
    static bool                        setOrientationData( void *pObject, const char *pArray, const char *pData );
};

//-----------------------------------------------------------------------------

#endif // _VT_VMOTIONTRACK_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VCAMERAGROUP_H_
#define _VT_VCAMERAGROUP_H_

#ifndef _VT_VSCENEOBJECTGROUP_H_
#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#endif

#ifndef _VT_VCONTROLLER_H
#include "Verve/Core/VController.h"
#endif

//-----------------------------------------------------------------------------

class VCameraGroup;

//-----------------------------------------------------------------------------

class VCameraGroup : public VSceneObjectGroup
{
    typedef VSceneObjectGroup Parent;

public:

    enum eCameraEventType
    {
        k_EventActivate,
        k_EventDeactivate,
    };

    typedef Signal<bool( const eCameraEventType & )> CameraEventSignal;
    typedef Signal<void( VCameraGroup* )>            CameraChangeSignal;

protected:

    static VCameraGroup         *mActiveGroup;
    static CameraChangeSignal    mCameraChangeSignal;

    CameraEventSignal            mCameraEventSignal;

public:

    VCameraGroup( void );

    // Tree Methods.

    void                   onAttach( void );
    void                   onDetach( void );

    // Controller Methods.

    bool                   onControllerEvent( VController::eControllerEventType pEvent );

    // Camera Methods.

    inline bool            isActive( void ) { return ( bool )( this == getActiveGroup() ); };
    inline VCameraGroup   *getActiveGroup( void ) { return mActiveGroup; };

    void                   setActive( void );

    static void            clearActiveGroup( void );
    static void            setActiveGroup( VCameraGroup *pCameraGroup );

    // Signal Methods.

    static inline CameraChangeSignal   &getCameraChangeSignal( void ) { return mCameraChangeSignal; };
    inline CameraEventSignal           &getCameraEventSignal( void ) { return mCameraEventSignal; };

    // Console Declaration.

    DECLARE_CONOBJECT( VCameraGroup );
};

//-----------------------------------------------------------------------------

#endif // _VT_VCAMERAGROUP_H_
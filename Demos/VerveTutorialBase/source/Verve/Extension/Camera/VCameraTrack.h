//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VCAMERATRACK_H_
#define _VT_VCAMERATRACK_H_

#ifndef _VT_VSCENEOBJECTTRACK_H_
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"
#endif

#ifndef _VT_VCAMERAGROUP_H_
#include "Verve/Extension/Camera/VCameraGroup.h"
#endif

//-----------------------------------------------------------------------------

class VCameraTrack : public VSceneObjectTrack
{
    typedef VSceneObjectTrack Parent;

public:

    VCameraTrack( void );

    // Tree Methods.

    void            onAttach( void );
    void            onDetach( void );

    // Camera Methods.

    virtual bool    onCameraEvent( const VCameraGroup::eCameraEventType &pEvent );

    // Console Declaration.

    DECLARE_CONOBJECT( VCameraTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VCAMERATRACK_H_
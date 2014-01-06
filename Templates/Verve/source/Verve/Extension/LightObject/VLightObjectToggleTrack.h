//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VLIGHTOBJECTTOGGLETRACK_H_
#define _VT_VLIGHTOBJECTTOGGLETRACK_H_

#ifndef _VT_VSCENEOBJECTTRACK_H_
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"
#endif

#ifndef _VT_TORQUE_LIGHTOBJECT_H_
#include "Verve/Torque/TLightObject.h"
#endif

//-----------------------------------------------------------------------------

class VLightObjectToggleTrack : public VSceneObjectTrack
{
    typedef VSceneObjectTrack Parent;

public:

    VLightObjectToggleTrack( void );

    // Controller Methods.

    virtual void      onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VLightObjectToggleTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VLIGHTOBJECTTOGGLETRACK_H_
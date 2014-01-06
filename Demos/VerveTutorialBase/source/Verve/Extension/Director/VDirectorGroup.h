//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VDIRECTORGROUP_H_
#define _VT_VDIRECTORGROUP_H_

#ifndef _VT_VGROUP_H_
#include "Verve/Core/VGroup.h"
#endif

//-----------------------------------------------------------------------------

class VDirectorTrack;
class VCameraGroup;

//-----------------------------------------------------------------------------

class VDirectorGroup : public VGroup
{
    typedef VGroup Parent;

protected:

    // Camera.
    VCameraGroup             *mActiveCamera;

public:

    VDirectorGroup( void );

    VDirectorTrack        *getDirectorTrack( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VDirectorGroup );
};

//-----------------------------------------------------------------------------

#endif // _VT_VDIRECTORGROUP_H_
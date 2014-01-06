//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VCAMERASHAKETRACK_H_
#define _VT_VCAMERASHAKETRACK_H_

#ifndef _VT_VCAMERATRACK_H_
#include "Verve/Extension/Camera/VCameraTrack.h"
#endif

//-----------------------------------------------------------------------------

class VCameraShakeTrack : public VCameraTrack
{
    typedef VCameraTrack Parent;

public:

    VCameraShakeTrack( void );

    // Camera Methods.

    bool            onCameraEvent( const VCameraGroup::eCameraEventType &pEvent );

    // Controller Methods.

    void            onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VCameraShakeTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VCAMERASHAKETRACK_H_
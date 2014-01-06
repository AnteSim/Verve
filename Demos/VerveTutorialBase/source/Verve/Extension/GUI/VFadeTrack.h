//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VFADETRACK_H_
#define _VT_VFADETRACK_H_

#ifndef _VT_VTRACK_H_
#include "Verve/Core/VTrack.h"
#endif

//-----------------------------------------------------------------------------

class VFadeTrack : public VTrack
{
    typedef VTrack Parent;

public:

    VFadeTrack( void );

    // Controller Methods.

    virtual bool    onControllerEvent( VController::eControllerEventType pEvent );
    virtual void    onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VFadeTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VFADETRACK_H_
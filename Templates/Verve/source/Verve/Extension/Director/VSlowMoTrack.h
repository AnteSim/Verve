//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSLOWMOTRACK_H_
#define _VT_VSLOWMOTRACK_H_

#ifndef _VT_VTRACK_H_
#include "Verve/Core/VTrack.h"
#endif

//-----------------------------------------------------------------------------

class VSlowMoTrack : public VTrack
{
    typedef VTrack Parent;

public:

    VSlowMoTrack( void );

    // Controller Methods.

    bool            onControllerEvent( VController::eControllerEventType pEvent );
    void            onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VSlowMoTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSLOWMOTRACK_H_
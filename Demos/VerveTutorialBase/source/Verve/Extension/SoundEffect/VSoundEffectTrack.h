//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSOUNDEFFECTTRACK_H_
#define _VT_VSOUNDEFFECTTRACK_H_

#ifndef _VT_VTRACK_H_
#include "Verve/Core/VTrack.h"
#endif

#ifndef _VT_TORQUE_SOUNDEFFECT_H_
#include "Verve/Torque/TSoundEffect.h"
#endif

//-----------------------------------------------------------------------------

class VSoundEffectTrack : public VTrack
{
    typedef VTrack Parent;

public:

    VTorque::SoundSourceType   *mSource;

public:

    VSoundEffectTrack( void );

    // Controller Methods.

    virtual bool    onControllerEvent( VController::eControllerEventType pEvent );
    virtual void    onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VSoundEffectTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSOUNDEFFECTTRACK_H_
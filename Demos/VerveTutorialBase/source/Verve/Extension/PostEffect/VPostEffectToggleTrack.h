//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VPOSTEFFECTTOGGLETRACK_H_
#define _VT_VPOSTEFFECTTOGGLETRACK_H_

#ifndef _VT_VCAMERATRACK_H_
#include "Verve/Extension/Camera/VCameraTrack.h"
#endif

#ifndef _VT_TORQUE_POSTEFFECT_H_
#include "Verve/Torque/TPostEffect.h"
#endif

//-----------------------------------------------------------------------------

class VPostEffectToggleTrack : public VCameraTrack
{
    typedef VCameraTrack Parent;

protected:

    SimObjectPtr<VTorque::PostEffectType>   mPostEffect;

public:

    VPostEffectToggleTrack( void );

    static  void               initPersistFields( void );

    // Camera Methods.

    bool            onCameraEvent( const VCameraGroup::eCameraEventType &pEvent );

    // Console Declaration.

    DECLARE_CONOBJECT( VPostEffectToggleTrack );

public:

    VTorque::PostEffectType   *getPostEffect( void ) { return mPostEffect; };
};

//-----------------------------------------------------------------------------

#endif // _VT_VPOSTEFFECTTOGGLETRACK_H_
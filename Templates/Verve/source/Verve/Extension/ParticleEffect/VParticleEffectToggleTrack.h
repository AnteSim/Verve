//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VPARTICLEEFFECTTOGGLETRACK_H_
#define _VT_VPARTICLEEFFECTTOGGLETRACK_H_

#ifndef _VT_VSCENEOBJECTTRACK_H_
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"
#endif

#ifndef _VT_TORQUE_PARTICLEEFFECT_H_
#include "Verve/Torque/TParticleEffect.h"
#endif

//-----------------------------------------------------------------------------

class VParticleEffectToggleTrack : public VSceneObjectTrack
{
    typedef VSceneObjectTrack Parent;

public:

    VParticleEffectToggleTrack( void );

    // Controller Methods.

    virtual void      onControllerReset( const S32 &pTime, const bool &pForward );

    // Console Declaration.

    DECLARE_CONOBJECT( VParticleEffectToggleTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VPARTICLEEFFECTTOGGLETRACK_H_
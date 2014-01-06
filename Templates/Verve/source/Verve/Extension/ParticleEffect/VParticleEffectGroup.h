//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VPARTICLEEFFECTGROUP_H_
#define _VT_VPARTICLEEFFECTGROUP_H_

#ifndef _VT_VSCENEOBJECTGROUP_H_
#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#endif

//-----------------------------------------------------------------------------

class VParticleEffectGroup : public VSceneObjectGroup
{
    typedef VSceneObjectGroup Parent;

public:

    VParticleEffectGroup( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VParticleEffectGroup );
};

//-----------------------------------------------------------------------------

#endif // _VT_VPARTICLEEFFECTGROUP_H_
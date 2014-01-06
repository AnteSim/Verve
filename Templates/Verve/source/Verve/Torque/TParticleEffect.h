//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TORQUE_PARTICLEEFFECT_H_
#define _VT_TORQUE_PARTICLEEFFECT_H_

#ifndef _H_PARTICLE_EMITTER
#include "T3D/fx/particleEmitter.h"
#endif

#ifndef _PARTICLEEMITTERDUMMY_H_
#include "T3D/fx/particleEmitterNode.h"
#endif

namespace VTorque
{
    
    typedef ParticleEmitterNode    ParticleEffectType;

    bool                           isParticleEffectEnabled( ParticleEffectType *pParticleEffect );
    void                           setParticleEffectOn( ParticleEffectType *pParticleEffect, const bool &pStatus );
};

#endif // _VT_TORQUE_PARTICLEEFFECT_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Torque/TParticleEffect.h"

//-----------------------------------------------------------------------------
//
// Particle Effect Methods.
//
//-----------------------------------------------------------------------------

bool VTorque::isParticleEffectEnabled( ParticleEffectType *pParticleEffect )
{
    if ( !pParticleEffect )
    {
        // Sanity!
        return false;
    }

    // Get Active.
    return pParticleEffect->getActive();
}

void VTorque::setParticleEffectOn( ParticleEffectType *pParticleEffect, const bool &pStatus )
{
    if ( !pParticleEffect )
    {
        // Sanity!
        return;
    }

    // Set Active.
    pParticleEffect->setActive( pStatus );
}
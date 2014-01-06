//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/ParticleEffect/VParticleEffectToggleTrack.h"
#include "Verve/Extension/ParticleEffect/VParticleEffectToggleEvent.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VParticleEffectToggleTrack );
//-----------------------------------------------------------------------------

VParticleEffectToggleTrack::VParticleEffectToggleTrack( void )
{
    setLabel( "ToggleTrack" );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VParticleEffectToggleTrack::onControllerReset( pTime, pForward );
// 
// Enable or Disable the particle effect after a reset.
// 
//-----------------------------------------------------------------------------
void VParticleEffectToggleTrack::onControllerReset( const S32 &pTime, const bool &pForward )
{
    // Default Reset.
    Parent::onControllerReset( pTime, pForward );

    VParticleEffectToggleEvent  *event;
    VTorque::ParticleEffectType *particleEffect;
    if ( getSceneObject( particleEffect ) && getPreviousEvent( event ) )
    {
        // Turn On?
        const bool turnOn = ( event->mEventType == VSharedEnum::k_ActionTurnOn );

        // Toggle the Particle Effect.
        VTorque::setParticleEffectOn( particleEffect, turnOn );
    }
}
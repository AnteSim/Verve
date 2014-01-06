//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/ParticleEffect/VParticleEffectToggleEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VParticleEffectToggleEvent );
//-----------------------------------------------------------------------------

VParticleEffectToggleEvent::VParticleEffectToggleEvent( void ) : 
        mEventType( VSharedEnum::k_ActionTurnOn )
{
    setLabel( "ToggleEvent" );
}

void VParticleEffectToggleEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "Action", TYPEID<VActionToggle>(), Offset( mEventType, VParticleEffectToggleEvent ) );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VParticleEffectToggleEvent::onTrigger( pTime, pDelta );
// 
// Toggle the Particle Effect.
// 
//-----------------------------------------------------------------------------
void VParticleEffectToggleEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    VTorque::ParticleEffectType *particleEffect;
    if ( getSceneObject( particleEffect ) )
    {
        // Turn On?
        const bool turnOn = ( mEventType == VSharedEnum::k_ActionTurnOn );

        // Toggle Particle Effect.
        VTorque::setParticleEffectOn( particleEffect, turnOn );
    }
}
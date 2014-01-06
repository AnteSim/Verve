//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/PostEffect/VPostEffectToggleEvent.h"
#include "Verve/Extension/PostEffect/VPostEffectToggleTrack.h"
#include "Verve/Extension/Camera/VCameraGroup.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VPostEffectToggleEvent );
//-----------------------------------------------------------------------------

VPostEffectToggleEvent::VPostEffectToggleEvent( void ) : 
        mEventType( VSharedEnum::k_ActionTurnOn )
{
    setLabel( "ToggleEvent" );
}

void VPostEffectToggleEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "Action", TYPEID<VActionToggle>(), Offset( mEventType, VPostEffectToggleEvent ) );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VPostEffectToggleEvent::onTrigger( pTime, pDelta );
// 
// Only enable this effect if the parent group is currently active. 
// 
//-----------------------------------------------------------------------------
void VPostEffectToggleEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch Parent Objects.
    VCameraGroup           *group;
    VPostEffectToggleTrack *track;
    if ( ( !getGroup( group ) || !group->isActive() ) || !getTrack( track ) )
    {
        // Quit.
        return;
    }

    // Turn On?
    const bool turnOn = ( mEventType == VSharedEnum::k_ActionTurnOn );

    // Enable Effect.
    VTorque::setPostEffectOn( track->getPostEffect(), turnOn );
}
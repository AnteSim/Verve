//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/LightObject/VLightObjectToggleTrack.h"
#include "Verve/Extension/LightObject/VLightObjectToggleEvent.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VLightObjectToggleTrack );
//-----------------------------------------------------------------------------

VLightObjectToggleTrack::VLightObjectToggleTrack( void )
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
// VLightObjectToggleTrack::onControllerReset( pTime, pForward );
// 
// Enable or Disable the light object after a reset.
// 
//-----------------------------------------------------------------------------
void VLightObjectToggleTrack::onControllerReset( const S32 &pTime, const bool &pForward )
{
    // Default Reset.
    Parent::onControllerReset( pTime, pForward );

    VLightObjectToggleEvent  *event;
    VTorque::LightObjectType *lightObject;
    if ( getSceneObject( lightObject ) && getPreviousEvent( event ) )
    {
        // Turn On?
        const bool turnOn = ( event->mEventType == VSharedEnum::k_ActionTurnOn );

        // Toggle the Light.
        VTorque::setLightObjectOn( lightObject, turnOn );
    }
}
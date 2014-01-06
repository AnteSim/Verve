//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/LightObject/VLightObjectAnimationTrack.h"
#include "Verve/Extension/LightObject/VLightObjectAnimationEvent.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VLightObjectAnimationTrack );
//-----------------------------------------------------------------------------

VLightObjectAnimationTrack::VLightObjectAnimationTrack( void )
{
    setLabel( "AnimationTrack" );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VLightObjectAnimationTrack::onControllerEvent( pEvent );
// 
// 
// 
//-----------------------------------------------------------------------------
bool VLightObjectAnimationTrack::onControllerEvent( VController::eControllerEventType pEvent )
{
    if ( !Parent::onControllerEvent( pEvent ) )
    {
        // Skip.
        return false;
    }

    // Enabled?
    if ( !isEnabled() )
    {
        // Continue Processing Events.
        return true;
    }

    // Fetch the Light Object.
    VTorque::LightObjectType *lightObject;
    if ( !getSceneObject( lightObject ) )
    {
        // Skip.
        return true;
    }

    switch ( pEvent )
    {
        case VController::k_EventPlay :
            {

                // Play Animation?
                VLightObjectAnimationEvent *event;
                if ( getCurrentEvent( event ) )
                {
                    // Play.
                    VTorque::playAnimation( lightObject );
                }

            } break;

        case VController::k_EventPause :
        case VController::k_EventStop :
            {

                // Stop the Animation.
                VTorque::pauseAnimation( lightObject );

            } break;
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// VLightObjectAnimationTrack::onControllerReset( pTime, pForward );
// 
// 
// 
//-----------------------------------------------------------------------------
void VLightObjectAnimationTrack::onControllerReset( const S32 &pTime, const bool &pForward )
{
    // Default Reset.
    Parent::onControllerReset( pTime, pForward );

    // Fetch the Light Object.
    VTorque::LightObjectType *lightObject;
    if ( getSceneObject( lightObject ) )
    {
        // Stop the Animation.
        VTorque::pauseAnimation( lightObject );
    }
}
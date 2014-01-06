//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Camera/VCameraShakeTrack.h"
#include "Verve/Extension/Camera/VCameraShakeEvent.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VCameraShakeTrack );
//-----------------------------------------------------------------------------

VCameraShakeTrack::VCameraShakeTrack( void )
{
    setLabel( "CameraShakeTrack" );
}

//-----------------------------------------------------------------------------
//
// Camera Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraShakeTrack::onCameraEvent( pEvent );
// 
// When the Camera changes, this method is called on both the outgoing and
// incoming Camera Groups.
// 
// For a full list of possible events, see the 'eCameraEventType' declaration
// in VCameraGroup.h.
// 
//-----------------------------------------------------------------------------
bool VCameraShakeTrack::onCameraEvent( const VCameraGroup::eCameraEventType &pEvent )
{
    // Parent Call.
    if ( !Parent::onCameraEvent( pEvent ) )
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

    switch( pEvent )
    {
        case VCameraGroup::k_EventActivate :
            {

                VCameraShakeEvent *event;
                if ( getCurrentEvent( event ) )
                {
                    // Re-Trigger Event.
                    event->onTrigger( getControllerTime(), 0 );
                }

            } break;

        case VCameraGroup::k_EventDeactivate :
            {

                // Stop Camera Shake.
                VTorque::stopCameraShake();

            } break;
    }

    return true;
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraShakeTrack::onControllerReset( pTime, pForward );
// 
// Stop all camera shake events.
// 
//-----------------------------------------------------------------------------
void VCameraShakeTrack::onControllerReset( const S32 &pTime, const bool &pForward )
{
    // Default Reset.
    Parent::onControllerReset( pTime, pForward );

    // Stop Camera Shake.
    VTorque::stopCameraShake();
}
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/GUI/VFadeTrack.h"
#include "Verve/Extension/GUI/VFadeEvent.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VFadeTrack );
//-----------------------------------------------------------------------------

VFadeTrack::VFadeTrack( void )
{
    setLabel( "FadeTrack" );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VFadeTrack::onControllerEvent( pEvent );
// 
// When the controller's state changes, this method is called. If the
// controller is paused, or stops playing, then the fade control will cease
// playing. If the controller resumes play, the fade control will continue.
// 
// For a full list of possible events, see the 'eControllerEventType'
// declaration in VController.h.
// 
//-----------------------------------------------------------------------------
bool VFadeTrack::onControllerEvent( VController::eControllerEventType pEvent )
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

    // Fetch the next Event.
    VFadeEvent *event;
    if ( !getNextEvent( event ) )
    {
        // No Event.
        return true;
    }

    // Fetch GUI Control.
    VFadeControl *fadeControl = dynamic_cast<VFadeControl*>( Sim::findObject( "VFadeControlGui" ) );
    if ( !fadeControl )
    {
        // No Control.
        return true;
    }

    switch ( pEvent )
    {
        case VController::k_EventPlay:
            {
                // Play?
                const S32 &time = getControllerTime();
                fadeControl->mActive = ( time > event->getTriggerTime()
                                         && time < event->getFinishTime() ) ;

            } break;

        case VController::k_EventPause :
        case VController::k_EventStop :
            {

                // Pause.
                fadeControl->mActive = false;

            } break;
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// VFadeTrack::onControllerReset( pTime, pForward );
// 
// Reset the fade state of the fade control.
// 
//-----------------------------------------------------------------------------
void VFadeTrack::onControllerReset( const S32 &pTime, const bool &pForward )
{
    Parent::onControllerReset( pTime, pForward );

    // Fetch GUI Control.
    VFadeControl *fadeControl;
    if ( !Sim::findObject( "VFadeControlGUI", fadeControl ) )
    {
        // Invalid.
        return;
    }

    VFadeEvent *event;
    if ( !getNextEvent( event ) )
    {
        // No Events.
        return;
    }

    // Apply Settings.
    fadeControl->mActive      = false;
    fadeControl->mFadeType    = event->getFadeType();
    fadeControl->mDuration    = event->getDuration();
    fadeControl->mElapsedTime = getMax( pTime - event->getTriggerTime(), 0 );
}
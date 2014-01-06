//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/SoundEffect/VSoundEffectTrack.h"
#include "Verve/Extension/SoundEffect/VSoundEffectEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSoundEffectTrack );
//-----------------------------------------------------------------------------

VSoundEffectTrack::VSoundEffectTrack( void ) : 
        mSource( NULL )
{
    setLabel( "SoundTrack" );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSoundEffectTrack::onControllerEvent( pEvent );
// 
// If the controller ceases playback and the track has a valid reference to a
// source provider, then the sound is stopped.
// 
//-----------------------------------------------------------------------------
bool VSoundEffectTrack::onControllerEvent( VController::eControllerEventType pEvent )
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

    switch ( pEvent )
    {
        case VController::k_EventPause :
        case VController::k_EventStop :
            {
#ifdef VT_EDITOR

                if ( mSource )
                {
                    // Stop Sound.
                    VTorque::stopSound( mSource );

                    // Clear Source.
                    mSource = NULL;
                }

#endif
            } break;
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// VSoundEffectTrack::onControllerReset( pTime, pForward );
// 
// If the track is reset and it has a valid reference to a source provider,
// then the sound is stopped.
// 
//-----------------------------------------------------------------------------
void VSoundEffectTrack::onControllerReset( const S32 &pTime, const bool &pForward )
{
    // Default Reset.
    Parent::onControllerReset( pTime, pForward );

    if ( mSource )
    {
        // Stop Sound.
        VTorque::stopSound( mSource );
    }

    // Clear Source.
    mSource = NULL;
}
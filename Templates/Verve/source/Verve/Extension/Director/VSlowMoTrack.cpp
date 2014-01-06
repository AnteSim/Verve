//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Director/VSlowMoTrack.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSlowMoTrack );
//-----------------------------------------------------------------------------

VSlowMoTrack::VSlowMoTrack( void )
{
    setLabel( "SlowMoTrack" );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSlowMoTrack::onControllerEvent( pEvent );
// 
// ...
// 
// For a full list of possible events, see the 'eControllerEventType'
// declaration in VController.h.
// 
//-----------------------------------------------------------------------------
bool VSlowMoTrack::onControllerEvent( VController::eControllerEventType pEvent )
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
        case VController::k_EventStop :
            {

                // Reset Time Scale.
                getController()->setTimeScale( ( isControllerPlayingForward() ) ? 1.f : -1.f );

            } break;
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// VSlowMoTrack::onControllerReset( pTime, pForward );
// 
// ...
// 
//-----------------------------------------------------------------------------
void VSlowMoTrack::onControllerReset( const S32 &pTime, const bool &pForward )
{
    // Parent Reset.
    Parent::onControllerReset( pTime, pForward );

    // Reset Time Scale.
    getController()->setTimeScale( ( pForward ) ? 1.f : -1.f );
}
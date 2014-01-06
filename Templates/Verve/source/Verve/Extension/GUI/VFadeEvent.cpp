//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/GUI/VFadeEvent.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VFadeEvent );
//-----------------------------------------------------------------------------

VFadeEvent::VFadeEvent( void )
{
    setLabel( "FadeEvent" );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VFadeEvent::onTrigger( pTime, pDelta );
// 
// Start the fade sequence if a valid fade control can be found.
// 
//-----------------------------------------------------------------------------
void VFadeEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch GUI Control.
    VFadeControl *fadeControl;
    if ( !Sim::findObject( "VFadeControlGUI", fadeControl ) )
    {
        // Invalid.
        return;
    }

    // Start Fade.
    fadeControl->start( getFadeType(), mDuration );

    // Set Elapsed Time.
    fadeControl->mElapsedTime = mAbs( pTime - getStartTime() );
}

//-----------------------------------------------------------------------------
// 
// VFadeEvent::onComplete( pTime, pDelta );
// 
// Tidy up the fade control once the event has finished.
// 
//-----------------------------------------------------------------------------
void VFadeEvent::onComplete( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch GUI Control.
    VFadeControl *fadeControl;
    if ( !Sim::findObject( "VFadeControlGUI", fadeControl ) )
    {
        // Invalid.
        return;
    }

    // Set Elapsed Time.
    fadeControl->mElapsedTime = mDuration;
}

//-----------------------------------------------------------------------------
//
// Property Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VFadeEvent::getFadeType();
// 
// Returns the type of fade (in or out) that this event will use. Zero and Even
// indices will Fade Out, while Odd numbers will Fade In.
// 
//-----------------------------------------------------------------------------
VFadeControl::eFadeType VFadeEvent::getFadeType( void )
{
    if ( !isControllerPlayingForward() )
    {
        return ( getIndex() % 2 == 0 ) ? VFadeControl::k_TypeOut : VFadeControl::k_TypeIn;
    }

    return ( getIndex() % 2 == 0 ) ? VFadeControl::k_TypeIn : VFadeControl::k_TypeOut;
}
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VController.h"
#include "Verve/Extension/Director/VSlowMoEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSlowMoEvent );
//-----------------------------------------------------------------------------

VSlowMoEvent::VSlowMoEvent( void ) : 
        mTimeScale( 1.f ),
        mTimeScaleTickDelta( 0.f )
{
    setLabel( "SlowMoEvent" );
}

void VSlowMoEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "TimeScale", TypeF32, Offset( mTimeScale, VSlowMoEvent ), "The Time Scale to be applied to the Root Controller." );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSlowMoEvent::onTrigger( pTime, pDelta );
// 
// 
// 
//-----------------------------------------------------------------------------
void VSlowMoEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    VController *controller = getController();
    if ( !controller )
    {
        // Invalid Controller.
        return;
    }

    // Instant Update?
    if ( getDuration() == 0 )
    {
        // Apply & Return.
        controller->setTimeScale( mTimeScale );
        return;
    }

    // Determine the Number of Ticks.
    const F32 tickCount = ( ( F32 )getDuration() ) / TickMs;

    // Determine the Tick Delta.
    mTimeScaleTickDelta = ( mTimeScale - controller->getTimeScale() ) / tickCount;
}

//-----------------------------------------------------------------------------
// 
// VSlowMoEvent::onUpdate( pTime, pDelta );
// 
// 
// 
//-----------------------------------------------------------------------------
void VSlowMoEvent::onUpdate( const S32 &pTime, const S32 &pDelta )
{
    Parent::onUpdate( pTime, pDelta );

    VController *controller = getController();
    if ( !controller )
    {
        // Invalid Controller.
        return;
    }

    // Fetch Current Time Scale.
    const F32 timeScale = controller->getTimeScale();

    // Apply Update.
    controller->setTimeScale( timeScale + mTimeScaleTickDelta );
}

//-----------------------------------------------------------------------------
// 
// VSlowMoEvent::onComplete( pTime, pDelta );
// 
// 
// 
//-----------------------------------------------------------------------------
void VSlowMoEvent::onComplete( const S32 &pTime, const S32 &pDelta )
{
    Parent::onComplete( pTime, pDelta );

    VController *controller = getController();
    if ( !controller )
    {
        // Invalid Controller.
        return;
    }

    // Tidy Up.
    controller->setTimeScale( mTimeScale );
}
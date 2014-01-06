//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Animation/VShapeAnimationEvent.h"
#include "Verve/Extension/Animation/VShapeAnimationTrack.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VShapeAnimationEvent );
//-----------------------------------------------------------------------------

VShapeAnimationEvent::VShapeAnimationEvent( void ) : 
        mAnimationData( String::EmptyString ),
        mAutoDuration( true )
{
    setLabel( "AnimationEvent" );
}

//-----------------------------------------------------------------------------

void VShapeAnimationEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "AnimationData", TypeRealString, Offset( mAnimationData, VShapeAnimationEvent ), "The name of the Animation Sequence to play upon triggering." );
    addField( "AutoDuration",  TypeBool,       Offset( mAutoDuration,  VShapeAnimationEvent ), "Force the Event's Duration to match the length of the Animation." );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VShapeAnimationEvent::onTrigger( pTime, pDelta );
// 
// Play the desired animation. Also account for any offet in playtime, and
// timescale.
// 
//-----------------------------------------------------------------------------
void VShapeAnimationEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    VTorque::SceneObjectType *object = getSceneObject();
    VShapeAnimationTrack *track;
    if ( !object || !getTrack( track ) )
    {
        // Sanity!
        return;
    }

    // Play Animation.
    VTorque::playAnimation( object, track->getThreadIndex(), mAnimationData );

    // Set Position.
    VTorque::setAnimationPosition( object, track->getThreadIndex(), getAnimationPosition( pTime + pDelta ) );

    // Set Time Scale.
    VTorque::setAnimationTimeScale( object, track->getThreadIndex(), ( ( pDelta > 0 ) ? 1.f : -1.f ) );
}

//-----------------------------------------------------------------------------
// 
// VShapeAnimationEvent::onComplete( pTime, pDelta );
// 
// If the animation is cyclic, then it needs to be paused once the event has
// finished playing.
// 
//-----------------------------------------------------------------------------
void VShapeAnimationEvent::onComplete( const S32 &pTime, const S32 &pDelta )
{
    // Fetch Object.
    VTorque::SceneObjectType *object = getSceneObject();
    VShapeAnimationTrack *track;
    if ( object && VTorque::isAnimationLooping( object, mAnimationData ) && getTrack( track ) )
    {
        // Pause Animation.
        VTorque::pauseAnimation( object, track->getThreadIndex() );
    }
}

//-----------------------------------------------------------------------------
//
// Property Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VShapeAnimationEvent::getAnimationPosition( pTime );
// 
// Returns the time that the animation should be positioned at, at the given
// time. This method considers whether the animation is cyclic or not and will
// return the appropriate time regardless. Time is expressed in seconds and not
// milliseconds.
// 
//-----------------------------------------------------------------------------
F32 VShapeAnimationEvent::getAnimationPosition( const S32 &pTime )
{
    // Fetch Object.
    VSceneObjectTrack        *track;
    VTorque::SceneObjectType *object = getSceneObject();
    if ( !getTrack( track ) || !object )
    {
        // Null.
        return 0.f;
    }

    // Fetch Interp.
    F32 interp = track->calculateInterp( pTime );
    if ( !isControllerPlayingForward() )
    {
        // Flip.
        interp = ( 1.f - interp );
    }

    // Not Looping?
    if ( !VTorque::isAnimationLooping( object, mAnimationData ) )
    {
        // Return Interp.
        return interp;
    }

    // Fetch Sequence Duration.
    const S32 duration = ( S32 )( 1000 * VTorque::getAnimationDuration( object, mAnimationData ) );

    // Fetch Loop Interp.
    const S32 loopInterp = S32( mDuration * interp ) % duration;

    return ( F32 )loopInterp / ( F32 )duration;
}
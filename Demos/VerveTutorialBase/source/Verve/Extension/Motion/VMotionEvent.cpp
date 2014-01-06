//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VController.h"
#include "Verve/Core/VGroup.h"
#include "Verve/Extension/Motion/VMotionEvent.h"
#include "Verve/Extension/Motion/VMotionTrack.h"

#include "console/consoleTypes.h"
#include "math/mMathFn.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VMotionEvent );
//-----------------------------------------------------------------------------

VMotionEvent::VMotionEvent( void )
{
    setLabel( "MotionEvent" );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VMotionEvent::onTrigger( pDelta, pDelta );
// 
// The path object is told to move to the next node. If this event corresponds
// to Node 0, the object will move to Node 1. If the object reaches the node
// before the next event is triggered, then the object will stop moving.
// 
// The object's position is only reset when the track is reset and not when an
// event is triggered.
// 
//-----------------------------------------------------------------------------
void VMotionEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch Parent Track.
    VMotionTrack *track;
    if ( !getTrack( track ) )
    {
        // Invalid Track.
        return;
    }

    // Fetch Path & Reference Object.
    VTorque::PathObjectType  *path   = track->getPath();
    VTorque::SceneObjectType *object = getSceneObject();
    if ( !path || !object )
    {
        // Invalid.
        return;
    }

    // Valid Destination Node?
    if ( !isControllerLooping() && !getNextEvent() )
    {
        // Clear Active.
        VTorque::setPathObjectActive( path, object, false );
        // Quit.
        return;
    }

    // Set Active.
    VTorque::setPathObjectActive( path, object, true );

    // Apply Speed.
    VTorque::setPathObjectSpeed( path, object, getObjectSpeed() );
}

//-----------------------------------------------------------------------------
//
// Reference Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// VMotionTrack::getPath();
// 
// Returns the path that this track is referencing.
//
//-----------------------------------------------------------------------------
VTorque::PathObjectType *VMotionEvent::getPath( void )
{
    // Fetch Track.
    VMotionTrack *track;
    if ( !getTrack( track ) )
    {
        // Invalid.
        return NULL;
    }

    // Return Path.
    return track->getPath();
}

//-----------------------------------------------------------------------------
//
// VMotionTrack::getObjectSpeed();
// 
// Determine the Speed that an object must move at to travel over the segment
// length of the Path.
//
//-----------------------------------------------------------------------------
F32 VMotionEvent::getObjectSpeed( void )
{
    // Fetch Parent Track.
    VMotionTrack *track;
    if ( !getTrack( track ) )
    {
        // Invalid Track.
        return 0.f;
    }

    // Fetch Path & Reference Object.
    VTorque::PathObjectType  *path   = track->getPath();
    VTorque::SceneObjectType *object = getSceneObject();
    if ( !path || !object )
    {
        // Invalid Object(s).
        return 0.f;
    }

    // Fetch Node Index.
    const S32 &srcNodeIndex = getNodeIndex( ( isControllerPlayingForward() ) ? 0 : -1 );

    // Fetch the Next Event.
    VEvent *nextEvent = getNextEvent();

    // Valid Destination Node?
    if ( !isControllerLooping() && !nextEvent )
    {
        // No Next Node.
        return 0.f;
    }

    // Valid Next Node?
    if ( nextEvent )
    {
        // Fetch Segment Length & Duration.
        const F32 &length   = VTorque::getPathNodeLength( path, srcNodeIndex );
        const F32 &duration = mAbs( getTriggerTime() - nextEvent->getTriggerTime() );

        // Speed = Distance / Duration.
        return ( length / ( duration / 1000.f ) );
    }

    // Playing Forwards?
    if ( isControllerPlayingForward() )
    {
        // Fetch the First Event.
        VEvent *firstEvent = dynamic_cast<VEvent*>( track->getChild() );

        // Fetch Segment Length & Duration.
        const F32 &length   = VTorque::getPathNodeLength( path, srcNodeIndex );
        const F32 &duration = ( getControllerDuration() - getTriggerTime() ) + firstEvent->getTriggerTime();

        // Speed = Distance / Duration.
        return ( length / ( duration / 1000.f ) );
    }

    // Fetch the Last Event.
    VEvent *lastEvent = dynamic_cast<VEvent*>( track->getLastChild() );

    // Fetch Segment Length & Duration.
    const F32 &length   = VTorque::getPathNodeLength( path, srcNodeIndex );
    const F32 &duration = ( getControllerDuration() - lastEvent->getTriggerTime() ) + getTriggerTime();

    // Speed = Distance / Duration.
    return ( length / ( duration / 1000.f ) );
}

//-----------------------------------------------------------------------------
// 
// VMotionEvent::getNodeIndex( pDelta );
// 
// Returns the index of the path node associated with this event object.
// 
//-----------------------------------------------------------------------------
S32 VMotionEvent::getNodeIndex( const S32 &pDelta )
{
    // Fetch Event Count.
    const S32 eventCount = ( ( VTreeNode* )getParent() )->size();

    // Return Index.
    return ( getIndex() + pDelta ) % eventCount;
}
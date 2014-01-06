//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VController.h"
#include "Verve/Extension/Director/VSceneJumpEvent.h"
#include "Verve/Extension/Director/VDirectorGroup.h"
#include "Verve/Extension/Director/VDirectorTrack.h"
#include "Verve/Extension/Director/VDirectorEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSceneJumpEvent );
//-----------------------------------------------------------------------------

VSceneJumpEvent::VSceneJumpEvent( void ) :
        mTarget( String::EmptyString )
{
    setLabel( "SceneJumpEvent" );
}

void VSceneJumpEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "Target", TypeRealString, Offset( mTarget, VSceneJumpEvent ), "The name of the Scene that the controller will jump to upon triggering." );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSceneJumpEvent::onTrigger( pTime, pDelta );
// 
// Tell the controller to jump to a new scene.
// 
//-----------------------------------------------------------------------------
void VSceneJumpEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    VDirectorTrack *track = getController()->getDirectorTrack();
    if ( !track )
    {
        // Invalid Track.
        return;
    }

    // Get Event.
    VDirectorEvent *event;
    if ( !track->getObject( mTarget, event ) )
    {
        // Can't Jump.
        return;
    }

    // Go To Scene.
    getController()->jump( VController::k_JumpTime, event->getTriggerTime() );
}
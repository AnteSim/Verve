//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VGroup.h"
#include "Verve/Extension/Director/VDirectorEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VDirectorEvent );
//-----------------------------------------------------------------------------

VDirectorEvent::VDirectorEvent( void ) : 
        mTarget( String::EmptyString )
{
    // Void.
}

void VDirectorEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "Target", TypeRealString, Offset( mTarget, VDirectorEvent ), "The name of the CameraGroup that will be activated upon triggering." );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VDirectorEvent::onTrigger( pTime, pDelta );
// 
// Cut the camera to the target group.
// 
//-----------------------------------------------------------------------------
void VDirectorEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch Controller.
    VController *controller = getController();

    // Valid Target?
    VCameraGroup *targetGroup = NULL;
    if ( !controller->getObject( mTarget, targetGroup ) )
    {
        Con::warnf( "VDirectorEvent::onTrigger() - Invalid Target Group specified." );
        return;
    }

    // Change Camera.
    targetGroup->setActive();
}
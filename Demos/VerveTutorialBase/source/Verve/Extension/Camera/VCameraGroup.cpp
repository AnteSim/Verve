//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Camera/VCameraGroup.h"
#include "Verve/Extension/Camera/VCameraTrack.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VCameraGroup );
//-----------------------------------------------------------------------------

VCameraGroup *VCameraGroup::mActiveGroup = NULL;
VCameraGroup::CameraChangeSignal VCameraGroup::mCameraChangeSignal;

//-----------------------------------------------------------------------------

VCameraGroup::VCameraGroup( void )
{
    setLabel( "CameraGroup" );
};

//-----------------------------------------------------------------------------
//
// Tree Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraGroup::onAttach();
// 
// This callback subscribes this object to the controller's event signal.
// 
//-----------------------------------------------------------------------------
void VCameraGroup::onAttach( void )
{
    Parent::onAttach();

    // Valid Controller?
    if ( getController() )
    {
        // Subscribe to Events.
        getController()->getControllerEventSignal().notify( this, &VCameraGroup::onControllerEvent );
    }
}

//-----------------------------------------------------------------------------
// 
// VCameraGroup::onAttach();
// 
// This callback removes this object from the controller's event signal
// notification list.
// 
//-----------------------------------------------------------------------------
void VCameraGroup::onDetach( void )
{
    // Valid Controller?
    if ( getController() )
    {
        // Remove Event Notification.
        getController()->getControllerEventSignal().remove( this, &VCameraGroup::onControllerEvent );
    }

    Parent::onDetach();
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraGroup::onControllerEvent( pEvent );
// 
// When the controller's state changes, this method is called.
// 
// For a full list of possible events, see the 'eControllerEventType'
// declaration in VController.h.
// 
//-----------------------------------------------------------------------------
bool VCameraGroup::onControllerEvent( VController::eControllerEventType pEvent )
{
    if ( !getController() )
    {
        AssertFatal( false, "VCameraGroup::onControllerEvent() - Invalid Controller." );
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
#ifdef VT_EDITOR
        case VController::k_EventPause :
#endif
        case VController::k_EventStop :
            {

                // Clear the Camera.
                clearActiveGroup();

            } break;
    }

    return true;
}

//-----------------------------------------------------------------------------
//
// Camera Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraGroup::setActive();
// 
// Set this Group to Active.
// 
//-----------------------------------------------------------------------------
void VCameraGroup::setActive( void )
{
    // Set Active.
    setActiveGroup( this );
}

//-----------------------------------------------------------------------------
// 
// VCameraGroup::clearActiveGroup();
// 
// Clear the Active Camera.
// 
//-----------------------------------------------------------------------------
void VCameraGroup::clearActiveGroup( void )
{
    if ( mActiveGroup )
    {
        // Deactivate Signal.
        mActiveGroup->getCameraEventSignal().trigger( k_EventDeactivate );
    }

    // Store.
    mActiveGroup = NULL;

    // Clear Camera Object.
    VTorque::setCamera( NULL );

    // Change Signal.
    getCameraChangeSignal().trigger( NULL );
}

//-----------------------------------------------------------------------------
// 
// VCameraGroup::setActiveGroup( pCameraGroup );
// 
// Change the current camera group. The actual camera object is the object that
// the group references.
// 
// A NULL value of pCameraGroup will clear the active camera, which generally
// reverts to the connection's control object. The camera is also cleared when
// the Controller stops playing.
// 
//-----------------------------------------------------------------------------
void VCameraGroup::setActiveGroup( VCameraGroup *pCameraGroup )
{
    // Change Camera?
    if ( pCameraGroup == mActiveGroup ||
         pCameraGroup && !pCameraGroup->isEnabled() )
    {
        // Invalid Target.
        return;
    }

    if ( mActiveGroup )
    {
        // Deactivate Signal.
        mActiveGroup->getCameraEventSignal().trigger( k_EventDeactivate );
    }

    // Store.
    mActiveGroup = pCameraGroup;

    if ( mActiveGroup )
    {
        // Set Camera Object.
        VTorque::setCamera( mActiveGroup->getSceneObject() );

        // Activate Signal.
        mActiveGroup->getCameraEventSignal().trigger( k_EventActivate );
    }
    else
    {
        // Clear Camera Object.
        VTorque::setCamera( NULL );
    }

    // Change Signal.
    getCameraChangeSignal().trigger( mActiveGroup );
}
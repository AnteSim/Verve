//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Camera/VCameraTrack.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VCameraTrack );
//-----------------------------------------------------------------------------

VCameraTrack::VCameraTrack( void )
{
    setLabel( "CameraTrack" );
}

//-----------------------------------------------------------------------------
//
// Tree Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraTrack::onAttach();
// 
// This callback subscribes this object to the Camera Group's event signal.
// 
//-----------------------------------------------------------------------------
void VCameraTrack::onAttach( void )
{
    Parent::onAttach();

    // Valid Controller & Group?
    VCameraGroup *group;
    if ( getController() && getGroup( group ) )
    {
        // Add Event Notification.
        group->getCameraEventSignal().notify( this, &VCameraTrack::onCameraEvent );
    }
}

//-----------------------------------------------------------------------------
// 
// VCameraTrack::onAttach();
// 
// This callback removes this object from the Camera Group's event signal
// notification list.
// 
//-----------------------------------------------------------------------------
void VCameraTrack::onDetach( void )
{
    // Valid Controller & Group?
    VCameraGroup *group;
    if ( getController() && getGroup( group ) )
    {
        // Clear Event Notification.
        group->getCameraEventSignal().remove( this, &VCameraTrack::onCameraEvent );
    }

    Parent::onDetach();
}

//-----------------------------------------------------------------------------
//
// Camera Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraTrack::onCameraEvent( pEvent );
// 
// When the Camera changes, this method is called on both the outgoing and
// incomming Camera Groups.
// 
// For a full list of possible events, see the 'eCameraEventType' declaration
// in VCameraGroup.h.
// 
//-----------------------------------------------------------------------------
bool VCameraTrack::onCameraEvent( const VCameraGroup::eCameraEventType &pEvent )
{
    if ( !getController() )
    {
        AssertFatal( false, "VCameraTrack::onControllerEvent() - Invalid Controller." );
        return false;
    }

    // Ok.
    return true;
}
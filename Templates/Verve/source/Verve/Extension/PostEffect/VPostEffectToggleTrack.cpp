//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/PostEffect/VPostEffectToggleTrack.h"
#include "Verve/Extension/PostEffect/VPostEffectToggleEvent.h"
#include "Verve/Extension/Camera/VCameraGroup.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VPostEffectToggleTrack );
//-----------------------------------------------------------------------------

VPostEffectToggleTrack::VPostEffectToggleTrack( void ) : 
        mPostEffect( NULL )
{
    setLabel( "PostEffectTrack" );
}

void VPostEffectToggleTrack::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "PostEffect", TYPEID<VTorque::PostEffectType>(), Offset( mPostEffect, VPostEffectToggleTrack ), "The name of the PostEffect object to be triggered." );
}

//-----------------------------------------------------------------------------
//
// Camera Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VPostEffectToggleTrack::onCameraEvent( pEvent );
// 
// When the Camera changes, this method is called on both the outgoing and
// incoming Camera Groups.
// 
// For a full list of possible events, see the 'eCameraEventType' declaration
// in VCameraGroup.h.
// 
//-----------------------------------------------------------------------------
bool VPostEffectToggleTrack::onCameraEvent( const VCameraGroup::eCameraEventType &pEvent )
{
    // Parent Call.
    if ( !Parent::onCameraEvent( pEvent ) )
    {
        // Skip.
        return false;
    }

    // Enabled?
    if ( !isEnabled() || !mPostEffect.isValid() )
    {
        // Quit Now.
        return true;
    }

    switch( pEvent )
    {
        case VCameraGroup::k_EventActivate :
            {

                VPostEffectToggleEvent *event;
                if ( getPreviousEvent( event ) && event->mEventType == VSharedEnum::k_ActionTurnOn )
                {
                    // Toggle Post Effect On.
                    VTorque::setPostEffectOn( mPostEffect, true );
                }

            } break;

        case VCameraGroup::k_EventDeactivate :
            {

                // Turn Post Effect Off.
                VTorque::setPostEffectOn( mPostEffect, false );

            } break;
    }

    return true;
}
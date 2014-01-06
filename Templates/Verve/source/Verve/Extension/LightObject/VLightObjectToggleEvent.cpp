//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/LightObject/VLightObjectToggleEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VLightObjectToggleEvent );
//-----------------------------------------------------------------------------

VLightObjectToggleEvent::VLightObjectToggleEvent( void ) : 
        mEventType( VSharedEnum::k_ActionTurnOn )
{
    setLabel( "ToggleEvent" );
}

void VLightObjectToggleEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "Action", TYPEID<VActionToggle>(), Offset( mEventType, VLightObjectToggleEvent ) );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VLightObjectToggleEvent::onTrigger( pTime, pDelta );
// 
// Toggle the Light Object.
// 
//-----------------------------------------------------------------------------
void VLightObjectToggleEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    VTorque::LightObjectType *lightObject;
    if ( getSceneObject( lightObject ) )
    {
        // Turn On?
        const bool turnOn = ( mEventType == VSharedEnum::k_ActionTurnOn );

        // Toggle Light.
        VTorque::setLightObjectOn( lightObject, turnOn );
    }
}
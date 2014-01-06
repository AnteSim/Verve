//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/LightObject/VLightObjectAnimationEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VLightObjectAnimationEvent );
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

VLightObjectAnimationEvent::VLightObjectAnimationEvent( void ) : 
        mAnimationData( NULL )
{
    setLabel( "AnimationEvent" );
}

void VLightObjectAnimationEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "AnimationData", TYPEID<VTorque::LightAnimationDataType>(), Offset( mAnimationData, VLightObjectAnimationEvent ) );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VLightObjectAnimationEvent::onTrigger( pTime, pDelta );
// 
// When this Event is triggered the light object will begin to play the target
// animation.
// 
//-----------------------------------------------------------------------------
void VLightObjectAnimationEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch the Light Object.
    VTorque::LightObjectType *lightObject;
    if ( getSceneObject( lightObject ) )
    {
        // Play the Animation.
        VTorque::playAnimation( lightObject, mAnimationData );
    }
}

//-----------------------------------------------------------------------------
// 
// VLightObjectAnimationEvent::onComplete( pTime, pDelta );
// 
// The current animation played by the light object will be paused when this
// Event completes its updates.
// 
//-----------------------------------------------------------------------------
void VLightObjectAnimationEvent::onComplete( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch the Light Object.
    VTorque::LightObjectType *lightObject;
    if ( getSceneObject( lightObject ) )
    {
        // Pause the Animation.
        VTorque::pauseAnimation( lightObject );
    }
}
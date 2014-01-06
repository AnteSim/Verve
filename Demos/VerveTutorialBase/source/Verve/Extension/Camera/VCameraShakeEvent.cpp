//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VGroup.h"
#include "Verve/Extension/Camera/VCameraGroup.h"
#include "Verve/Extension/Camera/VCameraShakeEvent.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VCameraShakeEvent );
//-----------------------------------------------------------------------------

VCameraShakeEvent::VCameraShakeEvent( void ) : 
        mAmplitude( Point3F::Zero ),
        mFalloff( 10.f ),
        mFrequency( Point3F::Zero )
{
    // Clear Label.
    setLabel( "CameraShakeEvent" );
}

void VCameraShakeEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "Amplitude", TypePoint3F, Offset( mAmplitude, VCameraShakeEvent ), "Amplitude of the Camera Shake event." );
    addField( "Falloff",   TypeF32,     Offset( mFalloff,   VCameraShakeEvent ), "Falloff of the Camera Shake event." );
    addField( "Frequency", TypePoint3F, Offset( mFrequency, VCameraShakeEvent ), "Frequency of the Camera Shake event." );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VCameraShakeEvent::onTrigger( pTime, pDelta );
// 
// Start shaking the camera. Also account for any offet in playtime, and
// timescale.
// 
//-----------------------------------------------------------------------------
void VCameraShakeEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch Group.
    VCameraGroup *group;
    if ( !getGroup( group ) || !group->isActive() )
    {
        // Inactive.
        return;
    }

    // Duration.
    //const F32 duration  = ( mDuration - mAbs( pTime - getStartTime() ) ) / ( 1000.f * mFabs( getControllerTimeScale() ) );
    const F32 duration  = ( mDuration - mAbs( pTime - getStartTime() ) ) / 1000.f;

    // Shake Camera.
    VTorque::startCameraShake( duration, mFalloff, mAmplitude, mFrequency );
}
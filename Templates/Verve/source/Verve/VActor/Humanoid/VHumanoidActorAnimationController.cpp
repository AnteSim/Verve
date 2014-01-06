//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VHumanoidActorAnimationController.h"

//-----------------------------------------------------------------------------

VHumanoidActorAnimationController::VHumanoidActorAnimationController( void )
{
    // Void.
}

VHumanoidActorAnimationController::~VHumanoidActorAnimationController( void )
{
    // Void.
}

//-----------------------------------------------------------------------------

bool VHumanoidActorAnimationController::initArmAnimation( const U32 &pIndex, const F32 &pPosition, const F32 &pTimeScale )
{
    // Initialise Animation Ref.
    return initAnimation( mArmAnimation, pIndex, pPosition, pTimeScale );
}




//-----------------------------------------------------------------------------
//
// Animation Methods
//
//-----------------------------------------------------------------------------

void VHumanoidActorAnimationController::update( const F32 &pDelta )
{
    // Parent Call.
    VActorAnimationController::update( pDelta );

    // Update the Look Thread.
    if ( mArmAnimation.Thread )
    {
        // Set Position.
        getShapeInstance()->setPos( mArmAnimation.Thread, 0.5f );
    }
}
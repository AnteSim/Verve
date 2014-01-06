//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VHumanoidPhysicsStates.h"
#include "VHumanoidActor.h"

#include "../VActorPhysicsController.h"

//-----------------------------------------------------------------------------
//
// Implement Physics States.
//
//-----------------------------------------------------------------------------

ImplementActorPhysicsState( HumanoidOnGround, VHumanoidActorData::k_OnGroundPhysics );
ImplementActorPhysicsState( HumanoidInAir,    VHumanoidActorData::k_InAirPhysics );
ImplementActorPhysicsState( HumanoidInWater,  VHumanoidActorData::k_InWaterPhysics );




//-----------------------------------------------------------------------------
//
// Execute Animation States.
//
//-----------------------------------------------------------------------------

ExecuteActorPhysicsState( HumanoidOnGround )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // On the Ground?
    if ( !physicsController->isOnGround() )
    {
        // No.
        return false;
    }

    // On Ground.
    return true;
}

ProcessActorPhysicsState( HumanoidOnGround )
{
    // Void.
}

//-----------------------------------------------------------------------------

ExecuteActorPhysicsState( HumanoidInAir )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // In the Air?
    if ( !physicsController->isInAir() )
    {
        // No.
        return false;
    }

    // In Air.
    return true;
}

ProcessActorPhysicsState( HumanoidInAir )
{
    // Apply Gravity for the Tick.
    pObject->getPhysicsController()->applyGravity( pElapsedTime );
}

//-----------------------------------------------------------------------------

ExecuteActorPhysicsState( HumanoidInWater )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // Sumberged?
    if ( !physicsController->isInWater() )
    {
        // No.
        return false;
    }

    // Swimming
    return true;
}

ProcessActorPhysicsState( HumanoidInWater )
{
    // Void.
}
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VHumanoidAnimationStates.h"
#include "VHumanoidActor.h"

#include "../VActorAnimationController.h"
#include "../VActorPhysicsController.h"

//-----------------------------------------------------------------------------
//
// Implement Animation States.
//
//-----------------------------------------------------------------------------

ImplementActorAnimationState( HumanoidIdle,         VHumanoidActorData::k_IdleAnimation );

ImplementActorAnimationState( HumanoidWalkForward,  VHumanoidActorData::k_WalkForwardAnimation );
ImplementActorAnimationState( HumanoidWalkBackward, VHumanoidActorData::k_WalkBackwardAnimation );

ImplementActorAnimationState( HumanoidRunForward,   VHumanoidActorData::k_RunForwardAnimation );
ImplementActorAnimationState( HumanoidRunBackward,  VHumanoidActorData::k_RunBackwardAnimation );

ImplementActorAnimationState( HumanoidSwimIdle,     VHumanoidActorData::k_SwimIdleAnimation );
ImplementActorAnimationState( HumanoidSwimForward,  VHumanoidActorData::k_SwimForwardAnimation );
ImplementActorAnimationState( HumanoidSwimBackward, VHumanoidActorData::k_SwimBackwardAnimation );




//-----------------------------------------------------------------------------
//
// Execute Animation States.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// OnGround Animation States
//
//-----------------------------------------------------------------------------

ExecuteActorAnimationState( HumanoidIdle )
{
    // Always Enter.
    return true;
}

ExecuteActorAnimationState( HumanoidWalkForward )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // On the Ground?
    if ( physicsController->getPhysicsState() != VHumanoidActorData::k_OnGroundPhysics )
    {
        // Can't Run Forward.
        return false;
    }

    // Fetch Velocity.
    const VectorF &velocity = physicsController->getVelocity();
    // Determine Move Speed.
    const F32 moveSpeed = mSqrt( velocity.x * velocity.x + velocity.y * velocity.y );

    // Moving Forward & Slow Enough?
    return ( ( physicsController->getMoveState() & k_ForwardMove ) &&
             ( moveSpeed < pObject->getDataBlock()->getRunSpeed() ) );
}

ExecuteActorAnimationState( HumanoidWalkBackward )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // On the Ground?
    if ( physicsController->getPhysicsState() != VHumanoidActorData::k_OnGroundPhysics )
    {
        // Can't Run Backward.
        return false;
    }

    // Fetch Velocity.
    const VectorF &velocity = physicsController->getVelocity();
    // Determine Move Speed.
    const F32 moveSpeed = mSqrt( velocity.x * velocity.x + velocity.y * velocity.y );

    // Moving Backward?
    return ( ( physicsController->getMoveState() & k_BackwardMove ) &&
             ( moveSpeed < pObject->getDataBlock()->getRunSpeed() ) );
}

ExecuteActorAnimationState( HumanoidRunForward )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // On the Ground?
    if ( physicsController->getPhysicsState() != VHumanoidActorData::k_OnGroundPhysics )
    {
        // Can't Run Forward.
        return false;
    }

    // Fetch Velocity.
    const VectorF &velocity = physicsController->getVelocity();
    // Determine Move Speed.
    const F32 moveSpeed = mSqrt( velocity.x * velocity.x + velocity.y * velocity.y );

    // Moving Forward?
    return ( ( physicsController->getMoveState() & k_ForwardMove ) &&
             ( moveSpeed >= pObject->getDataBlock()->getRunSpeed() ) );
}

ExecuteActorAnimationState( HumanoidRunBackward )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // On the Ground?
    if ( physicsController->getPhysicsState() != VHumanoidActorData::k_OnGroundPhysics )
    {
        // Can't Run Backward.
        return false;
    }

    // Fetch Velocity.
    const VectorF &velocity = physicsController->getVelocity();
    // Determine Move Speed.
    const F32 moveSpeed = mSqrt( velocity.x * velocity.x + velocity.y * velocity.y );

    // Moving Backward?
    return ( ( physicsController->getMoveState() & k_BackwardMove ) &&
             ( moveSpeed >= pObject->getDataBlock()->getRunSpeed() ) );
}




//-----------------------------------------------------------------------------
//
// InWater Animation States
//
//-----------------------------------------------------------------------------

ExecuteActorAnimationState( HumanoidSwimIdle )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // In the Water?
    if ( physicsController->getPhysicsState() != VHumanoidActorData::k_InWaterPhysics )
    {
        // Can't Swim.
        return false;
    }

    // Idle?
    return ( physicsController->getMoveState() & k_NullMove );
}

ExecuteActorAnimationState( HumanoidSwimForward )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // In the Water?
    if ( physicsController->getPhysicsState() != VHumanoidActorData::k_InWaterPhysics )
    {
        // Can't Swim.
        return false;
    }

    // Moving Around?
    return ( physicsController->getMoveState() & ( k_ForwardMove |
                                                   k_UpMove |
                                                   k_DownMove ) );
}

ExecuteActorAnimationState( HumanoidSwimBackward )
{
    // Fetch Controller.
    VActorPhysicsController *physicsController = pObject->getPhysicsController();

    // In the Water?
    if ( physicsController->getPhysicsState() != VHumanoidActorData::k_InWaterPhysics )
    {
        // Can't Swim.
        return false;
    }

    // Moving Backward?
    return ( physicsController->getMoveState() & k_BackwardMove );
}
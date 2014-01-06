//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VHumanoidActorData.h"
#include "VHumanoidAnimationStates.h"
#include "VHumanoidPhysicsStates.h"

//-----------------------------------------------------------------------------
// Animation Table.
//-----------------------------------------------------------------------------

static VActorData::sAnimationSequence animSequenceLookup[] =
    {
        // State Based Animations.
        { VHumanoidActorData::k_IdleAnimation,           "root",     0.0f, ActorAnimationStateInstance( HumanoidIdle )         },

        { VHumanoidActorData::k_WalkForwardAnimation,    "walk",     0.1f, ActorAnimationStateInstance( HumanoidWalkForward )  },
        { VHumanoidActorData::k_WalkBackwardAnimation,   "walkback", 0.1f, ActorAnimationStateInstance( HumanoidWalkBackward ) },

        { VHumanoidActorData::k_RunForwardAnimation,     "run",      0.1f, ActorAnimationStateInstance( HumanoidRunForward )   },
        { VHumanoidActorData::k_RunBackwardAnimation,    "runback",  0.1f, ActorAnimationStateInstance( HumanoidRunBackward )  },

        { VHumanoidActorData::k_SwimIdleAnimation,       "swimroot", 1.0f, ActorAnimationStateInstance( HumanoidSwimIdle )     },
        { VHumanoidActorData::k_SwimForwardAnimation,    "swim",     1.0f, ActorAnimationStateInstance( HumanoidSwimForward )  },
        { VHumanoidActorData::k_SwimBackwardAnimation,   "swimback", 1.0f, ActorAnimationStateInstance( HumanoidSwimBackward ) },

        // Support Animations.
        { VHumanoidActorData::k_HeadHorizontalAnimation, "headside" },
        { VHumanoidActorData::k_HeadVerticalAnimation,   "head"     },

        { VHumanoidActorData::k_ArmsUpDownAnimation,     "look"     },
    };

//-----------------------------------------------------------------------------
// Physics Table.
//-----------------------------------------------------------------------------

static VActorData::sPhysicsState physStateLookup[] =
    {
        { VHumanoidActorData::k_OnGroundPhysics, 0.f, ActorPhysicsStateInstance( HumanoidOnGround ) },
        { VHumanoidActorData::k_InAirPhysics,    0.f, ActorPhysicsStateInstance( HumanoidInAir )    },
        { VHumanoidActorData::k_InWaterPhysics,  0.f, ActorPhysicsStateInstance( HumanoidInWater )  },
    };

//-----------------------------------------------------------------------------
IMPLEMENT_CO_DATABLOCK_V1( VHumanoidActorData );
//-----------------------------------------------------------------------------

VHumanoidActorData::VHumanoidActorData( void )
{
    // Void.
};

bool VHumanoidActorData::preload( bool pServer, String &pErrorStr )
{
    if ( !Parent::preload( pServer, pErrorStr ) )
    {
        return false;
    }

    // Initialise Animation List.
    if ( !initAnimationSequenceList( sizeof( animSequenceLookup ) /  sizeof( VActorData::sAnimationSequence ), &animSequenceLookup[0] ) )
    {
        Con::errorf( "VHumanoidActorData::preload() - Failed to Initialise Actor Animations." );
        return false;
    }

    // Initialise Physics State List.
    if ( !initPhysicsStateList( sizeof( physStateLookup ) /  sizeof( VActorData::sPhysicsState ), &physStateLookup[0] ) )
    {
        Con::errorf( "VHumanoidActorData::preload() - Failed to Initialise Actor Physics States." );
        return false;
    }

    // Valid Load.
    return true;
}
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Game/VSpawnSphereSpawnTargetEvent.h"
#include "Verve/Extension/Game/VSpawnSphereSpawnTargetTrack.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSpawnSphereSpawnTargetEvent );
//-----------------------------------------------------------------------------

VSpawnSphereSpawnTargetEvent::VSpawnSphereSpawnTargetEvent( void )
{
    setLabel( "SpawnTargetEvent" );
}

void VSpawnSphereSpawnTargetEvent::initPersistFields( void )
{
    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSpawnSphereSpawnTargetEvent::onTrigger( pTime, pDelta );
// 
// Spawn the Target.
// 
//-----------------------------------------------------------------------------
void VSpawnSphereSpawnTargetEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch Track.
    VSpawnSphereSpawnTargetTrack *track;
    if ( !getTrack( track ) )
    {
        return;
    }

    // Spawn the Target.
    track->spawnTarget();
}
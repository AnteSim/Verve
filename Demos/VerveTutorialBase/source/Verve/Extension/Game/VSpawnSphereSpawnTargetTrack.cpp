//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Game/VSpawnSphereSpawnTargetTrack.h"
#include "Verve/Torque/TSpawnSphere.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSpawnSphereSpawnTargetTrack );
//-----------------------------------------------------------------------------

VSpawnSphereSpawnTargetTrack::VSpawnSphereSpawnTargetTrack( void )
{
    setLabel( "SpawnTargetTrack" );
}

void VSpawnSphereSpawnTargetTrack::initPersistFields( void )
{
    // Parent Call.
    Parent::initPersistFields();

    addField( "DespawnOnLoop", TypeBool, Offset( mDespawnOnLoop, VSpawnSphereSpawnTargetTrack ), "Despawn all targets when the Controller loops?" );
    addField( "DespawnOnStop", TypeBool, Offset( mDespawnOnStop, VSpawnSphereSpawnTargetTrack ), "Despawn all targets when the Controller stops playing?" );
}

//-----------------------------------------------------------------------------
//
// Controller Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSpawnSphereSpawnTargetTrack::onControllerEvent( pEvent );
// 
// For a full list of possible events, see the 'eControllerEventType'
// declaration in VController.h.
// 
//-----------------------------------------------------------------------------
bool VSpawnSphereSpawnTargetTrack::onControllerEvent( VController::eControllerEventType pEvent )
{
    if ( !Parent::onControllerEvent( pEvent ) )
    {
        // Skip.
        return false;
    }

    // Enabled?
    if ( !isEnabled() )
    {
        // Continue Processing Events.
        return true;
    }

    switch ( pEvent )
    {
        case VController::k_EventLoop :
            {
                if ( mDespawnOnLoop )
                {
                    despawnTargets();
                }

            } break;

        case VController::k_EventStop :
            {
                if ( mDespawnOnStop )
                {
                    despawnTargets();
                }

            } break;
    }

    return true;
}

//-----------------------------------------------------------------------------
//
// Spawn Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSpawnSphereSpawnTargetTrack::spawnTarget( pTime, pForward );
// 
// Spawn an Object.
// 
//-----------------------------------------------------------------------------
void VSpawnSphereSpawnTargetTrack::spawnTarget( void )
{
    VTorque::SpawnSphereType *object;
    if ( !getSceneObject( object ) )
    {
        return;
    }

    // Spawn the Object.
    SimObject *spawnedObject = object->spawnObject();

    // Scene Object?
    VTorque::SceneObjectType *sceneObject = dynamic_cast<VTorque::SceneObjectType*>( spawnedObject );
    if ( sceneObject )
    {
        sceneObject->setPosition( object->getPosition() );
    }

    // Valid?
    if ( spawnedObject )
    {
        // Add Reference.
        mSpawnList.addObject( spawnedObject );
    }
}

//-----------------------------------------------------------------------------
// 
// VSpawnSphereSpawnTargetTrack::despawnTargets();
// 
// Despawn all of the objects spawned by this track.
// 
//-----------------------------------------------------------------------------
void VSpawnSphereSpawnTargetTrack::despawnTargets( void )
{
    while( mSpawnList.size() > 0 )
    {
        // Fetch the Last Object
        SimObject *object = mSpawnList.last();
        // Remove it.
        mSpawnList.popObject();

        // Delete the Object.
        object->deleteObject();
    }
}
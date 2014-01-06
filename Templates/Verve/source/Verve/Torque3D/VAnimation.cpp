//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Torque/TAnimation.h"

#include "T3D/shapeBase.h"

//-----------------------------------------------------------------------------
//
// Animation Methods.
//
//-----------------------------------------------------------------------------

bool VTorque::isAnimationLooping( SceneObjectType *pObject, const char *pData )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape || !shape->getShape() )
    {
        // Sanity!
        return false;
    }

    // Find Sequence.
    const S32 sequenceIndex = shape->getShape()->findSequence( pData );
    if ( sequenceIndex == -1 )
    {
        // Invalid Sequence.
        return false;
    }

    // Return Cyclic.
    return shape->getShape()->sequences[sequenceIndex].isCyclic();
}

String VTorque::getAnimation( SceneObjectType *pObject, const U32 &pThreadIndex )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape )
    {
        // Sanity!
        return "";
    }

    // Return Name.
    return shape->getThreadSequenceName( pThreadIndex );
}

F32 VTorque::getAnimationDuration( SceneObjectType *pObject, const char *pData )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape || !shape->getShape() )
    {
        // Sanity!
        return 0.f;
    }

    // Find Sequence.
    const S32 sequenceIndex = shape->getShape()->findSequence( pData );
    if ( sequenceIndex == -1 )
    {
        // Invalid Sequence.
        return 0.f;
    }

    // Return Duration.
    return shape->getShape()->sequences[sequenceIndex].duration;
}

void VTorque::setAnimationPosition( SceneObjectType *pObject, const U32 &pThreadIndex, const F32 &pPosition )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape )
    {
        // Sanity!
        return;
    }

    // Set Position.
    shape->setThreadPosition( 0, pPosition );
}

void VTorque::setAnimationTimeScale( SceneObjectType *pObject, const U32 &pThreadIndex, const F32 &pTimeScale )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape )
    {
        // Sanity!
        return;
    }

    // Set TimeScale.
    shape->setThreadTimeScale( pThreadIndex, pTimeScale );
}

void VTorque::playAnimation( SceneObjectType *pObject, const U32 &pThreadIndex, const char *pData )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape || !shape->getShape() )
    {
        // Sanity!
        return;
    }

    // Find Sequence.
    const S32 sequenceIndex = shape->getShape()->findSequence( pData );
    if ( sequenceIndex == -1 )
    {
        // Invalid Sequence.
        return;
    }

    // Play Sequence.
    shape->setThreadSequence( pThreadIndex, sequenceIndex );
}

void VTorque::playAnimation( SceneObjectType *pObject, const U32 &pThreadIndex )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape )
    {
        // Sanity!
        return;
    }

    // Play Sequence.
    shape->playThread( pThreadIndex );
}

void VTorque::stopAnimation( SceneObjectType *pObject, const U32 &pThreadIndex )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape )
    {
        // Sanity!
        return;
    }

    // Pause Thread.
    shape->stopThread( pThreadIndex );
}

void VTorque::pauseAnimation( SceneObjectType *pObject, const U32 &pThreadIndex )
{
    ShapeBase *shape = dynamic_cast<ShapeBase*>( pObject );
    if ( !shape )
    {
        // Sanity!
        return;
    }

    // Pause Thread.
    shape->pauseThread( pThreadIndex );
}
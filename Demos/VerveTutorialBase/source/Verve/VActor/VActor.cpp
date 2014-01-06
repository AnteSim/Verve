//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VActor.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CO_NETOBJECT_V1( VActor );
//-----------------------------------------------------------------------------

VActor::VActor( void ) :
        mDataBlock( NULL )
{
    // Void.
}

VActor::~VActor( void )
{
    // Void.
}




//-----------------------------------------------------------------------------
//
// Initialisation Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// VActor::onAdd();
//
// ...
//
//-----------------------------------------------------------------------------
bool VActor::onAdd( void )
{
    if ( !Parent::onAdd() || !mDataBlock )
    {
        return false;
    }

    // Add to Scene.
    addToScene();

    if ( isServerObject() )
    {
        // Script Callback.
        scriptOnAdd();
    }

    return true;
}

//-----------------------------------------------------------------------------
//
// VActor::onRemove();
//
// ...
//
//-----------------------------------------------------------------------------
void VActor::onRemove( void )
{
    // Script Callback.
    scriptOnRemove();

    // Remove From Scene.
    removeFromScene();

    Parent::onRemove();
}

//-----------------------------------------------------------------------------
//
// VActor::onNewDataBlock( pDataBlock );
//
// ...
//
//-----------------------------------------------------------------------------
bool VActor::onNewDataBlock( GameBaseData *pDataBlock, bool pReload )
{
    // Store DataBlock Reference.
    mDataBlock = dynamic_cast<VActorData*>( pDataBlock );

    if ( !mDataBlock )
    {
        // Invalid Data.
        return false;
    }

    // Parent Call.
    return Parent::onNewDataBlock( pDataBlock, pReload );
}




//-----------------------------------------------------------------------------
//
// Update Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// VActor::processTick( pMove );
//
// ...
//
//-----------------------------------------------------------------------------
void VActor::processTick( const Move *pMove )
{
    // Parent Call.
    Parent::processTick( pMove );

    // Triggers?
    if ( pMove && mDamageState == Enabled )
    {
        // Handle each Image Trigger.
        const U32 imageCount = getMin( ShapeBase::MaxMountedImages, MaxTriggerKeys );
        for ( U32 i = 0; i < imageCount; i++ )
        {
            setImageTriggerState( i, pMove->trigger[i] );
        }
    }
}

//-----------------------------------------------------------------------------
//
// VActor::packUpdate( pConnection, pMask, pStream );
//
// ...
//
//-----------------------------------------------------------------------------
U32 VActor::packUpdate( NetConnection *pConnection, U32 pMask, BitStream *pStream )
{
    // Parent Call.
    return Parent::packUpdate( pConnection, pMask, pStream );
}

//-----------------------------------------------------------------------------
//
// VActor::unpackUpdate( pConnection, pStream );
//
// ...
//
//-----------------------------------------------------------------------------
void VActor::unpackUpdate( NetConnection *pConnection, BitStream *pStream )
{
    // Parent Call.
    Parent::unpackUpdate( pConnection, pStream );
}




//-----------------------------------------------------------------------------
//
// Physics Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// VActor::setTransform( pMatrix );
//
// ...
//
//-----------------------------------------------------------------------------
void VActor::setTransform( const MatrixF &pMatrix )
{
    Parent::setTransform( pMatrix );

    // Server Object?
    if ( isServerObject() )
    {
        // Move Object.
        setMaskBits( MoveMask );
    }
}

//-----------------------------------------------------------------------------
//
// VActor::onMount( pObject, pNode );
//
// ...
//
//-----------------------------------------------------------------------------
void VActor::onMount( SceneObject *pObject, S32 pNode )
{
    // Parent Call.
    Parent::onMount( pObject, pNode );

    // Post Event.
    mEventSignal.trigger( k_MountEvent );
}

//-----------------------------------------------------------------------------
//
// VActor::onUnmount( pObject, pNode );
//
// ...
//
//-----------------------------------------------------------------------------
void VActor::onUnmount( SceneObject *pObject, S32 pNode )
{
    // Parent Call.
    Parent::onUnmount( pObject, pNode );

    // Post Event.
    mEventSignal.trigger( k_UnmountEvent );
}
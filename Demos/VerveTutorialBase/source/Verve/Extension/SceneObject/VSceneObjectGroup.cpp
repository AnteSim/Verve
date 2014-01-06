//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#include "Verve/Core/VController.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSceneObjectGroup );
//-----------------------------------------------------------------------------

VSceneObjectGroup::VSceneObjectGroup( void ) : 
        mDataReference( String::EmptyString ),
        mSceneObject( NULL )
{
    setLabel( "SceneObjectGroup" );
};

void VSceneObjectGroup::initPersistFields( void )
{
    Parent::initPersistFields();

    addField( "Reference", TypeRealString, Offset( mDataReference, VSceneObjectGroup ), "The name of the data field referencing the targeted object." );
}

//-----------------------------------------------------------------------------
//
// Reference Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSceneObjectGroup::getObject();
// 
// Returns the group's object reference.
// 
//-----------------------------------------------------------------------------
VTorque::SceneObjectType *VSceneObjectGroup::getSceneObject( void )
{
#ifndef VT_EDITOR
    // Already Referenced?
    if ( mSceneObject )
    {
        // Return Object.
        return mSceneObject;
    }
#endif

    VController *controller = getController();
    if ( !controller )
    {
        // No Controller!
        return NULL;
    }

    String fieldValue;
    if ( controller->getDataValue( mDataReference, fieldValue ) )
    {
        // Store Object.
        mSceneObject = dynamic_cast<VTorque::SceneObjectType*>( Sim::findObject( fieldValue ) );
    }

    // Return.
    return mSceneObject;
}

#ifdef VT_EDITOR
//-----------------------------------------------------------------------------
//
// Debug Methods.
//
//-----------------------------------------------------------------------------

ConsoleMethod( VSceneObjectGroup, getSceneObject, S32, 2, 2, "( void ) - Get the object this group references.\n"
                                                             "@return Returns the SimObjectID for the object." )
{
    // Fetch Object.
    VTorque::SceneObjectType *objReference = object->getSceneObject();

    // Return.
    return ( objReference ) ? objReference->getId() : 0;
}
#endif
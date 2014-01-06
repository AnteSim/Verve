//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSceneObjectEvent );
//-----------------------------------------------------------------------------

VSceneObjectEvent::VSceneObjectEvent( void )
{
    // Void.
}

//-----------------------------------------------------------------------------
//
// Reference Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSceneObjectEvent::getSceneObject();
// 
// Returns the parent group's object reference.
// 
//-----------------------------------------------------------------------------
VTorque::SceneObjectType *VSceneObjectEvent::getSceneObject( void )
{
    VSceneObjectGroup *group;
    if ( !getGroup( group ) )
    {
        // No Group!
        return NULL;
    }

    // Return Object.
    return group->getSceneObject();
}

#ifdef VT_EDITOR
//-----------------------------------------------------------------------------
//
// Debug Methods.
//
//-----------------------------------------------------------------------------

ConsoleMethod( VSceneObjectEvent, getSceneObject, S32, 2, 2, "( void ) - Get the object this group references.\n"
                                                             "@return Returns the SimObjectID for the object." )
{
    // Fetch Object.
    VTorque::SceneObjectType *objReference = object->getSceneObject();

    // Return.
    return ( objReference ) ? objReference->getId() : 0;
}
#endif
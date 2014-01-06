//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#include "Verve/Extension/SceneObject/VSceneObjectTrack.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSceneObjectTrack );
//-----------------------------------------------------------------------------

VSceneObjectTrack::VSceneObjectTrack( void )
{
    setLabel( "SceneObjectTrack" );
}

//-----------------------------------------------------------------------------
//
// Reference Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSceneObjectTrack::getSceneObject();
// 
// Returns the parent group's object reference.
// 
//-----------------------------------------------------------------------------
VTorque::SceneObjectType *VSceneObjectTrack::getSceneObject( void )
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

ConsoleMethod( VSceneObjectTrack, getSceneObject, S32, 2, 2, "( void ) - Get the object this group references.\n"
                                                             "@return Returns the SimObjectID for the object." )
{
    // Fetch Object.
    VTorque::SceneObjectType *objReference = object->getSceneObject();

    // Return.
    return ( objReference ) ? objReference->getId() : 0;
}
#endif
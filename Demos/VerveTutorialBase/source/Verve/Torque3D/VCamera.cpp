//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/VerveConfig.h"
#include "Verve/Torque/TCamera.h"
#include "Verve/Torque3D/VCameraShake.h"

#include "T3D/gameBase/gameConnection.h"

//-----------------------------------------------------------------------------
//
// Camera Methods.
//
//-----------------------------------------------------------------------------

bool VTorque::isCamera( SceneObjectType *pObject )
{
    return ( dynamic_cast<GameBase*>( pObject ) != NULL );
}

void VTorque::setCamera( SceneObjectType *pObject )
{
    // Fetch Game Base.
    GameBase *object = dynamic_cast<GameBase*>( pObject );

    // Fetch Client Group.
    SimGroup* clientGroup = Sim::getClientGroup();

    for ( SimGroup::iterator itr = clientGroup->begin(); itr != clientGroup->end(); itr++ )
    {
        GameConnection *connection = dynamic_cast<GameConnection*>( *itr );
        if ( connection )
        {
            // Set Camera Object.
            connection->setCameraObject( object );
        }
    }
}
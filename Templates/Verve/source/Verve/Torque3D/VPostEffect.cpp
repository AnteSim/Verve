//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/VerveConfig.h"
#include "Verve/Torque3D/VPostEffect.h"

#include "T3D/gameBase/gameConnection.h"
#include "core/stream/bitStream.h"

//-----------------------------------------------------------------------------
//
// Post Effect Methods.
//
//-----------------------------------------------------------------------------

bool VTorque::isPostEffectEnabled( PostEffectType *pPostEffect )
{
    if ( !pPostEffect )
    {
        // Sanity!
        return false;
    }

    return pPostEffect->isEnabled();
}

void VTorque::setPostEffectOn( PostEffectType *pPostEffect, const bool &pStatus )
{
    if ( !pPostEffect )
    {
        // Sanity!
        return;
    }

#ifdef VT_EDITOR

    if ( pStatus )
    {
        // Enable Effect.
        pPostEffect->enable();
    }
    else
    {
        // Disable Effect.
        pPostEffect->disable();
    }

#else

    // Fetch Name.
    StringTableEntry name = pPostEffect->getName();
    if ( !name || name == StringTable->insert( "" ) )
    {
        Con::warnf( "VTorque::setPostEffectOn() - Invalid Object Name." );
        return;
    }

    // Fetch Client Group.
    SimGroup* clientGroup = Sim::getClientGroup();

    for ( SimGroup::iterator itr = clientGroup->begin(); itr != clientGroup->end(); itr++ )
    {
        NetConnection *connection = static_cast<NetConnection*>( *itr );
        if ( connection )
        {
            // Create Event.
            VPostEffectNetEvent *event = new VPostEffectNetEvent();

            // Setup Event.
            event->mPostEffect = name;
            event->mEnabled    = pStatus;

            // Post Event.
            connection->postNetEvent( event );
        }
    }

#endif
}

//-----------------------------------------------------------------------------
IMPLEMENT_CO_CLIENTEVENT_V1( VPostEffectNetEvent );
//-----------------------------------------------------------------------------

VPostEffectNetEvent::VPostEffectNetEvent( void ) : mPostEffect( StringTable->insert( "" ) ),
                                                   mEnabled( false )
{
    // Void.
}

void VPostEffectNetEvent::write( NetConnection *pConnection, BitStream *pStream )
{
    // Void.
}

void VPostEffectNetEvent::pack( NetConnection *pConnection, BitStream *pStream )
{
    // Object Name.
    pStream->writeString( mPostEffect );

    // Status.
    pStream->writeFlag( mEnabled );
}

void VPostEffectNetEvent::unpack( NetConnection *pConnection, BitStream *pStream )
{
    // Object Name.
    mPostEffect = pStream->readSTString();

    // Status.
    mEnabled = pStream->readFlag();
}

void VPostEffectNetEvent::process( NetConnection *pConnection )
{
    PostEffect *postEffect;
    if ( !Sim::findObject( mPostEffect, postEffect ) )
    {
        Con::warnf( "VPostEffectNetEvent::process() - Unable to find PostEffect Object '%s'", mPostEffect );
        return;
    }

    if ( mEnabled )
    {
        // Enable Effect.
        postEffect->enable();
    }
    else
    {
        // Disable Effect.
        postEffect->disable();
    }
}
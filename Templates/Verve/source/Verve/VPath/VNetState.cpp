//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "VNetState.h"

//-----------------------------------------------------------------------------

U32 VNetState::gInvalidMask = 0;

//-----------------------------------------------------------------------------

VNetState::VNetState() :
        mMask( 0 )
{
}

VNetState::~VNetState( void )
{
    for ( VNetState::iterator itr = begin(); itr != end(); itr++ )
    {
        // Fetch info.
        VNetStateInfo *state = ( *itr );
        // Delete State.
        delete state;
    }

    // Clear.
    clear();
}

//-----------------------------------------------------------------------------
//
// Connection Methods.
//
//-----------------------------------------------------------------------------

bool VNetState::isConnection( NetConnection *pConnection )
{
    for ( VNetState::iterator itr = begin(); itr != end(); itr++ )
    {
        VNetStateInfo *state = ( *itr );
        if ( state->Connection == pConnection )
        {
            // Valid.
            return true;
        }
    }

    // Invalid.
    return false;
}

void VNetState::addConnection( NetConnection *pConnection )
{
    // Init State.
    VNetStateInfo *state = new VNetStateInfo( pConnection, mMask );
    // Add.
    push_back( state );
}

void VNetState::clearConnection( NetConnection *pConnection )
{
    for ( VNetState::iterator itr = begin(); itr != end(); itr++ )
    {
        VNetStateInfo *state = ( *itr );
        if ( state->Connection == pConnection )
        {
            // Delete.
            delete state;
            // Erase.
            erase( itr );
            // Quit.
            return;
        }
    }
}

//-----------------------------------------------------------------------------
//
// Mask Methods.
//
//-----------------------------------------------------------------------------

VNetStateInfo *VNetState::getState( NetConnection *pConnection )
{
    for ( VNetState::iterator itr = begin(); itr != end(); itr++ )
    {
        VNetStateInfo *state = ( *itr );
        if ( state->Connection == pConnection )
        {
            return state;
        }
    }

    return NULL;
}

void VNetState::setMaskBits( const U32 &pMask )
{
    // Apply Mask.
    mMask |= pMask;

    for ( VNetState::iterator itr = begin(); itr != end(); itr++ )
    {
        // Fetch info.
        VNetStateInfo *state = ( *itr );
        // Apply Mask.
        state->Mask |= pMask;
    }
}

void VNetState::clearMaskBits( const U32 &pMask )
{
    // Clear Mask.
    mMask &= ~pMask;

    for ( VNetState::iterator itr = begin(); itr != end(); itr++ )
    {
        // Fetch info.
        VNetStateInfo *state = ( *itr );
        // Clear Mask.
        state->Mask &= ~pMask;
    }
}
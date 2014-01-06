//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Director/VDirectorGroup.h"
#include "Verve/Extension/Director/VDirectorTrack.h"
#include "Verve/Extension/Camera/VCameraGroup.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VDirectorGroup );
//-----------------------------------------------------------------------------

VDirectorGroup::VDirectorGroup( void ) :
        mActiveCamera( NULL )
{
    setLabel( "DirectorGroup" );
};

//-----------------------------------------------------------------------------
// 
// VDirectorGroup::getDirectorTrack();
// 
// Returns the DirectorTrack reference.
// 
//-----------------------------------------------------------------------------
VDirectorTrack *VDirectorGroup::getDirectorTrack( void )
{
    for ( ITreeNode *node = mChildNode; node != NULL; node = node->mSiblingNextNode )
    {
        if ( VDirectorTrack *track = dynamic_cast<VDirectorTrack*>( node ) )
        {
            // Return Track.
            return track;
        }
    }

    // Invalid Track.
    return NULL;
}
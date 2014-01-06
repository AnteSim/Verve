//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Extension/Director/VDirectorTrack.h"

#include "math/mMathFn.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VDirectorTrack );
//-----------------------------------------------------------------------------

VDirectorTrack::VDirectorTrack( void )
{
    setLabel( "DirectorTrack" );
}

#ifdef VT_EDITOR
//-----------------------------------------------------------------------------
//
// Debug Methods.
//
//-----------------------------------------------------------------------------

ConsoleMethod( VDirectorTrack, updateTrack, void, 2, 2, "( void ) - Update the Track.\n"
                                                        "@return No return value."  )
{
    for ( ITreeNode *node = object->mChildNode; node != NULL; node = node->mSiblingNextNode )
    {
        VEvent *currEvent = ( VEvent* )node;
        VEvent *nextEvent = ( VEvent* )node->mSiblingNextNode;

        if ( !nextEvent )
        {
            // Update Duration.
            currEvent->setDuration( object->getControllerDuration() - currEvent->getTriggerTime() );
        }
        else
        {
            // Update Duration.
            currEvent->setDuration( mAbs( nextEvent->getTriggerTime() - currEvent->getTriggerTime() ) );
        }
    }
}
#endif
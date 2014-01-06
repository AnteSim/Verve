//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VObject.h"
#include "Verve/Core/VController.h"
#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VObject );
//-----------------------------------------------------------------------------

VObject::VObject( void ) : 
        mController( NULL ),
        mLabel( String::EmptyString ),
        mEnabled( true )
{
    // Void.
};

VObject::~VObject( void )
{
    // Remove.
    remove();
}

void VObject::initPersistFields( void )
{
    // Don't Use Parent Fields.
    // Parent::initPersistFields();

    addProtectedField( "Enabled", TypeBool,       Offset( mEnabled, VObject ), &setEnabled, &defaultProtectedGetFn, "Enable or Disable the object from playback." );
    addProtectedField( "Label",   TypeRealString, Offset( mLabel,   VObject ), &setLabel,   &defaultProtectedGetFn, "The label this object is referenced by." );
}

//-----------------------------------------------------------------------------
//
// Reference Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VObject::getObject( pLabel );
// 
// Returns the object with the given label. If no object belongs to this object
// with that label, then a NULL value is returned.
// 
//-----------------------------------------------------------------------------
VObject *VObject::getObject( const String &pLabel )
{
    VObject *node = ( VObject* )mChildNode;
    while ( node )
    {
        // Compare Names.
        if ( node->getLabel().equal( pLabel, String::NoCase ) )
        {
            // Valid.
            return node;
        }

        // Next Sibling.
        node = ( VObject* )node->mSiblingNextNode;
    }

    // Invalid.
    return NULL;
}

//-----------------------------------------------------------------------------
//
// Property Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VObject::isEnabled();
// 
// Returns whether this object is enabled.
// 
//-----------------------------------------------------------------------------
bool VObject::isEnabled( void )
{
    VObject *parent = dynamic_cast<VObject*>( getParent() );
    if ( parent && !parent->isEnabled() )
    {
        return false;
    }

    return mEnabled;
}

//-----------------------------------------------------------------------------
// 
// VObject::isControllerPlaying();
// 
// Returns whether the root controller is currently playing.
// 
//-----------------------------------------------------------------------------
bool VObject::isControllerPlaying( void )
{
    if ( getController() )
    {
        return getController()->isPlaying();
    }

    return false;
}

//-----------------------------------------------------------------------------
// 
// VObject::isControllerPaused();
// 
// Returns whether the root controller is currently paused.
// 
//-----------------------------------------------------------------------------
bool VObject::isControllerPaused( void )
{
    if ( getController() )
    {
        return getController()->isPaused();
    }

    return false;
}

//-----------------------------------------------------------------------------
// 
// VObject::isControllerStopped();
// 
// Returns whether the root controller is currently stopped.
// 
//-----------------------------------------------------------------------------
bool VObject::isControllerStopped( void )
{
    if ( getController() )
    {
        return getController()->isStopped();
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// VObject::isControllerPlayingForward();
// 
// Returns whether the root controller is currently playing forward.
// 
//-----------------------------------------------------------------------------
bool VObject::isControllerPlayingForward( void )
{
    if ( getController() )
    {
        return getController()->isPlayingForward();
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// VObject::isControllerLooping();
// 
// Returns whether the root controller is looping the sequence.
// 
//-----------------------------------------------------------------------------
bool VObject::isControllerLooping( void )
{
    if ( getController() )
    {
        return getController()->isLooping();
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// VObject::getControllerTime();
// 
// Returns the current time of the root controller.
// 
//-----------------------------------------------------------------------------
S32 VObject::getControllerTime( void )
{
    if ( getController() )
    {
        return getController()->getTime();
    }

    return 0;
}

//-----------------------------------------------------------------------------
// 
// VObject::getControllerTimeScale();
// 
// Returns the current timescale of the root controller.
// 
//-----------------------------------------------------------------------------
F32 VObject::getControllerTimeScale( void )
{
    if ( getController() )
    {
        return getController()->getTimeScale();
    }

    return 1.f;
}

//-----------------------------------------------------------------------------
// 
// VObject::getControllerDuration();
// 
// Returns the duration of the root controller.
// 
//-----------------------------------------------------------------------------
S32 VObject::getControllerDuration( void )
{
    if ( getController() )
    {
        return getController()->getDuration();
    }

    return 0;
}

//-----------------------------------------------------------------------------
// 
// VObject::setLabel( pLabel );
// 
// Set the label property.
// 
// If the project was built using the VT_EDITOR preprocessor argument, then the
// label will not be changed if the target name is already used in the parent
// object.
// 
//-----------------------------------------------------------------------------
void VObject::setLabel( const String &pLabel )
{
#ifdef VT_EDITOR
    if ( mParentNode )
    {
        // Empty Label?
        if ( mLabel.isEmpty() )
        {
            // Set Uniqu Label.
            setLabelUnique( pLabel );
            return;
        }

        for ( VObject *walk = ( VObject* )mChildNode; walk != NULL; walk = ( VObject* )walk->mSiblingNextNode )
        {
            if ( walk != this )
            {
                if ( pLabel == walk->getLabel() )
                {
                    // Exit.
                    return;
                }
            }
        }
    }
#endif

    // Set Label.
    mLabel = pLabel;
}

//-----------------------------------------------------------------------------
// 
// VObject::setLabelUnique( pLabel );
// 
// If the label that has been passed is already in use, then a new label will
// be generated by appending an index to the label. For example: MyLabel
// becomes MyLabel0 ... MyLabelN
//
//-----------------------------------------------------------------------------
void VObject::setLabelUnique( const String &pLabel )
{
    if ( mParentNode && pLabel.isNotEmpty() )
    {
        for ( VObject *walk = ( VObject* )mChildNode; walk != NULL; walk = ( VObject* )walk->mSiblingNextNode )
        {
            if ( walk != this )
            {
                if ( pLabel == walk->getLabel() )
                {
                    // Strip Trailing Number.
                    S32 i = -1;
                    String labelBase( String::GetTrailingNumber( pLabel, i ) );
                    i++;

                    // Construct New Name.
                    String labelBuffer = String::ToString( "%s%d", labelBase.c_str(), i );

                    // Set Name.
                    setLabelUnique( labelBuffer );

                    // Exit.
                    return;
                }
            }
        }
    }

    // Set Name.
    mLabel = pLabel;
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VObject::onAttach();
// 
// Callback made when this object is attached to another node.
// 
//-----------------------------------------------------------------------------
void VObject::onAttach( void )
{
    VTreeNode::onAttach();

    // Store Controller.
    mController = dynamic_cast<VController*>( getRoot() );

#ifdef VT_EDITOR
    if ( isProperlyAdded() )
    {
        Con::executef( this, "onAttach" );
    }
#endif
}

//-----------------------------------------------------------------------------
// 
// VObject::onDetach();
// 
// Callback made when this object is detached from a parent node.
// 
//-----------------------------------------------------------------------------
void VObject::onDetach( void )
{
    VTreeNode::onDetach();

    // Clear Controller.
    mController = NULL;

#ifdef VT_EDITOR
    if ( isProperlyAdded() )
    {
        Con::executef( this, "onDetach" );
    }
#endif
}

#ifdef VT_EDITOR
//-----------------------------------------------------------------------------
//
// Debug Methods.
//
//-----------------------------------------------------------------------------

ConsoleMethod( VObject, writeFile, bool, 3, 3, "( string pFileName ) - Save to a given filename.\n"
                                               "@param pFileName The target file to write to.\n"
                                               "@return Returns true if the write was successful." )
{
    // Write Target File.
    return VPersistence::writeFile( argv[2], object );
}

ConsoleMethod( VObject, readFile, bool, 3, 3, "( string pFileName ) - Clears the object and loads the new data from the given filename.\n"
                                              "@param pFileName The target file to read from.\n"
                                              "@return Returns true if the read was successful." )
{
    // Read Target File.
    return VPersistence::readFile( argv[2], object );
}

ConsoleMethod( VObject, getRoot, S32, 2, 2, "( void ) - Get the root object.\n"
                                            "@return Returns the SimObjectId for the root object."  )
{
    // Fetch Object.
    VObject *objectRef = ( VObject* )object->getRoot();

    // Return Object ID.
    return ( objectRef ) ? objectRef->getId() : 0;
}

ConsoleMethod( VObject, getParent, S32, 2, 2, "( void ) - Get the parent object.\n"
                                              "@return Returns the SimObjectId for the parent object." )
{
    // Fetch Object.
    VObject *objectRef = ( VObject* )object->mParentNode;

    // Return Object ID.
    return ( objectRef ) ? objectRef->getId() : 0;
}

ConsoleMethod( VObject, getIndex, S32, 2, 2, "( void ) - Get the index of this object relative to its siblings.\n"
                                             "@return Returns the index of this object." )
{
    return object->getIndex();
}

ConsoleMethod( VObject, getCount, S32, 2, 2, "( void ) - Get the number of child objects.\n"
                                             "@return Returns the number of child objects." )
{
    return object->size();
}

ConsoleMethod( VObject, getObject, S32, 3, 3, "( int pIndex ) - Get the object corresponding to the given index.\n"
                                              "@param pIndex The index of the object you wish to retrieve.\n"
                                              "@return Returns the SimObjectID for the object." )
{
    // Fetch Object.
    VObject *objectRef = ( VObject* )object->at( dAtoi( argv[2] ) );

    // Return Object ID.
    return ( objectRef ) ? objectRef->getId() : 0;
}

ConsoleMethod( VObject, clear, void, 2, 2, "( void ) - Detaches and deletes all of the child objects.\n"
                                           "@return No return value." )
{
    // Clear Sequence Lists.
    object->clear();
}

ConsoleMethod( VObject, addObject, void, 3, 3, "( SimObject pObject ) - Add a child object to this node.\n"
                                               "@param pObject The SimObjectID of the object to be added to this node.\n"
                                               "@return No return value." )
{
    VObject *child = dynamic_cast<VObject*>( Sim::findObject( argv[2] ) );
    if ( child )
    {
        child->addTo( object );
    }
}

ConsoleMethod( VObject, removeObject, void, 3, 3, "( SimObject pObject ) - Remove the target object from this node.\n"
                                                  "@param pObject The SimObjectID of the object to be removed from this node.\n"
                                                  "@return No return value." )
{
    VObject *child = dynamic_cast<VObject*>( Sim::findObject( argv[2] ) );
    if ( child && child->getParent() == object )
    {
        child->remove();
    }
}

ConsoleMethod( VObject, setLabelUnique, void, 3, 3, "( string pLabel ) - Force this label to be unique.\n"
                                                    "@param pLabel The name you wish to reference this object by.\n"
                                                    "@return No return value." )
{
    // Set Label.
    object->setLabelUnique( argv[2] );
}
#endif
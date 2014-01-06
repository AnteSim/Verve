//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_ITREENODE_H_
#define _VT_ITREENODE_H_

//-----------------------------------------------------------------------------

class ITreeNode
{
public:

    ITreeNode   *mParentNode;
    ITreeNode   *mChildNode;

    ITreeNode   *mSiblingPrevNode;
    ITreeNode   *mSiblingNextNode;

public:

    ITreeNode( void ) :
            mParentNode( 0 ),
            mChildNode( 0 ),
            mSiblingPrevNode( 0 ),
            mSiblingNextNode( 0 )
    {
        // Void.
    };

    virtual ~ITreeNode( void )
    {
        // Void.
    };

    virtual void               clear( void ) = 0;                              // Clear the Node.

    virtual ITreeNode         *getRoot( void ) = 0;                            // Get Root Node.
    virtual ITreeNode         *getParent( void ) = 0;                          // Get Parent Node.
    virtual ITreeNode         *getChild( void ) = 0;                           // Get Child Node.
    virtual ITreeNode         *getLastChild( void ) = 0;                       // Get Last Child Node.

    virtual ITreeNode         *getPrevSibling( void ) = 0;                     // Get Previous Sibling Node.
    virtual ITreeNode         *getNextSibling( void ) = 0;                     // Get Next Sibling Node.

    virtual void               addTo( ITreeNode *pNode ) = 0;                  // Add Node to target node.
    virtual void               remove( void ) = 0;                             // Remove this Node from the tree.
    virtual void               moveTo( ITreeNode* node ) = 0;                  // Move to specified Node.

    virtual void               onAttach( void ) = 0;                           // Attach Callback.
    virtual void               onDetach( void ) = 0;                           // Detach Callback.

    virtual bool               inTree( void ) = 0;                             // Is Node in a tree?
};

//-----------------------------------------------------------------------------

#endif // _VT_ITREENODE_H_
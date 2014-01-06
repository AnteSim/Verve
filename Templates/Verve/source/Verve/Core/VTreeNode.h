//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VTREENODE_H_
#define _VT_VTREENODE_H_

#ifndef _VT_ITREENODE_H_
#include "Verve/Core/ITreeNode.h"
#endif

//-----------------------------------------------------------------------------

class VTreeNode : public ITreeNode
{
    typedef ITreeNode Parent;

public:

    VTreeNode( void );
    ~VTreeNode( void );

    // Reference Methods.

    virtual void               clear( void );

    // ITreeNode Methods.

    virtual ITreeNode         *getRoot( void );
    virtual ITreeNode         *getParent( void );
    virtual ITreeNode         *getChild( void );
    virtual ITreeNode         *getLastChild( void );

    virtual ITreeNode         *getPrevSibling( void );
    virtual ITreeNode         *getNextSibling( void );

    virtual ITreeNode         *at( const int pIndex );
    virtual int                size( void );

    virtual int                getIndex( void );

    virtual void               addTo( ITreeNode *pNode );
    virtual void               addToFront( ITreeNode *pNode );
    virtual void               remove( void );
    virtual void               moveTo( ITreeNode *pNode );

    virtual void               onAttach( void );
    virtual void               onDetach( void );

    virtual bool               inTree( void );
};

//-----------------------------------------------------------------------------

#endif // _VT_VTREENODE_H_
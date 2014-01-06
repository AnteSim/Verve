//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VNETSTATE_H_
#define _VT_VNETSTATE_H_

#ifndef _NETCONNECTION_H_
#include "sim/netConnection.h"
#endif

#ifndef _TVECTOR_H_
#include "core/util/tVector.h"
#endif

//-----------------------------------------------------------------------------

struct VNetStateInfo
{
    SimObjectPtr<NetConnection>    Connection;
    U32                            Mask;

    VNetStateInfo( void ) :
        Connection( NULL ),
        Mask( 0 )
    {
        // Void.
    };

    VNetStateInfo( NetConnection *pConnection, U32 pMask )
    {
        Connection = pConnection;
        Mask       = pMask;
    };
};

//-----------------------------------------------------------------------------

class VNetState : public Vector<VNetStateInfo*>
{
protected:

    static U32 gInvalidMask;

    U32    mMask;

public:

    VNetState( void );
    virtual ~VNetState( void );

    // Connection Methods.

    bool             isConnection( NetConnection *pConnection );
    void             addConnection( NetConnection *pConnection );
    void             clearConnection( NetConnection *pConnection );

    // Property Methods.

    VNetStateInfo   *getState( NetConnection *pConnection );

    void             setMaskBits( const U32 &pMask );
    void             clearMaskBits( const U32 &pMask );
};

#endif // _VT_VNETSTATE_H_
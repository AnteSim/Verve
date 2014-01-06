//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VTORQUE3D_POSTEFFECT_H_
#define _VT_VTORQUE3D_POSTEFFECT_H_

#ifndef _VT_TORQUE_POSTEFFECT_H_
#include "Verve/Torque/TPostEffect.h"
#endif

#ifndef _NETCONNECTION_H_
#include "sim/netConnection.h"
#endif

class VPostEffectNetEvent : public NetEvent
{
    typedef NetEvent Parent;

public:

    StringTableEntry    mPostEffect;
    bool                mEnabled;

public:

    VPostEffectNetEvent( void );

    void            write( NetConnection *pConnection, BitStream *pStream );
    void            pack( NetConnection *pConnection, BitStream *pStream );
    void            unpack( NetConnection *pConnection, BitStream *pStream );
    void            process( NetConnection *pConnection );

    DECLARE_CONOBJECT( VPostEffectNetEvent );
};

#endif // _VT_VTORQUE3D_POSTEFFECT_H_
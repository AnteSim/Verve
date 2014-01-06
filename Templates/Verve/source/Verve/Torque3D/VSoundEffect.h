//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VTORQUE3D_SOUNDEFFECT_H_
#define _VT_VTORQUE3D_SOUNDEFFECT_H_

#ifndef _VT_TORQUE_SOUNDEFFECT_H_
#include "Verve/Torque/TSoundEffect.h"
#endif

#ifndef _NETCONNECTION_H_
#include "sim/netConnection.h"
#endif

class VSoundEffectNetEvent : public NetEvent
{
    typedef NetEvent Parent;

public:

    SFXProfile     *mProfile;

    F32             mPosition;
    F32             mPitch;

    bool            mIs3D;
    MatrixF         mTransform;

public:

    VSoundEffectNetEvent( void );

    void            write( NetConnection *pConnection, BitStream *pStream );
    void            pack( NetConnection *pConnection, BitStream *pStream );
    void            unpack( NetConnection *pConnection, BitStream *pStream );
    void            process( NetConnection *pConnection );

    DECLARE_CONOBJECT( VSoundEffectNetEvent );
};

#endif // _VT_VTORQUE3D_SOUNDEFFECT_H_
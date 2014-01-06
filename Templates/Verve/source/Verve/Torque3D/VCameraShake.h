//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VTORQUE3D_CAMERASHAKE_H_
#define _VT_VTORQUE3D_CAMERASHAKE_H_

#ifndef _VT_TORQUE_CAMERA_H_
#include "Verve/Torque/TCamera.h"
#endif

#ifndef _NETCONNECTION_H_
#include "sim/netConnection.h"
#endif

class VCameraShakeNetEvent : public NetEvent
{
    typedef NetEvent Parent;

public:

    enum eEventType
    {
        k_TypeClear = BIT( 0 ),
        k_TypeMake  = BIT( 1 ),
    };

    U32             mEventType;

    F32             mDuration;
    F32             mFalloff;
    VectorF         mAmplitude;
    VectorF         mFrequency;

public:

    VCameraShakeNetEvent( void );

    void            write( NetConnection *pConnection, BitStream *pStream );
    void            pack( NetConnection *pConnection, BitStream *pStream );
    void            unpack( NetConnection *pConnection, BitStream *pStream );
    void            process( NetConnection *pConnection );

    DECLARE_CONOBJECT( VCameraShakeNetEvent );
};

#endif // _VT_VTORQUE3D_CAMERASHAKE_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VCAMERASHAKEEVENT_H_
#define _VT_VCAMERASHAKEEVENT_H_

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VEvent.h"
#endif

//-----------------------------------------------------------------------------

class VCameraShakeEvent : public VEvent
{
    typedef VEvent Parent;

public:

    VectorF         mAmplitude;
    F32             mFalloff;
    VectorF         mFrequency;
    
public:

    VCameraShakeEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VCameraShakeEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VCAMERASHAKEEVENT_H_
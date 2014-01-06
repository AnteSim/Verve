//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSLOWMOEVENT_H_
#define _VT_VSLOWMOEVENT_H_

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VEvent.h"
#endif

//-----------------------------------------------------------------------------

class VSlowMoEvent : public VEvent
{
    typedef VEvent Parent;

public:

    F32    mTimeScale;
    F32    mTimeScaleTickDelta;
    
public:

    VSlowMoEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );
    virtual void    onUpdate( const S32 &pTime, const S32 &pDelta );
    virtual void    onComplete( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VSlowMoEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSLOWMOEVENT_H_
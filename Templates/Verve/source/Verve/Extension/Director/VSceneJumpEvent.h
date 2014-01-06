//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSCENEJUMPEVENT_H_
#define _VT_VSCENEJUMPEVENT_H_

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VEvent.h"
#endif

//-----------------------------------------------------------------------------

class VSceneJumpEvent : public VEvent
{
    typedef VEvent Parent;

public:

    String          mTarget;
    
public:

    VSceneJumpEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VSceneJumpEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSCENEJUMPEVENT_H_
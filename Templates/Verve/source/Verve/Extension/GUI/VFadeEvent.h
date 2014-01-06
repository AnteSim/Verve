//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VFADEEVENT_H_
#define _VT_VFADEEVENT_H_

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VEvent.h"
#endif

#ifndef _VT_VFADECONTROL_H_
#include "Verve/GUI/VFadeControl.h"
#endif

//-----------------------------------------------------------------------------

class VFadeEvent : public VEvent
{
    typedef VEvent Parent;

public:

    VFadeEvent( void );

    // Callback Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );
    virtual void    onComplete( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VFadeEvent );

public:

    VFadeControl::eFadeType    getFadeType( void );
};

//-----------------------------------------------------------------------------

#endif // _VT_VFADEEVENT_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VPOSTEFFECTTOGGLEEVENT_H_
#define _VT_VPOSTEFFECTTOGGLEEVENT_H_

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VEvent.h"
#endif

#ifndef _VT_TORQUE_POSTEFFECT_H_
#include "Verve/Torque/TPostEffect.h"
#endif

#ifndef _VT_VSHAREDENUM_H_
#include "Verve/Core/Util/VSharedEnum.h"
#endif

//-----------------------------------------------------------------------------

class VPostEffectToggleEvent : public VEvent
{
    typedef VEvent Parent;

public:

    VSharedEnum::eActionToggle    mEventType;

public:

    VPostEffectToggleEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VPostEffectToggleEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VPOSTEFFECTTOGGLEEVENT_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VLIGHTOBJECTTOGGLEEVENT_H_
#define _VT_VLIGHTOBJECTTOGGLEEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

#ifndef _VT_TORQUE_LIGHTOBJECT_H_
#include "Verve/Torque/TLightObject.h"
#endif

#ifndef _VT_VSHAREDENUM_H_
#include "Verve/Core/Util/VSharedEnum.h"
#endif

//-----------------------------------------------------------------------------

class VLightObjectToggleEvent : public VSceneObjectEvent
{
    typedef VEvent Parent;

public:

    VSharedEnum::eActionToggle    mEventType;

public:

    VLightObjectToggleEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VLightObjectToggleEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VLIGHTOBJECTTOGGLEEVENT_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VPARTICLEFFECTTOGGLEEVENT_H_
#define _VT_VPARTICLEFFECTTOGGLEEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

#ifndef _VT_TORQUE_PARTICLEEFFECT_H_
#include "Verve/Torque/TParticleEffect.h"
#endif

#ifndef _VT_VSHAREDENUM_H_
#include "Verve/Core/Util/VSharedEnum.h"
#endif

//-----------------------------------------------------------------------------

class VParticleEffectToggleEvent : public VSceneObjectEvent
{
    typedef VEvent Parent;

public:

    VSharedEnum::eActionToggle    mEventType;

public:

    VParticleEffectToggleEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VParticleEffectToggleEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VPARTICLEFFECTTOGGLEEVENT_H_
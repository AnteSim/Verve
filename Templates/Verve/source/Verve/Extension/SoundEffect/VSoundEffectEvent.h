//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSOUNDEFFECTEVENT_H_
#define _VT_VSOUNDEFFECTEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

#ifndef _VT_TORQUE_SOUNDEFFECT_H_
#include "Verve/Torque/TSoundEffect.h"
#endif

//-----------------------------------------------------------------------------

class VSoundEffectEvent : public VSceneObjectEvent
{
    typedef VSceneObjectEvent Parent;

    enum
    {
        SFXStartBuffer = 100,
    };

public:

    SimObjectPtr<VTorque::SoundEffectType>    mSoundEffect;

public:

    VSoundEffectEvent( void );

    static void     initPersistFields( void );

    static bool     setSoundData( void *pObject, const char *pArray, const char *pData );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VSoundEffectEvent );

public:

    // Property Methods.

    virtual void    setDuration( const S32 &pDuration );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSOUNDEFFECTEVENT_H_
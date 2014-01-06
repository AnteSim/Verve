//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TORQUE_SOUNDEFFECT_H_
#define _VT_TORQUE_SOUNDEFFECT_H_

#ifndef _SFXPROFILE_H_
#include "sfx/sfxProfile.h"
#endif

#ifndef _SFXSOUND_H_
#include "sfx/sfxSound.h"
#endif

#ifndef _VT_TORQUE_SCENEOBJECT_H_
#include "Verve/Torque/TSceneObject.h"
#endif

namespace VTorque
{
    typedef SFXProfile     SoundEffectType;
    typedef SFXSound       SoundSourceType;

    bool                   isSoundLooping( SoundEffectType *pSoundProfile );
    S32                    getSoundDuration( SoundEffectType *pSoundProfile );

    SoundSourceType       *playSound( SoundEffectType *pSoundProfile, const U32 &pPosition, const F32 &pPitch );
    SoundSourceType       *playSound( SoundEffectType *pSoundProfile, SceneObjectType *pObject, const U32 &pPosition, const F32 &pPitch );

    void                   playSound( SoundSourceType *pSource );
    void                   pauseSound( SoundSourceType *pSource );
    void                   stopSound( SoundSourceType *pSource );

    void                   setSoundPosition( SoundSourceType *pSource, const U32 &pPosition );
    void                   setSoundPitch( SoundSourceType *pSource, const F32 &pPitch );
};

#endif // _VT_TORQUE_SOUNDEFFECT_H_
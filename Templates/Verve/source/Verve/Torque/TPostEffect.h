//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TORQUE_POSTEFFECT_H_
#define _VT_TORQUE_POSTEFFECT_H_

#ifndef _POST_EFFECT_H_
#include "postFx/postEffect.h"
#endif

#ifndef _VT_TORQUE_SCENEOBJECT_H_
#include "Verve/Torque/TSceneObject.h"
#endif

namespace VTorque
{
    typedef PostEffect    PostEffectType;

    bool                  isPostEffectEnabled( PostEffectType *pPostEffect );
    void                  setPostEffectOn( PostEffectType *pPostEffect, const bool &pStatus );
};

#endif // _VT_TORQUE_POSTEFFECT_H_
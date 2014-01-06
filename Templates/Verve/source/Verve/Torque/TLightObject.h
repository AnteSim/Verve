//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_TORQUE_LIGHTOBJECT_H_
#define _VT_TORQUE_LIGHTOBJECT_H_

#ifndef _LIGHTBASE_H_
#include "T3D/lightBase.h"
#endif

#ifndef _LIGHTANIMDATA_H_
#include "T3D/lightAnimData.h"
#endif

namespace VTorque
{
    
    typedef LightBase        LightObjectType;
    typedef LightAnimData    LightAnimationDataType;

    bool                     isLightObjectEnabled( LightObjectType *pLightObject );
    void                     setLightObjectOn( LightObjectType *pLightObject, const bool &pStatus );

    void                     playAnimation( LightObjectType *pLightObject, LightAnimationDataType *pLightAnimationData );
    void                     playAnimation( LightObjectType *pLightObject );
    void                     pauseAnimation( LightObjectType *pLightObject );
};

#endif // _VT_TORQUE_LIGHTOBJECT_H_
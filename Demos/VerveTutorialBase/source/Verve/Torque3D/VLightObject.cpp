//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Torque/TLightObject.h"

//-----------------------------------------------------------------------------
//
// Light Object Methods.
//
//-----------------------------------------------------------------------------

bool VTorque::isLightObjectEnabled( LightObjectType *pLightObject )
{
    if ( !pLightObject )
    {
        // Sanity!
        return false;
    }

    // Get Enabled.
    return pLightObject->getLightEnabled();
}

void VTorque::setLightObjectOn( LightObjectType *pLightObject, const bool &pStatus )
{
    if ( !pLightObject )
    {
        // Sanity!
        return;
    }

    // Set Enabled.
    pLightObject->setLightEnabled( pStatus );
}

void VTorque::playAnimation( LightObjectType *pLightObject, LightAnimationDataType *pLightAnimationData )
{
    if ( !pLightObject || !pLightAnimationData )
    {
        // Sanity!
        return;
    }

    // Play Animation.
    pLightObject->playAnimation( pLightAnimationData );
}

void VTorque::playAnimation( LightObjectType *pLightObject )
{
    if ( !pLightObject )
    {
        // Sanity!
        return;
    }

    // Play Animation.
    pLightObject->playAnimation();
}

void VTorque::pauseAnimation( LightObjectType *pLightObject )
{
    if ( !pLightObject )
    {
        // Sanity!
        return;
    }

    // Play Animation.
    pLightObject->pauseAnimation();
}
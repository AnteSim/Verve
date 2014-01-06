//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/GUI/VFadeControl.h"

#include "console/consoleTypes.h"
#include "gfx/gfxDrawUtil.h"
#include "math/mMathFn.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VFadeControl );
//-----------------------------------------------------------------------------

VFadeControl::VFadeControl( void ) : 
        mActive( false ),
        mFadeType( k_TypeInvalid ),
        mElapsedTime( 0 ),
        mDuration( 1000 ),
        mLastTime( 0 )
{
    // Void.
}

//-----------------------------------------------------------------------------
//
// Render Methods.
//
//-----------------------------------------------------------------------------

void VFadeControl::onRender( Point2I pOffset, const RectI &pUpdateRect )
{
    Parent::onRender( pOffset, pUpdateRect );

    if ( mFadeType == k_TypeInvalid )
    {
        // Invalid Fade State.
        return;
    }

    // Fetch Time.
    const U32 time  = Platform::getRealMilliseconds();
    // Fetch Delta.
    const U32 delta = ( time - mLastTime );
    // Store Time.
    mLastTime       = time;

    if ( mActive )
    {
        // Update Elapsed Time.
        mElapsedTime += delta;
    }

    F32 alpha = 1.f - mClampF( F32( mElapsedTime ) / F32( mDuration ), 0.f, 1.f );

    if ( mFadeType == k_TypeOut )
    {
        // Flip.
        alpha = 1.f - alpha;
    }

    if ( alpha > 0.f )
    {
        // Render.
        GFX->getDrawUtil()->drawRectFill( pOffset, pOffset + getExtent(), ColorF( 0, 0, 0, alpha ) );
    }

    if ( mElapsedTime >= mDuration )
    {
        // Stop.
        mActive = false;
    }
}

//-----------------------------------------------------------------------------
//
// Control Methods.
//
//-----------------------------------------------------------------------------

void VFadeControl::start( eFadeType pType, S32 pDuration )
{
    mActive      = true;

    mFadeType    = pType;

    mElapsedTime = 0;
    mDuration    = pDuration;

    mLastTime    = Platform::getRealMilliseconds();
}

void VFadeControl::pause( void )
{
    mActive = false;
}
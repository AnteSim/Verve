//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/GUI/VEditorScrollControl.h"
#include "gfx/gfxDrawUtil.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VEditorScrollControl );
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Mouse Methods.
//
//-----------------------------------------------------------------------------

void VEditorScrollControl::onMouseUp( const GuiEvent &pEvent )
{
    Parent::onMouseUp( pEvent );

    // Event.
    onMouseEvent( "onMouseUp", pEvent );
}

void VEditorScrollControl::onRightMouseUp( const GuiEvent &pEvent )
{
    Parent::onMouseUp( pEvent );

    // Event.
    onMouseEvent( "onRightMouseUp", pEvent );
}

void VEditorScrollControl::onMouseEvent( const char *pEventName, const GuiEvent &pEvent )
{
    const S32 offsetX = ( mHasVScrollBar ) ? mScrollBarThickness : 0;
    const S32 offsetY = ( mHasHScrollBar ) ? mScrollBarThickness : 0;

    const RectI contentRect( 2, 2, getWidth() - offsetX - 4 - 1, getHeight() - offsetY - 4 - ( mHasHScrollBar ) );
    if ( !contentRect.pointInRect( pEvent.mousePoint ) )
    {
        // Return!
        return;
    }

    // Argument Buffers.
    char argBuffer[3][32];

    // Format Event-Position Buffer.
    dSprintf( argBuffer[0], 32, "%d %d", pEvent.mousePoint.x, pEvent.mousePoint.y );

    // Format Event-Modifier Buffer.
    dSprintf( argBuffer[1], 32, "%d", pEvent.modifier );

    // Format Mouse-Click Count Buffer.
    dSprintf( argBuffer[2], 32, "%d", pEvent.mouseClickCount );

    // Call Scripts.
    Con::executef( this, pEventName, argBuffer[0], argBuffer[1], argBuffer[2] );
}

//-----------------------------------------------------------------------------
//
// Render Methods.
//
//-----------------------------------------------------------------------------

void VEditorScrollControl::onRender( Point2I pOffset, const RectI &pUpdateRect )
{
    Parent::onRender( pOffset, pUpdateRect );

    const S32 offsetX = ( mHasVScrollBar ) ? mScrollBarThickness : 1;
    const S32 offsetY = ( mHasHScrollBar ) ? mScrollBarThickness : 1;

    RectI contentRect( pOffset.x + 1, pOffset.y + 1, getWidth() - offsetX - 1, getHeight() - offsetY - 1 );
    contentRect.intersect( pUpdateRect );

    GFX->getDrawUtil()->drawRect( contentRect, mProfile->mBorderColor );
}
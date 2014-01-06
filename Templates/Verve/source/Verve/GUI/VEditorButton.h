//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VEDITORBUTTON_H_
#define _VT_VEDITORBUTTON_H_

#ifndef _GUIBITMAPBUTTON_H_
#include "gui/buttons/guiBitmapButtonCtrl.h"
#endif

class VEditorButton : public GuiBitmapButtonTextCtrl
{
    typedef GuiBitmapButtonTextCtrl Parent;

public:

    bool            mIsDraggable;

public:

    VEditorButton();

    static void     initPersistFields( void );

    void            onMouseDown( const GuiEvent &pEvent );
    void            onMouseUp( const GuiEvent &pEvent );
    void            onMouseDragged( const GuiEvent &pEvent );

    void            onRightMouseDown( const GuiEvent &pEvent );
    void            onRightMouseUp( const GuiEvent &pEvent );

    void            onMouseEnter( const GuiEvent &pEvent );
    void            onMouseLeave( const GuiEvent &pEvent );
    void            onMouseEvent( const char *pEventName, const GuiEvent &pEvent );

    void            onRender( Point2I offset, const RectI &updateRect );

public:

    DECLARE_CONOBJECT( VEditorButton );
};

#endif //_VT_VEDITORBUTTON_H_
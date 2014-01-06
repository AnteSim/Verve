//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VEDITORSCROLLCONTROL_H_
#define _VT_VEDITORSCROLLCONTROL_H_

#ifndef _GUISCROLLCTRL_H_
#include "gui/containers/guiScrollCtrl.h"
#endif

//-----------------------------------------------------------------------------

class VEditorScrollControl : public GuiScrollCtrl
{
    typedef GuiScrollCtrl Parent;
    
public:

    // Mouse.

    virtual void    onMouseUp( const GuiEvent &pEvent );
    virtual void    onRightMouseUp( const GuiEvent &pEvent );

    void            onMouseEvent( const char *pEventName, const GuiEvent &pEvent );

    // Rendering.

    void            onRender( Point2I pOffset, const RectI &pUpdateRect );

    // Console Declaration.

    DECLARE_CONOBJECT( VEditorScrollControl );
};

//-----------------------------------------------------------------------------

#endif // _VT_VEDITORSCROLLCONTROL_H_
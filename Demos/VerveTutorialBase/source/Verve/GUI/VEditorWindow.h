//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VEDITORWINDOW_H_
#define _VT_VEDITORWINDOW_H_

#ifndef _GUICANVAS_H_
#include "gui/core/guiCanvas.h"
#endif

//-----------------------------------------------------------------------------

class VEditorWindow : public GuiCanvas
{
    typedef GuiCanvas Parent;
    
public:

    // Properties.

    virtual bool    onAdd( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VEditorWindow );
};

//-----------------------------------------------------------------------------

#endif // _VT_VEDITORWINDOW_H_
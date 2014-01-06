//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VTIMELINECONTROL_H_
#define _VT_VTIMELINECONTROL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

#ifndef _VT_VCONTROLLER_H_
#include "Verve/Core/VController.h"
#endif

//-----------------------------------------------------------------------------

class VTimeLineControl : public GuiControl
{
    typedef GuiControl Parent;

public:

    struct sSelection
    {
        bool   Active;
        S32    StartTime;
        S32    EndTime;
    };

    bool            mIsController;
    VController    *mController;

    S32             mDurationOffset;

    sSelection      mSelection;
    
public:

    VTimeLineControl( void );

    static void     initPersistFields( void );

    // Mouse.

    virtual void    onMouseDown( const GuiEvent &pEvent );
    virtual void    onMouseUp( const GuiEvent &pEvent );
    virtual void    onMouseDragged( const GuiEvent &pEvent );

    virtual void    onRightMouseDown( const GuiEvent &pEvent );

    void            onMouseEvent( const char *pEventName, const GuiEvent &pEvent );

    // Rendering.

    void            onPreRender( void );
    void            onRender( Point2I offset, const RectI &updateRect );

    // Console Declaration.

    DECLARE_CONOBJECT( VTimeLineControl );

public:

    S32             toTime( const S32 &pPoint );
    S32             toPoint( const S32 &pTime );

    void            updateDuration( void );
};

//-----------------------------------------------------------------------------

#endif // _VT_VTIMELINECONTROL_H_
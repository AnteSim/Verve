//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VFADECONTROL_H_
#define _VT_VFADECONTROL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

//-----------------------------------------------------------------------------

class VFadeControl : public GuiControl
{
    typedef GuiControl Parent;

public:

    enum eFadeType
    {
        k_TypeIn,
        k_TypeOut,

        k_TypeInvalid,
    };

    bool            mActive;
    eFadeType       mFadeType;

    S32             mElapsedTime;
    S32             mDuration;
    S32             mLastTime;
    
public:

    VFadeControl( void );

    // Render Methods.

    virtual void    onRender( Point2I pOffset, const RectI &pUpdateRect );

    // Console Declaration.

    DECLARE_CONOBJECT( VFadeControl );

public:

    void            start( eFadeType pType, S32 pDuration );
    void            pause( void );
};

//-----------------------------------------------------------------------------

#endif // _VT_VFADECONTROL_H_
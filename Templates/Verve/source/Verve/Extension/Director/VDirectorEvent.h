//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VDIRECTOREVENT_H_
#define _VT_VDIRECTOREVENT_H_

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VEvent.h"
#endif

#ifndef _VT_VCAMERAGROUP_H_
#include "Verve/Extension/Camera/VCameraGroup.h"
#endif

//-----------------------------------------------------------------------------

class VDirectorEvent : public VEvent
{
    typedef VEvent Parent;

public:

    String          mTarget;
    
public:

    VDirectorEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VDirectorEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VDIRECTOREVENT_H_
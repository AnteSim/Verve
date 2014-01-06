//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSCRIPTEVENTTRACK_H_
#define _VT_VSCRIPTEVENTTRACK_H_

#ifndef _VT_VTRACK_H_
#include "Verve/Core/VTrack.h"
#endif

//-----------------------------------------------------------------------------

class VScriptEventTrack : public VTrack
{
    typedef VTrack Parent;

public:

    VScriptEventTrack( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VScriptEventTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSCRIPTEVENTTRACK_H_
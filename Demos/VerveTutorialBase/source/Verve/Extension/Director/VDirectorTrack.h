//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VDIRECTORTRACK_H_
#define _VT_VDIRECTORTRACK_H_

#ifndef _VT_VTRACK_H_
#include "Verve/Core/VTrack.h"
#endif

//-----------------------------------------------------------------------------

class VDirectorTrack : public VTrack
{
    typedef VTrack Parent;

public:

    VDirectorTrack( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VDirectorTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VDIRECTORTRACK_H_
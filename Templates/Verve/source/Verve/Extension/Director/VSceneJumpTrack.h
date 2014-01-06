//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSCENEJUMPTRACK_H_
#define _VT_VSCENEJUMPTRACK_H_

#ifndef _VT_VTRACK_H_
#include "Verve/Core/VTrack.h"
#endif

//-----------------------------------------------------------------------------

class VSceneJumpTrack : public VTrack
{
    typedef VTrack Parent;

public:

    VSceneJumpTrack( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VSceneJumpTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSCENEJUMPTRACK_H_
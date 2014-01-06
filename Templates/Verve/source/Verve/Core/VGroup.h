//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VGROUP_H_
#define _VT_VGROUP_H_

#ifndef _VT_VOBJECT_H_
#include "Verve/Core/VObject.h"
#endif

//-----------------------------------------------------------------------------

class VGroup : public VObject
{
    typedef VObject Parent;

public:

    VGroup( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VGroup );
};

//-----------------------------------------------------------------------------

#endif // _VT_VGROUP_H_
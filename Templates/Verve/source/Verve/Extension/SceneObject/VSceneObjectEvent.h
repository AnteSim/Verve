//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSCENEOBJECTEVENT_H_
#define _VT_VSCENEOBJECTEVENT_H_

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VTrack.h"
#endif

#ifndef _VT_VSCENEOBJECTGROUP_H_
#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#endif

//-----------------------------------------------------------------------------

class VSceneObjectEvent : public VEvent
{
    typedef VEvent Parent;

public:

    VSceneObjectEvent( void );

    // Reference Methods.

    VTorque::SceneObjectType   *getSceneObject( void );
    template <class T> inline bool getSceneObject( T *&pSceneObject )
    {
        // Reference Scene Object.
        pSceneObject = dynamic_cast<T*>( getSceneObject() );

        // Valid?
        return ( pSceneObject != NULL );
    }

    // Console Declaration.

    DECLARE_CONOBJECT( VSceneObjectEvent );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSCENEOBJECTEVENT_H_
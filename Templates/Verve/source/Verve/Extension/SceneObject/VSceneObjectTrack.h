//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSCENEOBJECTTRACK_H_
#define _VT_VSCENEOBJECTTRACK_H_

#ifndef _VT_VTRACK_H_
#include "Verve/Core/VTrack.h"
#endif

#ifndef _VT_VSCENEOBJECTGROUP_H_
#include "Verve/Extension/SceneObject/VSceneObjectGroup.h"
#endif

//-----------------------------------------------------------------------------

class VSceneObjectTrack : public VTrack
{
    typedef VTrack Parent;

public:

    VSceneObjectTrack( void );

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

    DECLARE_CONOBJECT( VSceneObjectTrack );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSCENEOBJECTTRACK_H_
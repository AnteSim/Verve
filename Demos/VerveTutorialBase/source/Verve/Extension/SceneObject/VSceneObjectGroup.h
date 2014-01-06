//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSCENEOBJECTGROUP_H_
#define _VT_VSCENEOBJECTGROUP_H_

#ifndef _VT_VGROUP_H_
#include "Verve/Core/VGroup.h"
#endif

#ifndef _VT_TORQUE_SCENEOBJECT_H_
#include "Verve/Torque/TSceneObject.h"
#endif

//-----------------------------------------------------------------------------

class VSceneObjectGroup : public VGroup
{
    typedef VGroup Parent;

public:

    // Reference Members.

    String                      mDataReference;
    VTorque::SceneObjectType   *mSceneObject;

public:

    VSceneObjectGroup( void );

    static void                         initPersistFields( void );

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

    DECLARE_CONOBJECT( VSceneObjectGroup );
};

//-----------------------------------------------------------------------------

#endif // _VT_VSCENEOBJECTGROUP_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VSCRIPTEVENT_H_
#define _VT_VSCRIPTEVENT_H_

#ifndef _VT_VSCENEOBJECTEVENT_H_
#include "Verve/Extension/SceneObject/VSceneObjectEvent.h"
#endif

//-----------------------------------------------------------------------------

class VScriptEvent : public VSceneObjectEvent
{
    typedef VSceneObjectEvent Parent;

public:

    enum eCommandType
    {
        k_TypeExpression,
        k_TypeMethod,

        k_TypeInvalid,
    };

    eCommandType        mCommandType;
    String              mCommand;
    
public:

    VScriptEvent( void );

    static void     initPersistFields( void );

    // Event Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VScriptEvent );
};

//-----------------------------------------------------------------------------

// Define Types.
typedef VScriptEvent::eCommandType VScriptEventCommandType;

// Declare Enum Types.
DefineEnumType( VScriptEventCommandType );

//-----------------------------------------------------------------------------

#endif // _VT_VSCRIPTEVENT_H_
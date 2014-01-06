//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VTRACK_H_
#define _VT_VTRACK_H_

#ifndef _VT_VCONTROLLER_H_
#include "Verve/Core/VController.h"
#endif

#ifndef _VT_VEVENT_H_
#include "Verve/Core/VEvent.h"
#endif

#ifndef _VT_TORQUE_SCENEOBJECT_H_
#include "Verve/Torque/TSceneObject.h"
#endif

//-----------------------------------------------------------------------------
class VGroup;
//-----------------------------------------------------------------------------

class VTrack : public VObject
{
    typedef VObject Parent;

public:

    // Controller Members.

    VEvent         *mNextEvent;

public:

    VTrack();

    // Tree Methods.

    virtual void    onAttach( void );
    virtual void    onDetach( void );

    // Controller Methods.

    virtual void    onControllerUpdate( const S32 &pTime, const S32 &pDelta );
    virtual bool    onControllerEvent( VController::eControllerEventType pEvent );

    virtual void    onControllerReset( const S32 &pTime, const bool &pForward );

    // Reference Methods.

    void            sort( void );
    bool            updateNextEvent( void );

    // Console Declaration.

    DECLARE_CONOBJECT( VTrack );

public:

    // Property Methods.

    VGroup   *getGroup( void );
    template <class T> inline bool getGroup( T *&pGroup )
    {
        // Reference Group.
        pGroup = dynamic_cast<T*>( getGroup() );
        // Validate.
        return ( pGroup != NULL );
    }

    VEvent   *getNextEvent( void );
    template <class T> inline bool getNextEvent( T *&pEvent )
    {
        // Reference Object.
        pEvent = dynamic_cast<T*>( getNextEvent() );
        // Validate.
        return ( pEvent != NULL );
    }

    VEvent   *getCurrentEvent( void );
    template <class T> inline bool getCurrentEvent( T *&pEvent )
    {
        // Reference Object.
        pEvent = dynamic_cast<T*>( getCurrentEvent() );
        // Validate.
        return ( pEvent != NULL );
    }

    VEvent   *getPreviousEvent( void );
    template <class T> inline bool getPreviousEvent( T *&pEvent )
    {
        // Reference Object.
        pEvent = dynamic_cast<T*>( getPreviousEvent() );
        // Validate.
        return ( pEvent != NULL );
    }

    F32             calculateInterp( S32 pTime );
    F32            _calculateInterp( S32 pTime );
};

//-----------------------------------------------------------------------------

#endif // _VT_VTRACK_H_
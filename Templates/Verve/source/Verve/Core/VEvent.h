//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VEVENT_H_
#define _VT_VEVENT_H_

#ifndef _VT_VOBJECT_H_
#include "Verve/Core/VObject.h"
#endif

//-----------------------------------------------------------------------------
class VGroup;
class VTrack;
//-----------------------------------------------------------------------------

class VEvent : public VObject
{
    typedef VObject Parent;

protected:

    bool            mIsPlaying;
    bool            mTriggered;

    S32             mTriggerTime;
    S32             mDuration;
    
public:

    VEvent( void );

    static  void    initPersistFields( void );

    // Controller Methods.

    virtual void    onControllerReset( const S32 &pTime, const bool &pForward );
    virtual bool    onControllerUpdate( const S32 &pTime, const S32 &pDelta );

    // Callback Methods.

    virtual void    onTrigger( const S32 &pTime, const S32 &pDelta );
    virtual void    onUpdate( const S32 &pTime, const S32 &pDelta );
    virtual void    onComplete( const S32 &pTime, const S32 &pDelta );

    // Console Declaration.

    DECLARE_CONOBJECT( VEvent );

public:

    // Property Methods.

    VGroup         *getGroup( void );
    template <class T> inline bool getGroup( T *&pGroup )
    {
        // Reference Group.
        pGroup = dynamic_cast<T*>( getGroup() );
        // Validate.
        return ( pGroup != NULL );
    }

    VTrack         *getTrack( void );
    template <class T> inline bool getTrack( T *&pTrack )
    {
        // Reference Track.
        pTrack = dynamic_cast<T*>( getTrack() );
        // Validate.
        return ( pTrack != NULL );
    }

    VEvent         *getNextEvent( void );
    VEvent         *getPreviousEvent( void );

    inline bool     isPlaying( void )      { return mIsPlaying;   };
    inline S32      getTriggerTime( void ) { return mTriggerTime; };
    inline S32      getDuration( void )    { return mDuration;    };

    virtual S32     getStartTime( void );
    virtual S32     getFinishTime( void );

    virtual void    setTriggerTime( const S32 &pTime );
    virtual void    setDuration( const S32 &pDuration );

    static  bool    setTriggerTime( void *obj, const char *pArray, const char *data ) { static_cast<VEvent*>( obj )->setTriggerTime( dAtoi( data ) ); return false; };
    static  bool    setDuration( void *obj, const char *pArray, const char *data )    { static_cast<VEvent*>( obj )->setDuration( dAtoi( data ) );    return false; };
};

//-----------------------------------------------------------------------------

#endif // _VT_VEVENT_H_
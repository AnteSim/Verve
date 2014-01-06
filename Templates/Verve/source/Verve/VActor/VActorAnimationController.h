//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VACTORANIMATIONCONTROLLER_H_
#define _VT_VACTORANIMATIONCONTROLLER_H_

#ifndef _VT_TYPES_H_
#include "Types/VTypes.h"
#endif

#ifndef _VT_VACTORSTATETABLE_H_
#include "VActorStateTable.h"
#endif

#ifndef _TSSHAPEINSTANCE_H_
#include "ts/tsShapeInstance.h"
#endif

//-----------------------------------------------------------------------------

class VActor;
class VActorStateTable;

//-----------------------------------------------------------------------------

class VActorAnimationController
{
public:

    struct sAnimationRef
    {
        U32           Index;
        TSThread     *Thread;

        sAnimationRef( void ) :
            Index( U32_MAX ),
            Thread( NULL )
        {
            // Void.
        }
    };

protected:

    VActor             *mObject;

    VActorStateTable    mAnimationTable;
    sAnimationRef       mBaseAnimation;

public:

    VActorAnimationController( void );
    virtual ~VActorAnimationController( void );

    // Initialisation Methods.

    bool               initAnimationTable( void );
    bool               initAnimation( sAnimationRef &pAnimation, const U32 &pIndex, const F32 &pPosition, const F32 &pTimeScale );
    bool               initAnimationThread( TSThread *&pThread, const U32 &pIndex, const F32 &pPosition, const F32 &pTimeScale );

    bool               initBaseAnimation( const U32 &pIndex, const F32 &pPosition, const F32 &pTimeScale );

    // Accessor Methods.

    bool               isValidObject( void );
    VActor            *getObject( void );
    void               setObject( VActor *pObject );

    const TSShape     *getShape( void );
    TSShapeInstance   *getShapeInstance( void );

    // Animation Methods.

    virtual void       update( const F32 &pDelta );

    bool               isAnimationSequence( const U32 &pIndex );
    S32                getAnimationSequence( const U32 &pIndex );

    U32                getAnimation( void );
    void               setAnimation( const U32 &pIndex );
};

#endif // _VT_VACTORANIMATIONCONTROLLER_H_
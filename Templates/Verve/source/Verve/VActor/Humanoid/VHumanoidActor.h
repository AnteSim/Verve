//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VHUMANOIDACTOR_H_
#define _VT_VHUMANOIDACTOR_H_

#ifndef _VT_VACTOR_H_
#include "../VActor.h"
#endif

#ifndef _VT_VHUMANOIDACTORDATA_H_
#include "VHumanoidActorData.h"
#endif

#ifndef _VT_VHUMANOIDACTORANIMATIONCONTROLLER_H_
#include "VHumanoidActorAnimationController.h"
#endif

#ifndef _VT_VHUMANOIDACTORPHYSICSCONTROLLER_H_
#include "VHumanoidActorPhysicsController.h"
#endif

//-----------------------------------------------------------------------------

class VHumanoidActor : public VActor
{
    typedef VActor Parent;

protected:

    VHumanoidActorAnimationController   mAnimationController;
    VHumanoidActorPhysicsController     mPhysicsController;

public:

    VHumanoidActor( void );
    ~VHumanoidActor( void );

    // Initialisation Methods.

    bool            onNewDataBlock( GameBaseData *pDataBlock, bool pReload );

    // Update Methods.

    void            processTick( const Move *pMove );
    void            interpolateTick( F32 pDelta );
    void            advanceTime( F32 pDelta );

    U32             packUpdate( NetConnection *pConnection, U32 pMask, BitStream *pStream );
    void            unpackUpdate( NetConnection *pConnection, BitStream *pStream );

    // Animation Methods.

    bool                         initAnimationController( void );
    VActorAnimationController   *getAnimationController( void );

    // Physics Methods.

    bool                         initPhysicsController( void );
    VActorPhysicsController     *getPhysicsController( void );

    DECLARE_CONOBJECT( VHumanoidActor );
};

#endif // _VT_VHUMANOIDACTOR_H_
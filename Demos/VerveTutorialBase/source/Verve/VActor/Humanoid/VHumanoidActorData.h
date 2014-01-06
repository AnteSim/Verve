//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VHUMANOIDACTORDATA_H_
#define _VT_VHUMANOIDACTORDATA_H_

#ifndef _VT_VACTORDATA_H_
#include "../VActorData.h"
#endif

//-----------------------------------------------------------------------------

struct VHumanoidActorData : public VActorData
{
private:

    typedef VActorData Parent;
    friend class VHumanoidActor;

public:

    enum eAnimationList
    {
        k_IdleAnimation           = Parent::k_NextAnimation +  0,

        k_WalkForwardAnimation    = Parent::k_NextAnimation +  1,
        k_WalkBackwardAnimation   = Parent::k_NextAnimation +  2,

        k_RunForwardAnimation     = Parent::k_NextAnimation +  3,
        k_RunBackwardAnimation    = Parent::k_NextAnimation +  4,

        k_SwimIdleAnimation       = Parent::k_NextAnimation +  5,
        k_SwimForwardAnimation    = Parent::k_NextAnimation +  6,
        k_SwimBackwardAnimation   = Parent::k_NextAnimation +  7,

        k_HeadHorizontalAnimation = Parent::k_NextAnimation +  8,
        k_HeadVerticalAnimation   = Parent::k_NextAnimation +  9,

        k_ArmsUpDownAnimation     = Parent::k_NextAnimation + 10,

        k_NextAnimation           = Parent::k_NextAnimation + 11,
    };

    enum ePhysicsStateList
    {
        k_OnGroundPhysics         = Parent::k_NextPhysicsState + 0,
        k_InAirPhysics            = Parent::k_NextPhysicsState + 1,
        k_InWaterPhysics          = Parent::k_NextPhysicsState + 2,

        k_NextPhysicsState        = Parent::k_NextPhysicsState + 3,
    };

public:

    VHumanoidActorData( void );

    virtual bool    preload( bool pServer, String &pErrorStr );

    DECLARE_CONOBJECT( VHumanoidActorData );
};

#endif // _VT_VHUMANOIDACTORDATA_H_
//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VHUMANOIDACTORPHYSICSCONTROLLER_H_
#define _VT_VHUMANOIDACTORPHYSICSCONTROLLER_H_

#ifndef _VT_VACTORPHYSICSCONTROLLER_H_
#include "../VActorPhysicsController.h"
#endif

//-----------------------------------------------------------------------------

class VActor;
class VActorStateTable;

//-----------------------------------------------------------------------------

class VHumanoidActorPhysicsController : public VActorPhysicsController
{
public:

    VHumanoidActorPhysicsController( void );
    ~VHumanoidActorPhysicsController( void );
};

#endif // _VT_VHUMANOIDACTORPHYSICSCONTROLLER_H_
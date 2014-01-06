//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VHUMANOIDANIMATIONSTATES_H_
#define _VT_VHUMANOIDANIMATIONSTATES_H_

#ifndef _VT_VACTORANIMATIONSTATES_H_
#include "../VActorAnimationStates.h"
#endif

//-----------------------------------------------------------------------------

DeclareActorAnimationState( HumanoidIdle );

DeclareActorAnimationState( HumanoidWalkForward );
DeclareActorAnimationState( HumanoidWalkBackward );

DeclareActorAnimationState( HumanoidRunForward );
DeclareActorAnimationState( HumanoidRunBackward );

DeclareActorAnimationState( HumanoidSwimIdle );
DeclareActorAnimationState( HumanoidSwimForward );
DeclareActorAnimationState( HumanoidSwimBackward );

//-----------------------------------------------------------------------------

#endif // _VT_VHUMANOIDANIMATIONSTATES_H_
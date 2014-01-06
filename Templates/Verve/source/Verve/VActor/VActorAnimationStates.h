//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VACTORANIMATIONSTATES_H_
#define _VT_VACTORANIMATIONSTATES_H_

#ifndef _VT_VACTORSTATETABLE_H_
#include "VActorStateTable.h"
#endif

#ifndef _TSINGLETON_H_
#include "core/util/tSingleton.h"
#endif

//-----------------------------------------------------------------------------

class VActorAnimationState : public VActorState
{
public:

    virtual void    exit( VActor *pObject ) {};
};

//-----------------------------------------------------------------------------

#define DeclareActorAnimationState( name )                                    \
    class VActor##name##AnimationState : public VActorAnimationState          \
    {                                                                         \
    public:                                                                   \
        void            enter( VActor *pObject );                    \
        bool            execute( VActor *pObject );                  \
    }

#define ActorAnimationStateInstance( name )                                   \
    Singleton<VActor##name##AnimationState>::instance()

#define ImplementActorAnimationState( name, sequence )                        \
    void VActor##name##AnimationState::enter( VActor *pObject ) { pObject->getAnimationController()->setAnimation( sequence ); }

#define ExecuteActorAnimationState( name )                                    \
    bool VActor##name##AnimationState::execute( VActor *pObject )

//-----------------------------------------------------------------------------

#endif // _VT_VACTORANIMATIONSTATES_H_
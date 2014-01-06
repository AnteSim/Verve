//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VACTORPHYSICSSTATES_H_
#define _VT_VACTORPHYSICSSTATES_H_

#ifndef _VT_VACTORSTATETABLE_H_
#include "VActorStateTable.h"
#endif

#ifndef _TSINGLETON_H_
#include "core/util/tSingleton.h"
#endif

//-----------------------------------------------------------------------------
struct Move;
//-----------------------------------------------------------------------------

class VActorPhysicsState : public VActorState
{
public:

    virtual void    exit( VActor *pObject ) {};

    virtual void    processTick( VActor *pObject, const F32 &pElapsedTime, const Move *pMove ) = 0;
};

//-----------------------------------------------------------------------------

#define DeclareActorPhysicsState( name )                                         \
    class VActor##name##PhysicsState : public VActorPhysicsState                 \
    {                                                                            \
    public:                                                                      \
        void            enter( VActor *pObject );                                \
        bool            execute( VActor *pObject );                              \
        void            processTick( VActor *pObject, const F32 &pElapsedTime, const Move *pMove ); \
    }

#define ActorPhysicsStateInstance( name )                                        \
    Singleton<VActor##name##PhysicsState>::instance()

#define ImplementActorPhysicsState( name, state )                                \
    void VActor##name##PhysicsState::enter( VActor *pObject ) { pObject->getPhysicsController()->setPhysicsState( state ); }

#define ExecuteActorPhysicsState( name )                                         \
    bool VActor##name##PhysicsState::execute( VActor *pObject )

#define ProcessActorPhysicsState( name )                                         \
    void VActor##name##PhysicsState::processTick( VActor *pObject, const F32 &pElapsedTime, const Move *pMove )

//-----------------------------------------------------------------------------

#endif // _VT_VACTORPHYSICSSTATES_H_
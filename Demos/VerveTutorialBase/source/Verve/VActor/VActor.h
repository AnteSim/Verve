//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VACTOR_H_
#define _VT_VACTOR_H_

#ifndef _VT_VACTORDATA_H_
#include "VActorData.h"
#endif

#ifndef _VT_TYPES_H_
#include "Types/VTypes.h"
#endif

//-----------------------------------------------------------------------------
class VActorAnimationController;
class VActorPhysicsController;
//-----------------------------------------------------------------------------

class VActor : public ShapeBase
{
    typedef ShapeBase Parent;

public:

    enum eMaskBits
    {
        // Physics Bits.
        MoveMask     = Parent::NextFreeMask << 0,
        PhysicsMask  = ( MoveMask ),

        NextFreeMask = Parent::NextFreeMask << 1,
    };

    enum eEventType
    {
        k_MountEvent,
        k_UnmountEvent,
    };

    typedef Signal<void( const eEventType& )>    tEventSignal;

protected:

    VActorData     *mDataBlock;

    // Event Signal.
    tEventSignal    mEventSignal;

public:

    VActor( void );
    ~VActor( void );

    // Initialisation Methods.

    bool            onAdd( void );
    void            onRemove( void );

    bool            onNewDataBlock( GameBaseData *pDataBlock, bool pReload );

    // Update Methods.

    virtual void    processTick( const Move *pMove );

    virtual U32     packUpdate( NetConnection *pConnection, U32 pMask, BitStream *pStream );
    virtual void    unpackUpdate( NetConnection *pConnection, BitStream *pStream );

    DECLARE_CONOBJECT( VActor );

public:

    // Accessor Methods.

    inline VActorData     *getDataBlock( void ) { return mDataBlock; };
    inline tEventSignal   &getEventSignal( void ) { return mEventSignal; };

    // Animation Methods.

    /// Get Animation Controller.
    virtual VActorAnimationController *getAnimationController( void ) { return NULL; };

    // Physics Methods.

    /// Set Transform.
    virtual void setTransform( const MatrixF &pMatrix );

    /// Get Physics Controller.
    virtual VActorPhysicsController *getPhysicsController( void ) { return NULL; };

    /// On Mount.
    virtual void onMount( SceneObject *pObject, S32 pNode );
    /// On Unmount.
    virtual void onUnmount( SceneObject *pObject, S32 pNode );
};

#endif // _VT_VACTOR_H_
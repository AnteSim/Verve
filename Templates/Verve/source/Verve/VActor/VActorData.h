//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VACTORDATA_H_
#define _VT_VACTORDATA_H_

#ifndef _SHAPEBASE_H_
#include "T3D/shapeBase.h"
#endif

//-----------------------------------------------------------------------------
class VActor;
class VActorStateTable;
class VActorAnimationState;
class VActorPhysicsState;
//-----------------------------------------------------------------------------

struct VActorData : public ShapeBaseData
{
private:

    typedef ShapeBaseData Parent;
    friend class VActor;

public:

    // Animation Data.

    enum eAnimationList
    {
        k_NextAnimation = 0,
    };

    struct sAnimationSequence
    {
        U32                     Index;
        const char             *Name;
        F32                     Priority;

        VActorAnimationState   *State;
        S32                     Sequence;
    };

    struct sAnimationTransition
    {
        U32    FromIndex;
        U32    ToIndex;

        F32    Duration;

        bool   Ordered;
        U32    Sequence;
    };

    typedef Vector<sAnimationSequence>   tAnimationSequenceVector;
    typedef Vector<sAnimationTransition> tAnimationTransitionVector;

    // Physics Data.

    enum ePhysicsStateList
    {
        k_NextPhysicsState = 0,
    };

    struct sPhysicsState
    {
        U32                   Index;
        F32                   Priority;

        VActorPhysicsState   *State;
    };
    typedef Vector<sPhysicsState>      tPhysicsStateVector;

protected:

    tAnimationSequenceVector      mAnimationSequenceList;
    tAnimationTransitionVector    mAnimationTransitionList;
    tPhysicsStateVector           mPhysicsList;

    F32                           mMaxStepHeight;
    F32                           mRunSpeed;

    F32                           mSubmergeCoverage;

public:

    VActorData( void );
    ~VActorData( void );

    static void     initPersistFields( void );

    virtual bool    initAnimationSequenceList( const S32 &pSize, const sAnimationSequence *pTable );
    virtual bool    initAnimationTransitionList( const S32 &pSize, const sAnimationTransition *pTable );
    virtual bool    initPhysicsStateList( const S32 &pSize, const sPhysicsState *pTable );

    virtual void    packData( BitStream *pStream );
    virtual void    unpackData( BitStream *pStream );

    DECLARE_CONOBJECT( VActorData );

public:

    tAnimationSequenceVector   *getAnimationList( void ) { return &mAnimationSequenceList; };
    S32                         getAnimationSequence( const U32 &pIndex );

    tPhysicsStateVector        *getPhysicsStateList( void ) { return &mPhysicsList; };

    inline const F32           &getMaxStepHeight( void ) const { return mMaxStepHeight; };
    inline const F32           &getRunSpeed( void ) const { return mRunSpeed; };
    inline const F32           &getSumbergeCoverage( void ) const { return mSubmergeCoverage; };
};

#endif // _VT_VACTORDATA_H_
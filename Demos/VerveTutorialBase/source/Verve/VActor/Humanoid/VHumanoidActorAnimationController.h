//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VHUMANOIDACTORANIMATIONCONTROLLER_H_
#define _VT_VHUMANOIDACTORANIMATIONCONTROLLER_H_

#ifndef _VT_VACTORANIMATIONCONTROLLER_H_
#include "../VActorAnimationController.h"
#endif

//-----------------------------------------------------------------------------

class VActor;
class VActorStateTable;

//-----------------------------------------------------------------------------

class VHumanoidActorAnimationController : public VActorAnimationController
{
public:

    /*
    struct sHeadAnimation
    {
        S32           HSequence;
        TSThread     *HThread;
        Range         HRange;

        S32           VSequence;
        TSThread     *VThread;
        Range         VRange;

        S32           FaceSequence;
        TSThread     *FaceThread;

        sHeadAnimation( void ) :
            HSequence( -1 ),
            HThread( NULL ),
            HRange( 0.f, 1.f ),
            VSequence( -1 ),
            VThread( NULL ),
            VRange( 0.f, 1.f )
        {
            // Void.
        }
    };
    */

protected:

    //sHeadAnimation      mHeadAnimation;

    sAnimationRef    mHeadHAnimation;
    sAnimationRef    mHeadVAnimation;
    sAnimationRef    mArmAnimation;

public:

    VHumanoidActorAnimationController( void );
    virtual ~VHumanoidActorAnimationController( void );

    // Initialisation Methods.

    bool            initArmAnimation( const U32 &pIndex, const F32 &pPosition, const F32 &pTimeScale );

    // Animation Methods.

    void            update( const F32 &pDelta );
};

#endif // _VT_VACTORANIMATIONCONTROLLER_H_
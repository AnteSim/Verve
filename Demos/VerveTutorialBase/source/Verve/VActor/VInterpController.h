//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#ifndef _VT_VINTERPCONTROLLER_H_
#define _VT_VINTERPCONTROLLER_H_

#ifndef _MATH_H_
#include "math/mMath.h"
#endif

//-----------------------------------------------------------------------------

class VInterpController
{
protected:

    Point3F    mPosition[2];
    QuatF      mRotation[2];

public:
    
    //-------------------------------------------------------------------------
    // Interpolation Methods.
    //-------------------------------------------------------------------------

    /// Get Position.
    Point3F getPosition( const F32 &pDelta )
    {
        // Interpolate Position.
        Point3F interpPosition;
        interpPosition.interpolate( mPosition[1], mPosition[0], pDelta );
        // Return Interpolated Point.
        return interpPosition;
    };

    /// Get Rotation.
    QuatF getRotation( const F32 &pDelta )
    {
        // Interpolate Rotation.
        QuatF interpRotation;
        interpRotation.interpolate( mRotation[1], mRotation[0], pDelta );
        // Return Interpolated Quat.
        return interpRotation;
    };

    /// Get Transform.
    MatrixF getTransform( const F32 &pDelta )
    {
        // Get Position.
        const Point3F interpPosition = getPosition( pDelta );
        // Get Rotation.
        const QuatF interpRotation = getRotation( pDelta );

        // Setup Matrix.
        MatrixF transform;
        interpRotation.setMatrix( &transform );
        // Set Position.
        transform.setPosition( interpPosition );

        // Return Matrix.
        return transform;
    };
    
    //-------------------------------------------------------------------------
    // Delta Methods.
    //-------------------------------------------------------------------------

    /// Reset Delta.
    void resetDelta( const Point3F &pPosition, const QuatF &pRotation )
    {
        mPosition[0] = mPosition[1] = pPosition;
        mRotation[0] = mRotation[1] = pRotation;
    };

    /// Reset Delta.
    void resetDelta( const MatrixF &pMatrix )
    {
        // Setup Quat.
        QuatF rotationQuat( pMatrix );
        // Reset Delta.
        resetDelta( pMatrix.getPosition(), rotationQuat );
    };

    /// Reset Delta (Vector)
    void resetDelta( const Point3F &pPosition, const VectorF &pForwardVector )
    {
        // Assert.
        AssertFatal( pForwardVector.isUnitLength(), "VInterpController::resetDelta() - Forward Vector hasn't been Normalized." );

        // Static Up Vector.
        static const VectorF sUpVector( 0.f, 0.f, 1.f );

        // X-Axis.
        VectorF xVec = mCross( pForwardVector, sUpVector );
        xVec.normalize();
        // Z-Axis.
        VectorF zVec = mCross( xVec, pForwardVector );
        zVec.normalize();

        // Setup Object Transform.
        MatrixF transform;
        transform.setColumn( 0, xVec );
        transform.setColumn( 1, pForwardVector );
        transform.setColumn( 2, zVec );
        transform.setColumn( 3, pPosition );

        // Reset Delta.
        resetDelta( transform );
    };

    /// Reset Delta (AngAxis)
    void resetDelta( const Point3F &pPosition, const AngAxisF &pAngAxis )
    {
        // Setup Matrix.
        MatrixF transform;
        pAngAxis.setMatrix( &transform );
        // Set Position.
        transform.setPosition( pPosition );

        // Reset Delta.
        resetDelta( transform );
    };

    /// Push Delta.
    void pushDelta( const Point3F &pPosition, const QuatF &pRotation )
    {
        mPosition[1] = pPosition;
        mRotation[1] = pRotation;
    };

    /// Push Delta (Matrix)
    void pushDelta( const MatrixF &pMatrix )
    {
        // Setup Quat.
        QuatF rotationQuat( pMatrix );
        // Push Delta.
        pushDelta( pMatrix.getPosition(), rotationQuat );
    };

    /// Push Delta (Vector)
    void pushDelta( const Point3F &pPosition, const VectorF &pForwardVector )
    {
        // Assert.
        AssertFatal( pForwardVector.isUnitLength(), "VInterpController::pushDelta() - Forward Vector hasn't been Normalized." );

        // Static Up Vector.
        static const VectorF sUpVector( 0.f, 0.f, 1.f );

        // X-Axis.
        VectorF xVec = mCross( pForwardVector, sUpVector );
        xVec.normalize();
        // Z-Axis.
        VectorF zVec = mCross( xVec, pForwardVector );
        zVec.normalize();

        // Setup Object Transform.
        MatrixF transform;
        transform.setColumn( 0, xVec );
        transform.setColumn( 1, pForwardVector );
        transform.setColumn( 2, zVec );
        transform.setColumn( 3, pPosition );

        // Push Delta.
        pushDelta( transform );
    };

    /// Push Delta (AngAxis)
    void pushDelta( const Point3F &pPosition, const AngAxisF &pAngAxis )
    {
        // Setup Matrix.
        MatrixF transform;
        pAngAxis.setMatrix( &transform );
        // Set Position.
        transform.setPosition( pPosition );

        // Push Delta.
        pushDelta( transform );
    };

    /// Pop Delta.
    void popDelta( void )
    {
        mPosition[0] = mPosition[1];
        mRotation[0] = mRotation[1];
    };
};

#endif // _VT_VINTERPCONTROLLER_H_
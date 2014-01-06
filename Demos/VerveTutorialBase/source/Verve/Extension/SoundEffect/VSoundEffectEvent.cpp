//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

#include "Verve/Core/VGroup.h"
#include "Verve/Extension/SoundEffect/VSoundEffectEvent.h"
#include "Verve/Extension/SoundEffect/VSoundEffectTrack.h"

#include "console/consoleTypes.h"

//-----------------------------------------------------------------------------
IMPLEMENT_CONOBJECT( VSoundEffectEvent );
//-----------------------------------------------------------------------------

VSoundEffectEvent::VSoundEffectEvent( void ) : 
        mSoundEffect( NULL )
{
    setLabel( "SoundEvent" );
}

void VSoundEffectEvent::initPersistFields( void )
{
    Parent::initPersistFields();

    addProtectedField( "SoundEffect", TYPEID<VTorque::SoundEffectType>(), Offset( mSoundEffect, VSoundEffectEvent ), &setSoundData, &defaultProtectedGetFn, "" );
}

//-----------------------------------------------------------------------------
//
// Callback Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSoundEffectEvent::onTrigger( pTime, pDelta );
// 
// Play the target sound effect. If this track belongs to a SceneObjectGroup,
// then the sound will play with the reference object's transform. If this is
// not the case, then a 2D sound will be played.
// 
//-----------------------------------------------------------------------------
void VSoundEffectEvent::onTrigger( const S32 &pTime, const S32 &pDelta )
{
    Parent::onTrigger( pTime, pDelta );

    // Fetch Track.
    VSoundEffectTrack *track;
    if ( !getTrack( track ) )
    {
        return;
    }

    // Position & Pitch.
    U32 position = mAbs( ( pTime + pDelta ) - getStartTime() );
    F32 pitch    = mFabs( getControllerTimeScale() );
    if ( position < SFXStartBuffer )
    {
        // Zero.
        position = 0;
    }

    VSceneObjectGroup *group;
    if ( getGroup( group ) )
    {
        // Play Sound With Reference.
        track->mSource = VTorque::playSound( mSoundEffect, group->getSceneObject(), position, pitch );
    }
    else
    {
        // Play Sound.
        track->mSource = VTorque::playSound( mSoundEffect, position, pitch );
    }
}

//-----------------------------------------------------------------------------
//
// Property Methods.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 
// VSoundEffectEvent::setDuration( pDuration );
// 
// This event's duration is always set to the sound object's duration.
// 
//-----------------------------------------------------------------------------
void VSoundEffectEvent::setDuration( const S32 &pDuration )
{
    // Clear Duration.
    mDuration = VTorque::getSoundDuration( mSoundEffect );
}

//-----------------------------------------------------------------------------
//
// Static Field Methods.
//
//-----------------------------------------------------------------------------

bool VSoundEffectEvent::setSoundData( void *pObject, const char *pArray, const char *pData )
{
    // Fetch Event.
    VSoundEffectEvent *event = static_cast<VSoundEffectEvent*>( pObject );

    // Use Object.
    event->mSoundEffect = dynamic_cast<VTorque::SoundEffectType*>( Sim::findObject( pData ) );

    // Set Duration.
    event->setDuration( 0 );

    return false;
}
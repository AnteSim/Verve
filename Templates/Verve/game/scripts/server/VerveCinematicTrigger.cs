//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

datablock TriggerData( VerveCinematicTrigger )
{
    Field = "Value";
};

//-----------------------------------------------------------------------------

function VerveCinematicTrigger::onEnterTrigger( %this, %trigger, %object )
{
    // Play Sequence.
    Verve::PlayCinematic( %trigger.SequenceFile );
}
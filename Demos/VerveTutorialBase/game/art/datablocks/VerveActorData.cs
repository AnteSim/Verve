//-----------------------------------------------------------------------------
// Verve
// Copyright (C) - Violent Tulip
//-----------------------------------------------------------------------------

function VHumanoidActorData::create( %dataBlock )
{
    %actorObject = new VHumanoidActor()
    {
        dataBlock = %dataBlock;
    };
    
    return %actorObject;
}

//-----------------------------------------------------------------------------

datablock VHumanoidActorData( VGideonActorData )
{
    Category  = "VActor";
    ShapeFile = "art/shapes/actors/Gideon/gideon.dts";
};
//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// This is the default save location for any Decal datablocks created in the
// Decal Editor (this script is executed from onServerCreated())

datablock DecalData(ScorchBigDecal)
{
   Material = "DECAL_scorch";
   size = "5.0";
   lifeSpan = "50000";
};

datablock DecalData(ScorchRXDecal)
{
   Material = "DECAL_RocketEXP";
   size = "5.0";
   lifeSpan = "50000";
   randomize = "1";
   texRows = "2";
   texCols = "2";
   clippingAngle = "60";
};

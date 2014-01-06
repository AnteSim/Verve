//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// This is the default save location for any TSForestItemData(s) created in the
// Forest Editor Editor (this script is executed from onServerCreated())

datablock TSForestItemData(ExampleForestMesh)
{
   shapeFile = "art/shapes/trees/defaulttree/defaulttree.dae";
   internalName = "ExampleForestMesh";
   windScale = "1";
   trunkBendScale = "0.02";
   branchAmp = "0.05";
   detailAmp = "0.1";
   detailFreq = "0.2";
   mass = "5";
   rigidity = "20";
   dampingCoefficient = "0.2";
   tightnessCoefficient = "4";
};


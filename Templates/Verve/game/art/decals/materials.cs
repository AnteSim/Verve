//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

singleton Material(DECAL_scorch)
{
   baseTex[0] = "./scorch_decal.png";

   translucent = true;
   translucentBlendOp = None;
   translucentZWrite = true;
   alphaTest = true;
   alphaRef = 84;
};

singleton Material(DECAL_RocketEXP)
{
   translucent = true;
   translucentBlendOp = LerpAlpha;
   translucentZWrite = true;
   mapTo = "rBlast.png";
   diffuseMap[0] = "art/decals/rBlast.png";
   castShadows = "0";
};

singleton Material(DECAL_defaultblobshadow)
{
   baseTex[0] = "art/decals/defaultblobshadow";

   translucent = true;
   translucentBlendOp = LerpAlpha;
   translucentZWrite = true;
   alphaTest = false;
   //alphaRef = 64;
   //emissive[0] = "1";
};

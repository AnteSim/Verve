//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

new Material(w_swarmgun)
{
   mapTo = "w_swarmgun";
   diffuseMap[0] = "w_swarmgun";
   normalMap[0] = "w_swarmgun_bump";
   pixelSpecular[0] = true;
   specular[0] = "1.0 1.0 1.0 1.0";
   specularPower[0] = 32.0;
};

new Material(guide)
{
   mapTo = "guide";
   diffuseMap[0] = "art/shapes/weapons/SwarmGun/guide";
   emissive[0] = true;
   glow[0] = true;
};

new Material(energy)
{
   mapTo = "energy";
   diffuseMap[0] = "art/shapes/weapons/SwarmGun/energy.png";
   emissive[0] = true;
   glow[0] = true;
};

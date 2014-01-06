//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// steampunk materials


singleton Material(weapon_ramrifle_material)
{
   mapTo = "ramrifle_diff";
   diffuseMap[0] = "ramrifle_diff";
   normalMap[0] = "ramrifle_norm";
   //specularTex[0] = "ramrifle_spec";
   pixelSpecular[0] = true;
   specular[0] = "0.99 0.91 0.81";
   specularPower[0] = 32.0;
};

singleton Material(clip)
{
   mapTo = "clip";
   diffuseMap[0] = "clip";
};

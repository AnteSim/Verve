
singleton Material(soldier_gun_lense)
{
   mapTo = "soldier_gun_lense";
   diffuseMap[0] = "art/shapes/weapons/Soldier_gun/Weapon_Dif";
   specular[0] = "0.9 0.9 0.9 1";
   specularPower[0] = "10";
   translucentBlendOp = "None";
   glow[0] = "1";
   emissive[0] = "1";
   diffuseColor[0] = "0.129412 0.172549 0.141176 1";
};

singleton Material(soldier_gun_Gun)
{
   mapTo = "soldier_gun_Gun";
   diffuseMap[0] = "art/shapes/weapons/Soldier_gun/Weapon_Dif";
   specular[0] = "0.9 0.9 0.9 1";
   specularPower[0] = "1";
   translucentBlendOp = "None";
   normalMap[0] = "art/shapes/weapons/Soldier_gun/Weapon_Normal.tga";
   specularMap[0] = "art/shapes/weapons/Soldier_gun/Weapon_Spec.dds";
};

singleton Material(soldier_gun_flash)
{
   mapTo = "soldier_gun_flash";
   diffuseMap[0] = "art/shapes/weapons/Soldier_gun/muzzle_flash";
   specular[0] = "0.9 0.9 0.9 1";
   specularPower[0] = "10";
   translucentBlendOp = "Add";
   doubleSided = "1";
   translucent = "1";
   alphaTest = "1";
   diffuseColor[0] = "0.141176 0.117647 0.0901961 1";
   glow[0] = "1";
   emissive[0] = "1";
};

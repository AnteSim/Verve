//--- soldier_rigged.dae MATERIALS BEGIN ---
singleton Material(soldier_Max_Soldier)
{
   mapTo = "soldier_Max_Soldier";

   diffuseMap[0] = "art/shapes/actors/Soldier/Soldier_Dif";
   normalMap[0] = "art/shapes/actors/Soldier/Soldier_n.tga";
   specularMap[0] = "art/shapes/actors/Soldier/Soldier_Spec.dds";

   diffuseColor[0] = "1 1 1 1";
   specular[0] = "0.9 0.9 0.9 1";
   specularPower[0] = 10;

   doubleSided = false;
   translucent = false;
   translucentBlendOp = "None";
   materialTag0 = "RoadAndPath";
   showFootprints = "0";
};

singleton Material(soldier__4___Default)
{
   mapTo = "soldier__4_-_Default";

   diffuseMap[0] = "art/shapes/actors/Soldier/Soldier_Dazzle.dds";
   normalMap[0] = "";
   specularMap[0] = "";

   diffuseColor[0] = "1 1 1 1";
   specular[0] = "0.9 0.9 0.9 1";
   specularPower[0] = 10;

   doubleSided = false;
   translucent = "1";
   translucentBlendOp = "Add";
   glow[0] = "1";
   emissive[0] = "1";
   materialTag0 = "RoadAndPath";
   castShadows = "0";
   showFootprints = "0";
};

//--- soldier_rigged.dae MATERIALS END ---

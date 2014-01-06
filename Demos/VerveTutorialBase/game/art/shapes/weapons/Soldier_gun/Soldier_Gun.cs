
singleton TSShapeConstructor(Soldier_GunDAE)
{
   baseShape = "./Soldier_Gun.DAE";
   matNamePrefix = "soldier_gun_";
   forceUpdateMaterials = "0";
   loadLights = "0";
};

function Soldier_GunDAE::onLoad(%this)
{
   %this.addSequence("ambient", "fire", "0", "0");
   %this.addSequence("ambient", "fire_vis", "0", "2");
   %this.removeSequence("ambient");
}

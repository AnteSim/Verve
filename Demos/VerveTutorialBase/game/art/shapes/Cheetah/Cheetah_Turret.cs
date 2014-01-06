
singleton TSShapeConstructor(Cheetah_TurretDAE)
{
   baseShape = "./Cheetah_Turret.DAE";
};

function Cheetah_TurretDAE::onLoad(%this)
{
   %this.addNode("muzzlePoint", "Cheetah_Turret1", "-0.0192316 1.9606 -0.0781175 1 0 0 0", "1");
   %this.addNode("ejectPoint", "Cheetah_Turret1", "0.340047 0.793052 -0.044629 1 0 0 0", "1");
   %this.renameSequence("ambient", "fire");
}

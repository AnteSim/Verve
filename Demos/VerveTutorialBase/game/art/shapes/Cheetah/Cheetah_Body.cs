
singleton TSShapeConstructor(CheetahDAE)
{
   baseShape = "./Cheetah_Body.DAE";
   loadLights = "0";
   lodType = "SingleSize";
   singleDetailSize = "100";
   neverImport = "null";
};

function CheetahDAE::onLoad(%this)
{
   %this.renameObject("col-1", "col");
   %this.setMeshSize("col 100", "-1");
   %this.setSequenceCyclic("ambient", "0");
   %this.addSequence("ambient", "spring0", "1", "2");
   %this.addSequence("ambient", "spring1", "4", "5");
   %this.addSequence("ambient", "spring2", "7", "8");
   %this.addSequence("ambient", "spring3", "10", "11");
   %this.addSequence("ambient", "brakeLight", "12", "13");
   %this.removeSequence("ambient");
   %this.setNodeTransform("cam", "5.46934e-008 -4.75632 2.89171 -0.404897 0.817636 0.409303 1.71107", "1");
}

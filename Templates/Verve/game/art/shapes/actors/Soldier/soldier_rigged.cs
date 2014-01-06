
singleton TSShapeConstructor(SoldierDAE)
{
   baseShape = "./soldier_rigged.dae";
   loadLights = "0";
   unit = "1.2";
   upAxis = "DEFAULT";
   lodType = "DetectDTS";
   singleDetailSize = "2";
   matNamePrefix = "soldier_";
   ignoreNodeScale = "0";
   adjustCenter = "0";
   adjustFloor = "0";
};

function SoldierDAE::onLoad(%this)
{
   %this.addSequence("ambient", "root", "0", "60");
   %this.addSequence("ambient", "run", "1510", "1530");
   %this.addSequence("ambient", "back", "1610", "1630");
   %this.addSequence("ambient", "side", "1560", "1580");
   %this.addSequence("ambient", "crouch_root", "330", "390");
   %this.addSequence("ambient", "crouch_forward", "1660", "1690");
   %this.addSequence("ambient", "crouch_backward", "1720", "1750");
   %this.addSequence("ambient", "crouch_side", "1780", "1810");
   %this.addSequence("ambient", "swim_root", "2150", "2210");
   %this.addSequence("ambient", "swim_forward", "2240", "2300");
   %this.addSequence("ambient", "swim_backward", "2330", "2390");
   %this.addSequence("ambient", "swim_left", "2420", "2480");
   %this.addSequence("ambient", "swim_right", "2510", "2570");
   %this.addSequence("ambient", "death1", "1230", "1270");
   %this.setSequenceCyclic("death1", "0");
   %this.addSequence("ambient", "death2", "1300", "1340");
   %this.setSequenceCyclic("death2", "0");
   %this.addSequence("ambient", "death3", "1370", "1410");
   %this.setSequenceCyclic("death3", "0");
   %this.addSequence("ambient", "jump", "1040", "1050");
   %this.setSequenceCyclic("jump", "0");
   %this.addSequence("ambient", "land", "1950", "1960");
   %this.setSequenceCyclic("land", "0");
   %this.addSequence("ambient", "fall", "1890", "1905");
   %this.setSequenceCyclic("fall", "0");
   %this.addSequence("ambient", "sitting", "1840", "1840");
   %this.addSequence("ambient", "look", "1990", "2000");
   %this.setSequenceCyclic("look", "0");
   %this.setSequenceBlend("look", "1", "root", "0");
   %this.setNodeTransform("cam", "0 0 1.90056 1 0 0 0", "1");
}

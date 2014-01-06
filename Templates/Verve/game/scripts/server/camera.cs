//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// Global movement speed that affects all cameras.  This should be moved
// into the camera datablock.
$Camera::movementSpeed = 40;

function Observer::onTrigger(%this,%obj,%trigger,%state)
{
   // state = 0 means that a trigger key was released
   if (%state == 0)
      return;

   // Default player triggers: 0=fire 1=altFire 2=jump
   %client = %obj.getControllingClient();
   switch$ (%obj.mode)
   {
      case "Observer":
         // Do something interesting.

      case "Corpse":
         // Viewing dead corpse, so we probably want to respawn.
         game.preparePlayer(%client);//%client.spawnPlayer();

         // Set the camera back into observer mode, since in
         // debug mode we like to switch to it.
         %this.setMode(%obj,"Observer");
   }
}

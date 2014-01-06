//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// Timeouts for corpse deletion.
$CorpseTimeoutValue = 45 * 1000;

// // Damage Rate for entering Liquid
// $DamageLava = 0.01;
// $DamageHotLava = 0.01;
// $DamageCrustyLava = 0.01;

// Death Animations
$PlayerDeathAnim::TorsoFrontFallForward = 1;
$PlayerDeathAnim::TorsoFrontFallBack = 2;
$PlayerDeathAnim::TorsoBackFallForward = 3;
$PlayerDeathAnim::TorsoLeftSpinDeath = 4;
$PlayerDeathAnim::TorsoRightSpinDeath = 5;
$PlayerDeathAnim::LegsLeftGimp = 6;
$PlayerDeathAnim::LegsRightGimp = 7;
$PlayerDeathAnim::TorsoBackFallForward = 8;
$PlayerDeathAnim::HeadFrontDirect = 9;
$PlayerDeathAnim::HeadBackFallForward = 10;
$PlayerDeathAnim::ExplosionBlowBack = 11;

//----------------------------------------------------------------------------
// Armor Datablock methods
//----------------------------------------------------------------------------

function Armor::onAdd(%this, %obj)
{
   // Vehicle timeout
   %obj.mountVehicle = true;

   // Default dynamic armor stats
   %obj.setRechargeRate(%this.rechargeRate);
   %obj.setRepairRate(0);

   // Set the numerical Health HUD
   //%obj.updateHealth();

   // Calling updateHealth() must be delayed now... for some reason
   %obj.schedule(50, "updateHealth");
}

function Armor::onRemove(%this, %obj)
{
   if (%obj.client.player == %obj)
      %obj.client.player = 0;
}

function Armor::onNewDataBlock(%this, %obj)
{
}

//----------------------------------------------------------------------------

function Armor::onMount(%this, %obj, %vehicle, %node)
{
   // Node 0 is the pilot's position, we need to dismount his weapon.
   if (%node == 0)
   {
      %obj.setTransform("0 0 0 0 0 1 0");
      %obj.setActionThread(%vehicle.getDatablock().mountPose[%node], true, true);

      %obj.lastWeapon = %obj.getMountedImage($WeaponSlot);
      %obj.unmountImage($WeaponSlot);

      %obj.setControlObject(%vehicle);
      
      if(%obj.getClassName() $= "Player")
         commandToClient(%obj.client, 'toggleVehicleMap', true);
   }
   else
   {
      if (%vehicle.getDataBlock().mountPose[%node] !$= "")
         %obj.setActionThread(%vehicle.getDatablock().mountPose[%node]);
      else
         %obj.setActionThread("root", true);
   }
}

function Armor::onUnmount(%this, %obj, %vehicle, %node)
{
   if (%node == 0)
   {
      %obj.mountImage(%obj.lastWeapon, $WeaponSlot);
      %obj.setControlObject("");
   }
}

function Armor::doDismount(%this, %obj, %forced)
{
   //echo("\c4Armor::doDismount(" @ %this @", "@ %obj.client.nameBase @", "@ %forced @")");

   // This function is called by player.cc when the jump trigger
   // is true while mounted
   %vehicle = %obj.mVehicle;
   if (!%obj.isMounted() || !isObject(%vehicle))
      return;

   // Vehicle must be at rest!
   if ((VectorLen(%vehicle.getVelocity()) <= %vehicle.getDataBlock().maxDismountSpeed ) || %forced)
   {
      // Position above dismount point
      %pos = getWords(%obj.getTransform(), 0, 2);
      %rot = getWords(%obj.getTransform(), 3, 6);
      %oldPos = %pos;
      %vec[0] = " -1 0 0";
      %vec[1] = " 0 0 1";
      %vec[2] = " 0 0 -1";
      %vec[3] = " 1 0 0";
      %vec[4] = "0 -1 0";
      %impulseVec = "0 0 0";
      %vec[0] = MatrixMulVector(%obj.getTransform(), %vec[0]);

      // Make sure the point is valid
      %pos = "0 0 0";
      %numAttempts = 5;
      %success = -1;
      for (%i = 0; %i < %numAttempts; %i++)
      {
         %pos = VectorAdd(%oldPos, VectorScale(%vec[%i], 3));
         if (%obj.checkDismountPoint(%oldPos, %pos))
         {
            %success = %i;
            %impulseVec = %vec[%i];
            break;
         }
      }
      if (%forced && %success == -1)
         %pos = %oldPos;

      %obj.mountVehicle = false;
      %obj.schedule(4000, "mountVehicles", true);

      // Position above dismount point
      %obj.unmount();
      %obj.setTransform(%pos SPC %rot);//%obj.setTransform(%pos);
      //%obj.playAudio(0, UnmountVehicleSound);
      %obj.applyImpulse(%pos, VectorScale(%impulseVec, %obj.getDataBlock().mass));

      // Set player velocity when ejecting
      %vel = %obj.getVelocity();
      %vec = vectorDot( %vel, vectorNormalize(%vel));
      if(%vec > 50)
      {
         %scale = 50 / %vec;
         %obj.setVelocity(VectorScale(%vel, %scale));
      }

      //%obj.vehicleTurret = "";
   }
   else
      messageClient(%obj.client, 'msgUnmount', '\c2Cannot exit %1 while moving.', %vehicle.getDataBlock().nameTag);
}

//----------------------------------------------------------------------------

function Armor::onCollision(%this, %obj, %col)
{
   if (!isObject(%col) || %obj.getState() $= "Dead")
      return;

   // Try and pickup all items
   if (%col.getClassName() $= "Item")
   {
      %obj.pickup(%col);
      return;
   }

   // Mount vehicles
   if (%col.getType() & $TypeMasks::GameBaseObjectType)
   {
      %db = %col.getDataBlock();
      if ((%db.getClassName() $= "WheeledVehicleData" ) && %obj.mountVehicle && %obj.getState() $= "Move" && %col.mountable)
      {
         // Only mount drivers for now.
         ServerConnection.setFirstPerson(0);
         
         // For this specific example, only one person can fit
         // into a vehicle
         %mount = %col.getMountNodeObject(0);         
         if(%mount)
            return;
         
         // For this specific FPS Example, always mount the player
         // to node 0
         %node = 0;
         %col.mountObject(%obj, %node);
         %obj.mVehicle = %col;
      }
   }
}

function Armor::onImpact(%this, %obj, %collidedObject, %vec, %vecLen)
{
   %obj.damage(0, VectorAdd(%obj.getPosition(), %vec), %vecLen * %this.speedDamageScale, "Impact");
}

//----------------------------------------------------------------------------

function Armor::damage(%this, %obj, %sourceObject, %position, %damage, %damageType)
{
   if (!isObject(%obj) || %obj.getState() $= "Dead")
      return;

   %obj.applyDamage(%damage);

   %location = "Body";

   // Update the numerical Health HUD
   %obj.updateHealth();

   // Deal with client callbacks here because we don't have this
   // information in the onDamage or onDisable methods
   %client = %obj.client;
   %sourceClient = %sourceObject ? %sourceObject.client : 0;

   if (%obj.getState() $= "Dead" && isObject(%client))
      %client.onDeath(%sourceObject, %sourceClient, %damageType, %location);
}

function Armor::onDamage(%this, %obj, %delta)
{
   // This method is invoked by the ShapeBase code whenever the
   // object's damage level changes.
   if (%delta > 0 && %obj.getState() !$= "Dead")
   {
      // Apply a damage flash
      %obj.setDamageFlash(1);

      // If the pain is excessive, let's hear about it.
      if (%delta > 10)
         %obj.playPain();
   }
}

// ----------------------------------------------------------------------------
// The player object sets the "disabled" state when damage exceeds it's
// maxDamage value. This is method is invoked by ShapeBase state mangement code.

// If we want to deal with the damage information that actually caused this
// death, then we would have to move this code into the script "damage" method.

function Armor::onDisabled(%this, %obj, %state)
{
   // Release the main weapon trigger
   %obj.setImageTrigger(0, false);

   // Toss current mounted weapon and ammo if any
   %item = %obj.getMountedImage($WeaponSlot).item;
   if (isObject(%item))
   {
      %amount = %obj.getInventory(%item.image.ammo);
      if(%amount)
         %obj.throw(%item.image.ammo, %amount);
      %obj.throw(%item, 1);
   }

   // Toss out a health patch
   %obj.tossPatch();

   %obj.playDeathCry();
   %obj.playDeathAnimation();
   //%obj.setDamageFlash(0.75);

   // Schedule corpse removal. Just keeping the place clean.
   %obj.schedule($CorpseTimeoutValue - 1000, "startFade", 1000, 0, true);
   %obj.schedule($CorpseTimeoutValue, "delete");
}

//-----------------------------------------------------------------------------

function Armor::onLeaveMissionArea(%this, %obj)
{
   //echo("\c4Leaving Mission Area at POS:"@ %obj.getPosition());

   // Inform the client
   %obj.client.onLeaveMissionArea();

   // Damage over time and kill the coward!
   //%obj.setDamageDt(0.2, "MissionAreaDamage");
}

function Armor::onEnterMissionArea(%this, %obj)
{
   //echo("\c4Entering Mission Area at POS:"@ %obj.getPosition());

   // Inform the client
   %obj.client.onEnterMissionArea();

   // Stop the punishment
   //%obj.clearDamageDt();
}

//-----------------------------------------------------------------------------

function Armor::onEnterLiquid(%this, %obj, %coverage, %type)
{
   //echo("\c4this:"@ %this @" object:"@ %obj @" just entered water of type:"@ %type @" for "@ %coverage @"coverage");
}

function Armor::onLeaveLiquid(%this, %obj, %type)
{
   //
}

//-----------------------------------------------------------------------------

function Armor::onTrigger(%this, %obj, %triggerNum, %val)
{
   // This method is invoked when the player receives a trigger move event.
   // The player automatically triggers slot 0 and slot one off of triggers #
   // 0 & 1.  Trigger # 2 is also used as the jump key.
}

//-----------------------------------------------------------------------------
// Player methods
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------

function Player::kill(%this, %damageType)
{
   %this.damage(0, %this.getPosition(), 10000, %damageType);
}

//----------------------------------------------------------------------------

function Player::mountVehicles(%this, %bool)
{
   // If set to false, this variable disables vehicle mounting.
   %this.mountVehicle = %bool;
}

function Player::isPilot(%this)
{
   %vehicle = %this.getObjectMount();
   // There are two "if" statements to avoid a script warning.
   if (%vehicle)
      if (%vehicle.getMountNodeObject(0) == %this)
         return true;
   return false;
}

//----------------------------------------------------------------------------

function Player::playDeathAnimation(%this)
{
   if (isObject(%this.client))
   {
      if (%this.client.deathIdx++ > 11)
         %this.client.deathIdx = 1;
      %this.setActionThread("Death" @ %this.client.deathIdx);
   }
   else
   {
      %rand = getRandom(1, 11);
      %this.setActionThread("Death" @ %rand);
   }
}

function Player::playCelAnimation(%this, %anim)
{
   if (%this.getState() !$= "Dead")
      %this.setActionThread("cel"@%anim);
}


//----------------------------------------------------------------------------

function Player::playDeathCry(%this)
{
   %this.playAudio(0, DeathCrySound);
}

function Player::playPain(%this)
{
   %this.playAudio(0, PainCrySound);
}

// ----------------------------------------------------------------------------
// Numerical Health Counter
// ----------------------------------------------------------------------------

function Player::updateHealth(%player)
{
   //echo("\c4Player::updateHealth() -> Player Health changed, updating HUD!");

   // Calcualte player health
   %maxDamage = %player.getDatablock().maxDamage;
   %damageLevel = %player.getDamageLevel();
   %curHealth = %maxDamage - %damageLevel;
   %curHealth = mceil(%curHealth);

   // Send the player object's current health level to the client, where it
   // will Update the numericalHealth HUD.
   commandToClient(%player.client, 'setNumericalHealthHUD', %curHealth);
}

function Player::use(%player, %data)
{
   // No mounting/using weapons when you're driving!
   if (%player.isPilot())
      return(false);

   Parent::use(%player, %data);
}

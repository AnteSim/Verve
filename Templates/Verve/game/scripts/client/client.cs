//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Server Admin Commands
//-----------------------------------------------------------------------------

function SAD(%password)
{
   if (%password !$= "")
      commandToServer('SAD', %password);
}

function SADSetPassword(%password)
{
   commandToServer('SADSetPassword', %password);
}

//----------------------------------------------------------------------------
// Misc server commands
//----------------------------------------------------------------------------

function clientCmdSyncClock(%time)
{
   // Time update from the server, this is only sent at the start of a mission
   // or when a client joins a game in progress.
}

//-----------------------------------------------------------------------------
// Numerical Health Counter
//-----------------------------------------------------------------------------

function clientCmdSetNumericalHealthHUD(%curHealth)
{
   // Skip if the hud is missing.
   if (!isObject(numericalHealthHUD))
      return;

   // The server has sent us our current health, display it on the HUD
   numericalHealthHUD.setValue(%curHealth);

   // Ensure the HUD is set to visible while we have health / are alive
   if (%curHealth)
      HealthHUD.setVisible(true);
   else
      HealthHUD.setVisible(false);
}

// ----------------------------------------------------------------------------
// WeaponHUD
// ----------------------------------------------------------------------------

// Update the Ammo Counter with current ammo, if not any then hide the counter.

function clientCmdSetAmmoAmountHud(%amount)
{
   if (!%amount)
      AmmoAmount.setVisible(false);
   else
   {
      AmmoAmount.setVisible(true);
      AmmoAmount.setText("Ammo: "@%amount);
   }
}

// Here we update the Weapon Preview image & reticle for each weapon.  We also
// update the Ammo Counter (just so we don't have to call it separately).
// Passing an empty parameter ("") hides the HUD component.

function clientCmdRefreshWeaponHUD(%amount, %preview, %ret, %zoomRet)
{
   if (!%amount)
      AmmoAmount.setVisible(false);
   else
   {
      AmmoAmount.setVisible(true);
      AmmoAmount.setText("Ammo: "@ %amount);
   }

   if (%preview $= "")
      WeaponHUD.setVisible(false);//PreviewImage.setVisible(false);
   else
   {
      WeaponHUD.setVisible(true);//PreviewImage.setVisible(true);
      PreviewImage.setbitmap("art/gui/weaponHud/"@ detag(%preview));
   }

   if (%ret $= "")
      Reticle.setVisible(false);
   else
   {
      Reticle.setVisible(true);
      Reticle.setbitmap("art/gui/weaponHud/"@ detag(%ret));
   }

   if (isObject(ZoomReticle))
   {
      if (%zoomRet $= "")
      {
         ZoomReticle.setBitmap("");
      }
      else
      {
         ZoomReticle.setBitmap("art/gui/weaponHud/"@ detag(%zoomRet));
      }
   }
}

function clientCmdtoggleVehicleMap(%toggle)
{
   if(%toggle)
   {
      moveMap.pop();
      vehicleMap.push();
   }
   else
   {
      vehicleMap.pop();
      moveMap.push();
   }
}
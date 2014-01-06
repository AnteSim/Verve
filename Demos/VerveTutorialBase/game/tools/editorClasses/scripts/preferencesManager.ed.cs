//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//*** Initializes the Preferences Manager
function initPreferencesManager()
{
   // FIXME TGEA doesnt currently have these due to the way it's built
   return;
   
   //*** Create the Preferences Manager singleton
   %pm = new PreferencesManager(pref);
   registerPreferencesManager(%pm.getId());
}

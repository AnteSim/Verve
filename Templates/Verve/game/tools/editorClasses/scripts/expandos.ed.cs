//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// Called to setup the base expandos
function setupBaseExpandos()
{
   // FIXME TGEA doesnt currently have these due to the way it's built
   return;
   
   setScriptPathExpando("tools", getExecutablePath() @ "/tools", true);
   setScriptPathExpando("tool", getExecutablePath() , true);
   setScriptPathExpando("toolResources", getExecutablePath() @ "/resources", true);
   
   setScriptPathExpando("core", getExecutablePath() @ "/core", true);
   
   // Remove the game expando so we can use this to reset expandos
   removeScriptPathExpando("game");
}

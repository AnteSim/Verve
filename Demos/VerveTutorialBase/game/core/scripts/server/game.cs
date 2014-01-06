//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// The default hooks that are most likely be overridden/implemented by a game
//-----------------------------------------------------------------------------

function onServerCreated()
{
   // Invoked by createServer(), when server is created and ready to go

   // Server::GameType is sent to the master server.
   // This variable should uniquely identify your game and/or mod.
   $Server::GameType = "Test App";

   // Server::MissionType sent to the master server.  Clients can
   // filter servers based on mission type.
   $Server::MissionType = "Deathmatch";

   // Load up any objects or datablocks saved to the editor managed scripts
   if (isScriptFile("art/shapes/particles/managedParticleData.cs"))
      exec("art/shapes/particles/managedParticleData.cs");
   if (isScriptFile("art/decals/managedDecalData.cs"))
      exec("art/decals/managedDecalData.cs");
   if (isScriptFile("art/datablocks/managedDatablocks.cs"))
      exec("art/datablocks/managedDatablocks.cs");
   if (isScriptFile("art/forest/managedItemData.cs"))
      exec("art/forest/managedItemData.cs");
      
   // Load up user specified data and object declarations
   exec("art/datablocks/datablockExec.cs");

   // Run the other gameplay scripts in this folder
   exec("./scriptExec.cs");

   // For backwards compatibility...
   createGame();
}

function onServerDestroyed()
{
   // Invoked by destroyServer(), right before the server is destroyed
   destroyGame();
}

function onMissionLoaded()
{
   // Called by loadMission() once the mission is finished loading
   startGame();
}

function onMissionEnded()
{
   // Called by endMission(), right before the mission is destroyed
   endGame();
}

function onMissionReset()
{
   // Called by resetMission(), after all the temporary mission objects
   // have been deleted.
}


//-----------------------------------------------------------------------------
// These methods are extensions to the GameConnection class. Extending
// GameConnection make is easier to deal with some of this functionality,
// but these could also be implemented as stand-alone functions.
//-----------------------------------------------------------------------------

function GameConnection::onClientEnterGame(%this)
{
   // Called for each client after it's finished downloading the
   // mission and is ready to start playing.
}

function GameConnection::onClientLeaveGame(%this)
{
   // Call for each client that drops
}


//-----------------------------------------------------------------------------
// Functions that implement game-play
// These are here for backwards compatibilty only, games and/or mods should
// really be overloading the server and mission functions listed ubove.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

function createGame()
{
   // This function is called by onServerCreated (above)
}

function destroyGame()
{
   // This function is called by onServerDestroyed (above)
}


//-----------------------------------------------------------------------------

function startGame()
{
   // This is where the game play should start
   // The default onMissionLoaded function starts the game.
}

function endGame()
{
   // This is where the game play should end
   // The default onMissionEnded function shuts down the game.
}

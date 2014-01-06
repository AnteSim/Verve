//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GameCore
// ----------------------------------------------------------------------------
// This is the core of the gametype functionality. The "Default Game". All of
// the gametypes share or over-ride the scripted controls for the default game.
//
// The desired Game Type must be added to each mission's LevelInfo object.
//   - gameType = "";
//   - gameType = "Deathmatch";
// If this information is missing then the GameCore will default to Deathmatch.
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Basic Functionality
// ----------------------------------------------------------------------------

// Static function to create the Game object.
// Makes use of theLevelInfo object to determine the game type.
// Returns: The Game object
function GameCore::createGame()
{
   // Create Game Objects
   // Here begins our gametype functionality
   if (isObject(theLevelInfo))
   {
      $Server::MissionType = theLevelInfo.gameType;  //MissionInfo.gametype;
      //echo("\c4 -> Parsed mission Gametype: "@ theLevelInfo.gameType); //MissionInfo.gametype);
   }
   else
   {
      $Server::MissionType = "";
   }

   if ($Server::MissionType $= "")
      $Server::MissionType = "Deathmatch"; //Default gametype, just in case

   // Note: The Game object will be cleaned up by MissionCleanup.  Therefore its lifetime is
   // limited to that of the mission.
   new ScriptObject(Game)
   {
      class = $Server::MissionType @"Game";
      superClass = GameCore;
   };

   // Activate the Game specific packages that are defined by the level's gameType
   Game.activatePackages();

   return game;
}

function GameCore::activatePackages(%game)
{
   echo (%game @"\c4 -> activatePackages");

   // Activate any mission specific game package
   if (isPackage(%game.class) && %game.class !$= GameCore)
      activatePackage(%game.class);
}

function GameCore::deactivatePackages(%game)
{
   echo (%game @"\c4 -> deactivatePackages");

   // Deactivate any mission specific game package
   if (isPackage(%game.class) && %game.class !$= GameCore)
      deactivatePackage(%game.class);
}

function GameCore::onAdd(%game)
{
   //echo (%game @"\c4 -> onAdd");
}

function GameCore::onRemove(%game)
{
   //echo (%game @"\c4 -> onRemove");

   // Clean up
   %game.deactivatePackages();
}

// ----------------------------------------------------------------------------
// Package
// ----------------------------------------------------------------------------

// The GameCore package overides functions loadMissionStage2(), endMission(),
// and function resetMission() from "core/scripts/server/missionLoad.cs" in
// order to create our Game object, which allows our gameType functionality to
// be initiated.

package GameCore
{
   function loadMissionStage2()
   {
      //echo("\c4 -> loadMissionStage2() override success");

      echo("*** Stage 2 load");

      // Create the mission group off the ServerGroup
      $instantGroup = ServerGroup;

      // Make sure the mission exists
      %file = $Server::MissionFile;

      if( !isFile( %file ) )
      {
         $Server::LoadFailMsg = "Could not find mission \"" @ %file @ "\"";
      }
      else
      {
         // Calculate the mission CRC.  The CRC is used by the clients
         // to caching mission lighting.
         $missionCRC = getFileCRC( %file );

         // Exec the mission.  The MissionGroup (loaded components) is added to the ServerGroup
         exec(%file);

         if( !isObject(MissionGroup) )
         {
            $Server::LoadFailMsg = "No 'MissionGroup' found in mission \"" @ %file @ "\".";
         }
      }

      if( $Server::LoadFailMsg !$= "" )
      {
         // Inform clients that are already connected
         for (%clientIndex = 0; %clientIndex < ClientGroup.getCount(); %clientIndex++)
            messageClient(ClientGroup.getObject(%clientIndex), 'MsgLoadFailed', $Server::LoadFailMsg);
         return;
      }

      // Set mission name.

      if( isObject( theLevelInfo ) )
         $Server::MissionName = theLevelInfo.levelName;

      // Mission cleanup group.  This is where run time components will reside.  The MissionCleanup
      // group will be added to the ServerGroup.
      new SimGroup(MissionCleanup);

      // Make the MissionCleanup group the place where all new objects will automatically be added.
      $instantGroup = MissionCleanup;

      // Create the Game object
      GameCore::createGame();

      // Construct MOD paths
      pathOnMissionLoadDone();

      // Mission loading done...
      echo("*** Mission loaded");

      // Start all the clients in the mission
      $missionRunning = true;
      for (%clientIndex = 0; %clientIndex < ClientGroup.getCount(); %clientIndex++)
         ClientGroup.getObject(%clientIndex).loadMission();

      // Go ahead and launch the mission
      Game.onMissionLoaded();
   }

   function endMission()
   {
      //echo("\c4 -> endMission() override success");

      // If there is no MissionGroup then there is no running mission.
      // It may have already been cleaned up.
      if (!isObject(MissionGroup))
         return;

      echo("*** ENDING MISSION");

      // Inform the game code we're done.
      Game.onMissionEnded();

      // Inform the clients
      for (%clientIndex = 0; %clientIndex < ClientGroup.getCount(); %clientIndex++)
      {
         // clear ghosts and paths from all clients
         %cl = ClientGroup.getObject(%clientIndex);
         %cl.endMission();
         %cl.resetGhosting();
         %cl.clearPaths();
      }

      // Delete everything
      MissionGroup.delete();
      MissionCleanup.delete();   // Note: Will also clean up the Game object

      // With MissionCleanup gone, make the ServerGroup the default place to put objects
      $instantGroup = ServerGroup;

      clearServerpaths();
   }

   // resetMission() is very game specific.  To get the most out of it you'll
   // need to expand on what is here, such as recreating runtime objects etc.
   function resetMission()
   {
      //echo("\c4 -> resetMission() override success");
      echo("*** MISSION RESET");

      // Remove any temporary mission objects
      // NOTE: This will likely remove any player objects as well so
      // use resetMission() with caution.
      MissionCleanup.delete();
      $instantGroup = ServerGroup;
      new SimGroup(MissionCleanup);
      $instantGroup = MissionCleanup;

      clearServerpaths();

      // Recreate the Game object
      GameCore::createGame();

      // Construct MOD paths
      pathOnMissionLoadDone();

      // Allow the Game object to reset the mission
      Game.onMissionReset();
   }

   // We also need to override function GameConnection::onConnect() from
   // "core/scripts/server/clientConnection.cs" in order to initialize, reset,
   // and pass some client scoring variables to playerList.gui -- the scoreHUD.

   function GameConnection::onConnect(%client, %name)
   {
      // Send down the connection error info, the client is responsible for
      // displaying this message if a connection error occurs.
      messageClient(%client, 'MsgConnectionError',"",$Pref::Server::ConnectionError);

      // Send mission information to the client
      sendLoadInfoToClient(%client);

      // Simulated client lag for testing...
      // %client.setSimulatedNetParams(0.1, 30);

      // Get the client's unique id:
      // %authInfo = %client.getAuthInfo();
      // %client.guid = getField(%authInfo, 3);
      %client.guid = 0;
      addToServerGuidList(%client.guid);

      // Set admin status
      if (%client.getAddress() $= "local")
      {
         %client.isAdmin = true;
         %client.isSuperAdmin = true;
      }
      else
      {
         %client.isAdmin = false;
         %client.isSuperAdmin = false;
      }

      // Save client preferences on the connection object for later use.
      %client.gender = "Male";
      %client.armor = "Light";
      %client.race = "Human";
      %client.skin = addTaggedString("base");
      %client.setPlayerName(%name);
      %client.score = 0;
      %client.kills = 0;
      %client.deaths = 0;

      //
      echo("CADD: "@ %client @" "@ %client.getAddress());

      // Inform the client of all the other clients
      %count = ClientGroup.getCount();
      for (%cl = 0; %cl < %count; %cl++)
      {
         %other = ClientGroup.getObject(%cl);
         if ((%other != %client))
         {
            // These should be "silent" versions of these messages...
            messageClient(%client, 'MsgClientJoin', "",
               %other.playerName,
               %other,
               %other.sendGuid,
               %other.score,
               %other.kills,
               %other.deaths,
               %other.isAIControlled(),
               %other.isAdmin,
               %other.isSuperAdmin);
         }
      }

      // Inform the client we've joined up
      messageClient(%client,
         'MsgClientJoin', '\c2Welcome to the Torque demo app %1.',
         %client.playerName,
         %client,
         %client.sendGuid,
         %client.score,
         %client.kills,
         %client.deaths,
         %client.isAiControlled(),
         %client.isAdmin,
         %client.isSuperAdmin);

      // Inform all the other clients of the new guy
      messageAllExcept(%client, -1, 'MsgClientJoin', '\c1%1 joined the game.',
         %client.playerName,
         %client,
         %client.sendGuid,
         %client.score,
         %client.kills,
         %client.deaths,
         %client.isAiControlled(),
         %client.isAdmin,
         %client.isSuperAdmin);

      // If the mission is running, go ahead download it to the client
      if ($missionRunning)
      {
         %client.loadMission();
      }
      else if ($Server::LoadFailMsg !$= "")
      {
         messageClient(%client, 'MsgLoadFailed', $Server::LoadFailMsg);
      }
      $Server::PlayerCount++;
   }

   function GameConnection::onClientEnterGame(%this)
   {
      Game.onClientEnterGame(%this);
   }

   function GameConnection::onClientLeaveGame(%this)
   {
      // If this mission has ended before the client has left the game then
      // the Game object will have already been cleaned up.  See endMission()
      // in the GameCore package.
      if (isObject(Game))
      {
         Game.onClientLeaveGame(%this);
      }
   }

   // Need to supersede this "core" function in order to properly re-spawn a
   // player after he/she is killed.
   // This will also allow the differing gametypes to more easily have a unique
   // method for spawn handling without needless duplication of code.
   function GameConnection::spawnPlayer(%this, %spawnPoint)
   {
      Game.spawnPlayer(%this, %spawnPoint);
   }
};
// end of our package... now activate it!
activatePackage(GameCore);

// ----------------------------------------------------------------------------
//  Game Control Functions
// ----------------------------------------------------------------------------

function GameCore::onMissionLoaded(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onMissionLoaded");

   //set up the game and game variables
   %game.initGameVars(%game);

   $Game::Duration = %game.duration;
   $Game::EndGameScore = %game.endgameScore;
   $Game::EndGamePause = %game.endgamePause;

   physicsStartSimulation("server");
   %game.startGame();
}

function GameCore::onMissionEnded(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onMissionEnded");

   // Called by endMission(), right before the mission is destroyed

   // Normally the game should be ended first before the next
   // mission is loaded, this is here in case loadMission has been
   // called directly.  The mission will be ended if the server
   // is destroyed, so we only need to cleanup here.

   physicsStopSimulation("server");
   %game.endGame();

   cancel($Game::Schedule);
   $Game::Running = false;
   $Game::Cycling = false;
}

function GameCore::onMissionReset(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onMissionReset");
}

function GameCore::startGame(%game)
{
   // This is where the game play should start

   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onStartGame");
   if ($Game::Running)
   {
      error("startGame: End the game first!");
      return;
   }

   // Inform the client we're starting up
   for (%clientIndex = 0; %clientIndex < ClientGroup.getCount(); %clientIndex++)
   {
      %cl = ClientGroup.getObject(%clientIndex);
      commandToClient(%cl, 'GameStart');

      // Other client specific setup..
      %cl.score = 0;
   }

   // Start the game timer
   if ($Game::Duration)
      $Game::Schedule = %game.schedule($Game::Duration * 1000, "onGameDurationEnd");
   $Game::Running = true;

//    // Start the AIManager
//    new ScriptObject(AIManager) {};
//    MissionCleanup.add(AIManager);
//    AIManager.think();
}

function GameCore::endGame(%game, %client)
{
   // This is where the game play should end

   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::endGame");

   if (!$Game::Running)
   {
      error("endGame: No game running!");
      return;
   }

//    // Stop the AIManager
//    AIManager.delete();

   // Stop any game timers
   cancel($Game::Schedule);

   for (%clientIndex = 0; %clientIndex < ClientGroup.getCount(); %clientIndex++)
   {
      %cl = ClientGroup.getObject(%clientIndex);
      commandToClient(%cl, 'GameEnd');
   }

   $Game::Running = false;
}

function GameCore::onGameDurationEnd()
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onGameDurationEnd");
   if ($Game::Duration && (!EditorIsActive() && !GuiEditorIsActive()))
      cycleGame();
}

// ----------------------------------------------------------------------------
//  Game Setup
// ----------------------------------------------------------------------------

function GameCore::initGameVars(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::initGameVars");

   //-----------------------------------------------------------------------------
   // What kind of "player" is spawned is either controlled directly by the
   // SpawnSphere or it defaults back to the values set here. This also controls
   // which SimGroups to attempt to select the spawn sphere's from by walking down
   // the list of SpawnGroups till it finds a valid spawn object.
   //-----------------------------------------------------------------------------
   $Game::DefaultPlayerClass = "Player";
   $Game::DefaultPlayerDataBlock = "DefaultPlayerData";
   $Game::DefaultPlayerSpawnGroups = "PlayerSpawnPoints PlayerDropPoints";

   //-----------------------------------------------------------------------------
   // What kind of "camera" is spawned is either controlled directly by the
   // SpawnSphere or it defaults back to the values set here. This also controls
   // which SimGroups to attempt to select the spawn sphere's from by walking down
   // the list of SpawnGroups till it finds a valid spawn object.
   //-----------------------------------------------------------------------------
   $Game::DefaultCameraClass = "Camera";
   $Game::DefaultCameraDataBlock = "Observer";
   $Game::DefaultCameraSpawnGroups = "CameraSpawnPoints PlayerSpawnPoints PlayerDropPoints";

   // Set the gameplay parameters
   %game.duration = 0;
   %game.endgameScore = 0;
   %game.endgamePause = 0;
}

// ----------------------------------------------------------------------------
//  Client Management
// ----------------------------------------------------------------------------

function GameCore::onClientEnterGame(%game, %client)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onClientEntergame");

   // Sync the client's clocks to the server's
   commandToClient(%client, 'SyncClock', $Sim::Time - $Game::StartTime);

   // Find a spawn point for the camera
   // This function currently relies on some helper functions defined in
   // core/scripts/server/spawn.cs. For custom spawn behaviors one can either
   // override the properties on the SpawnSphere's or directly override the
   // functions themselves.
   %cameraSpawnPoint = pickCameraSpawnPoint($Game::DefaultCameraSpawnGroups);
   // Spawn a camera for this client using the found %spawnPoint
   %client.spawnCamera(%cameraSpawnPoint);

   // Setup game parameters, the onConnect method currently starts
   // everyone with a 0 score.
   %client.score = 0;
   %client.kills = 0;
   %client.deaths = 0;

   // weaponHUD
   %client.RefreshWeaponHud(0, "", "");

   // Prepare the player object.
   %game.preparePlayer(%client);
}

function GameCore::onClientLeaveGame(%game, %client)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onClientLeaveGame");

   // Cleanup the camera
   if (isObject(%client.camera))
      %client.camera.delete();
   // Cleanup the player
   if (isObject(%client.player))
      %client.player.delete();
}

// Added this stage to creating a player so game types can override it easily.
// This is a good place to initiate team selection.
function GameCore::preparePlayer(%game, %client)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::preparePlayer");

   // Find a spawn point for the player
   // This function currently relies on some helper functions defined in
   // core/scripts/spawn.cs. For custom spawn behaviors one can either
   // override the properties on the SpawnSphere's or directly override the
   // functions themselves.
   %playerSpawnPoint = pickPlayerSpawnPoint($Game::DefaultPlayerSpawnGroups);
   // Spawn a camera for this client using the found %spawnPoint
   //%client.spawnPlayer(%playerSpawnPoint);
   %game.spawnPlayer(%client, %playerSpawnPoint);

   // Starting equipment
   %game.loadOut(%client.player);
}

function GameCore::loadOut(%game, %player)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::loadOut");

   %player.setInventory(RocketLauncher, %player.maxInventory(RocketLauncher));
   %player.setInventory(RocketLauncherAmmo, %player.maxInventory(RocketLauncherAmmo));

   %player.setInventory(GrenadeLauncher, %player.maxInventory(GrenadeLauncher));
   %player.setInventory(GrenadeLauncherAmmo, %player.maxInventory(GrenadeLauncherAmmo));
   
   %player.setInventory(Rifle, %player.maxInventory(Rifle));
   %player.setInventory(BulletAmmo, %player.maxInventory(BulletAmmo));

   %player.mountImage(%player.getDatablock().mainWeapon.image, 0);
}

function GameCore::onDeath(%game, %client, %sourceObject, %sourceClient, %damageType, %damLoc)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::onDeath");
   
   // clear the weaponHUD
   %client.RefreshWeaponHud(0, "", "");

   // Clear out the name on the corpse
   %client.player.setShapeName("");

   // Update the numerical Health HUD
   %client.player.updateHealth();

   // Switch the client over to the death cam and unhook the player object.
   if (isObject(%client.camera) && isObject(%client.player))
   {
      %client.camera.setMode("Corpse", %client.player);
      %client.setControlObject(%client.camera);
   }
   %client.player = 0;

   // Dole out points and display an appropriate message
   
   if (%damageType $= "Suicide" || %sourceClient == %client)
   {
      game.incScore( %client, -1, true );
      game.incDeaths( %client, 1, true );
      
      messageAll('MsgClientScoreChanged', "", %client.score, %client.kills, %client.deaths, %client);
      messageAll('MsgClientKilled', '%1 takes his own life!', %client.playerName);
   }
   else
   {
      game.incScore( %sourceClient, 1, true );
      game.incKills( %sourceClient, 1, true );
      game.incDeaths( %client, 1, true );
      
      messageAll('MsgClientScoreChanged', "", %sourceClient.score, %sourceClient.kills, %sourceClient.deaths, %sourceClient);
      messageAll('MsgClientScoreChanged', "", %client.score, %client.kills, %client.deaths, %client);
      messageAll('MsgClientKilled', '%1 gets nailed by %2!', %client.playerName, %sourceClient.playerName);

      // If the game may be ended by a client getting a particular score, check that now.
      if( $Game::EndGameScore > 0 && %sourceClient.score >= $Game::EndGameScore  )
         cycleGame();
   }
}

// ----------------------------------------------------------------------------
// Scoring
// ----------------------------------------------------------------------------

function GameCore::incKills(%game, %client, %kill, %dontMessageAll)
{
   %client.kills += %kill;
   
   if( !%dontMessageAll )
      messageAll('MsgClientScoreChanged', "", %client.score, %client.kills, %client.deaths, %client);
}

function GameCore::incDeaths(%game, %client, %death, %dontMessageAll)
{
   %client.deaths += %death;

   if( !%dontMessageAll )
      messageAll('MsgClientScoreChanged', "", %client.score, %client.kills, %client.deaths, %client);
}

function GameCore::incScore(%game, %client, %score, %dontMessageAll)
{
   %client.score += %score;

   if( !%dontMessageAll )
      messageAll('MsgClientScoreChanged', "", %client.score, %client.kills, %client.deaths, %client);
}

function GameCore::getScore(%client) { return %client.score; }
function GameCore::getKills(%client) { return %client.kills; }
function GameCore::getDeaths(%client) { return %client.deaths; }

// ----------------------------------------------------------------------------
// Spawning
// ----------------------------------------------------------------------------

function GameCore::spawnPlayer(%game, %client, %spawnPoint, %noControl)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::spawnPlayer");

   if (isObject(%client.player))
   {
      // The client should not already have a player. Assigning
      // a new one could result in an uncontrolled player object.
      error("Attempting to create a player for a client that already has one!");
   }

   // Attempt to treat %spawnPoint as an object
   if (getWordCount(%spawnPoint) == 1 && isObject(%spawnPoint))
   {
      // Defaults
      %spawnClass      = $Game::DefaultPlayerClass;
      %spawnDataBlock  = $Game::DefaultPlayerDataBlock;

      // Overrides by the %spawnPoint
      if (isDefined("%spawnPoint.spawnClass"))
      {
         %spawnClass = %spawnPoint.spawnClass;
         %spawnDataBlock = %spawnPoint.spawnDatablock;
      }
      else if (isDefined("%spawnPoint.spawnDatablock"))
      {
         // This may seem redundant given the above but it allows
         // the SpawnSphere to override the datablock without
         // overriding the default player class
         %spawnDataBlock = %spawnPoint.spawnDatablock;
      }

      %spawnProperties = %spawnPoint.spawnProperties;
      %spawnScript     = %spawnPoint.spawnScript;

      // Spawn with the engine's Sim::spawnObject() function
      %player = spawnObject(%spawnClass, %spawnDatablock, "",
                            %spawnProperties, %spawnScript);

      // If we have an object do some initial setup
      if (isObject(%player))
      {
         // Set the transform to %spawnPoint's transform
         %player.setTransform(%spawnPoint.getTransform());
      }
      else
      {
         // If we weren't able to create the player object then warn the user
         // When the player clicks OK in one of these message boxes, we will fall through
         // to the "if (!isObject(%player))" check below.
         if (isDefined("%spawnDatablock"))
         {
               MessageBoxOK("Spawn Player Failed",
                             "Unable to create a player with class " @ %spawnClass @
                             " and datablock " @ %spawnDatablock @ ".\n\nStarting as an Observer instead.",
                             "");
         }
         else
         {
               MessageBoxOK("Spawn Player Failed",
                              "Unable to create a player with class " @ %spawnClass @
                              ".\n\nStarting as an Observer instead.",
                              "");
         }
      }
   }
   else
   {
      // Create a default player
      %player = spawnObject($Game::DefaultPlayerClass, $Game::DefaultPlayerDataBlock);

      // Treat %spawnPoint as a transform
      %player.setTransform(%spawnPoint);
   }

   // If we didn't actually create a player object then bail
   if (!isObject(%player))
   {
      // Make sure we at least have a camera
      %client.spawnCamera(%spawnPoint);

      return;
   }

   // Update the default camera to start with the player
   if (isObject(%client.camera) && !isDefined("%noControl"))
   {
      if (%player.getClassname() $= "Player")
         %client.camera.setTransform(%player.getEyeTransform());
      else
         %client.camera.setTransform(%player.getTransform());
   }

   // Add the player object to MissionCleanup so that it
   // won't get saved into the level files and will get
   // cleaned up properly
   MissionCleanup.add(%player);

   // Store the client object on the player object for
   // future reference
   %player.client = %client;

   // Player setup...
   if (%player.isMethod("setShapeName"))
      %player.setShapeName(%client.playerName);

   if (%player.isMethod("setEnergyLevel"))
      %player.setEnergyLevel(%player.getDataBlock().maxEnergy);

   // Give the client control of the player
   %client.player = %player;

   // Give the client control of the camera if in the editor
   if( $startWorldEditor )
   {
      %control = %client.camera;
      %control.mode = "Fly";
      EditorGui.syncCameraGui();
   }
   else
      %control = %player;

   // Allow the player/camera to receive move data from the GameConnection.  Without this
   // the user is unable to control the player/camera.
   if (!isDefined("%noControl"))
      %client.setControlObject(%control);
}

// ----------------------------------------------------------------------------
// Observer
// ----------------------------------------------------------------------------

function GameCore::spawnObserver(%game, %client)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::spawnObserver");

   // Position the camera on one of our observer spawn points
   %client.camera.setTransform(%game.pickObserverSpawnPoint());

   // Set control to the camera
   %client.setControlObject(%client.camera);
}

function GameCore::pickObserverSpawnPoint(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> GameCore::pickObserverSpawnPoint");

   %groupName = "MissionGroup/ObserverSpawnPoints";
   %group = nameToID(%groupName);

   if (%group != -1)
   {
      %count = %group.getCount();
      if (%count != 0)
      {
         %index = getRandom(%count-1);
         %spawn = %group.getObject(%index);
         return %spawn.getTransform();
      }
      else
         error("No spawn points found in "@ %groupName);
   }
   else
      error("Missing spawn points group "@ %groupName);

   // Could be no spawn points, in which case we'll stick the
   // player at the center of the world.
   return "0 0 300 1 0 0 0";
}

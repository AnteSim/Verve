//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// DeathmatchGame
// ----------------------------------------------------------------------------
// Depends on methods found in gameCore.cs.  Those added here are specific to
// this game type and/or over-ride the "default" game functionaliy.
//
// The desired Game Type must be added to each mission's LevelInfo object.
//   - gameType = "Deathmatch";
// If this information is missing then the GameCore will default to Deathmatch.
// ----------------------------------------------------------------------------

function DeathMatchGame::onMissionLoaded(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> DeathMatchGame::onMissionLoaded");

   $Server::MissionType = "DeathMatch";
   parent::onMissionLoaded(%game);
}

function DeathMatchGame::initGameVars(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> DeathMatchGame::initGameVars");

   //-----------------------------------------------------------------------------
   // What kind of "player" is spawned is either controlled directly by the
   // SpawnSphere or it defaults back to the values set here. This also controls
   // which SimGroups to attempt to select the spawn sphere's from by walking down
   // the list of SpawnGroups till it finds a valid spawn object.
   // These override the values set in core/scripts/server/spawn.cs
   //-----------------------------------------------------------------------------
   $Game::defaultPlayerClass = "Player";
   $Game::defaultPlayerDataBlock = "DefaultPlayerData";
   $Game::defaultPlayerSpawnGroups = "PlayerSpawnPoints PlayerDropPoints";

   //-----------------------------------------------------------------------------
   // What kind of "camera" is spawned is either controlled directly by the
   // SpawnSphere or it defaults back to the values set here. This also controls
   // which SimGroups to attempt to select the spawn sphere's from by walking down
   // the list of SpawnGroups till it finds a valid spawn object.
   // These override the values set in core/scripts/server/spawn.cs
   //-----------------------------------------------------------------------------
   $Game::defaultCameraClass = "Camera";
   $Game::defaultCameraDataBlock = "Observer";
   $Game::defaultCameraSpawnGroups = "CameraSpawnPoints PlayerSpawnPoints PlayerDropPoints";

   // Set the gameplay parameters
   %game.duration = 30 * 60;
   %game.endgameScore = 20;
   %game.endgamePause = 10;
}

function DeathMatchGame::startGame(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> DeathMatchGame::startGame");

   parent::startGame(%game);
}

function DeathMatchGame::endGame(%game)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> DeathMatchGame::endGame");

   parent::endGame(%game);
}

function DeathMatchGame::onGameDurationEnd()
{
   //echo (%game @"\c4 -> "@ %game.class @" -> DeathMatchGame::onGameDurationEnd");

   parent::onGameDurationEnd();
}

function DeathMatchGame::onClientEnterGame(%game, %client)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> DeathMatchGame::onClientEnterGame");

   parent::onClientEnterGame(%game, %client);
}

function DeathMatchGame::onClientLeaveGame(%game, %client)
{
   //echo (%game @"\c4 -> "@ %game.class @" -> DeathMatchGame::onClientLeaveGame");

   parent::onClientLeaveGame(%game, %client);

}

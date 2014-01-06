//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function initializeBase()
{
   echo(" % - Initializing Base Editor");
   
   // Load Custom Editors
   loadDirectory( expandFilename( "./canvas" ) );
   loadDirectory( expandFilename( "./menuBar" ) );
   loadDirectory( expandFilename( "./utils" ) );
}

function destroyBase()
{
}

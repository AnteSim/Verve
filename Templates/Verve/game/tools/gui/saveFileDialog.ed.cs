//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function getSaveFilename( %filespec, %callback, %currentFile, %overwrite )
{
   if( %overwrite $= "" )
      %overwrite = true;
   
   %dlg = new SaveFileDialog()
   {
      Filters = %filespec;
      DefaultFile = %currentFile;
      ChangePath = false;
      OverwritePrompt = %overwrite;
   };
   
   if( filePath( %currentFile ) !$= "" )
      %dlg.DefaultPath = filePath( %currentFile );
   else
      %dlg.DefaultPath = getMainDotCSDir();
      
   if( %dlg.Execute() )
   {
      %filename = %dlg.FileName;
      eval( %callback @ "(\"" @ %filename @ "\");" );
   }
   
   %dlg.delete();
}

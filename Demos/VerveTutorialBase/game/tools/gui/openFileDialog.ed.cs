//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function getLoadFilename(%filespec, %callback, %currentFile)
{   
   %dlg = new OpenFileDialog()
   {
      Filters = %filespec;
      DefaultFile = %currentFile;
      ChangePath = false;
      MustExist = true;
      MultipleFiles = false;
   };
   
   if ( filePath( %currentFile ) !$= "" )
      %dlg.DefaultPath = filePath(%currentFile);  
      
   if ( %dlg.Execute() )
   {
      eval(%callback @ "(\"" @ %dlg.FileName @ "\");");
      $Tools::FileDialogs::LastFilePath = filePath( %dlg.FileName );
   }
   
   %dlg.delete();
}

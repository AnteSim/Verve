//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------


function Editor::validateObjectName( %name, %mustHaveName )
{
   if( %mustHaveName && %name $= "" )
   {
      MessageBoxOK( "Missing Object Name", "No name given for object.  Please enter a valid object name." );
      return false;
   }
   if( !isValidObjectName( %name ) )
   {
      MessageBoxOK( "Invalid Object Name", "'" @ %name @ "' is not a valid object name." NL
         "" NL
         "Please choose a name that beings with a letter or underscore and is otherwise comprised exclusively of letters, digits, and/or underscores."
      );
      return false;
   }
   if( isObject( %name ) )
   {
      MessageBoxOK( "Invalid Object Name", "An object with the name '" @ %name @ "' already exists.  Please choose another name." );
      return false;
   }
   if( isClass( %name ) )
   {
      MessageBoxOK( "Invalid Object Name", "'" @ %name @ "' is the name of an existing TorqueScript class.  Please choose another name." );
      return false;
   }

   return true;
}

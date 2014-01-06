//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------
// Define the field types for objects that link to the namespace MissionInfo
function SimObject::onDefineFieldTypes( %this )
{
   %this.setFieldType("Locked", "TypeBool");   
}
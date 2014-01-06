//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

uniform mat4 modelview;

varying vec2 tex0;

void main()
{
   gl_Position = modelview * gl_Vertex;
   tex0 = gl_MultiTexCoord0.st;
}
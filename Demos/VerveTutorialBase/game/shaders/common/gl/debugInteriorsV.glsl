//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

uniform mat4 modelview;

varying vec2 TEX0;

void main()
{
   gl_Position = modelview * gl_Vertex;
   TEX0 = gl_MultiTexCoord0.st;
}



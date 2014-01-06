//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//*****************************************************************************
// Glow shader
//*****************************************************************************

uniform mat4 modelview;
uniform vec2 offset0, offset1, offset2, offset3;

varying vec2 texc0, texc1, texc2, texc3;

void main()
{
   gl_Position = modelview * gl_Vertex;
   
   vec2 tc = gl_MultiTexCoord0.st;
   tc.y = 1.0 - tc.y;
   
   texc0 = tc + offset0;
   texc1 = tc + offset1;
   texc2 = tc + offset2;
   texc3 = tc + offset3;
}
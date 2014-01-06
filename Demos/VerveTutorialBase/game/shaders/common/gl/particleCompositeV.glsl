//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

uniform mat4 modelViewProj;
uniform mat4 targetModelViewProj;

varying vec4 offscreenPos;
varying vec4 backbufferPos;

void main()
{
   gl_Position = modelViewProj * gl_Vertex;
   backbufferPos = gl_Position;
   offscreenPos = targetModelViewProj * gl_Vertex;
}


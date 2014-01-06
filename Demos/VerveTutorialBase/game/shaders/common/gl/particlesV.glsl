//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

varying vec4 color;
varying vec2 uv0;
varying vec4 pos;

uniform mat4 modelViewProj;
uniform mat4 fsModelViewProj;

void main()
{
   gl_Position = modelViewProj * gl_Vertex;
   pos = fsModelViewProj * gl_Vertex;
   color = gl_Color;
   uv0 = gl_MultiTexCoord0.st;
}


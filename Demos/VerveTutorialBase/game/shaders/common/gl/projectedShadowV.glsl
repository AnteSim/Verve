//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//*****************************************************************************
// Precipitation vertex shader
//*****************************************************************************

varying vec2 texCoord;
varying vec4 color;
varying float fade;

uniform mat4 modelview;
uniform float shadowLength;
uniform vec3 shadowCasterPosition;

void main()
{
   gl_Position = modelview * vec4(gl_Vertex.xyz, 1.0);
   
   color = gl_Color;
   texCoord = gl_MultiTexCoord1.st;
   
   float fromCasterDist = length(gl_Vertex.xyz - shadowCasterPosition) - shadowLength;
   fade = 1.0 - clamp(fromCasterDist/shadowLength, 0.0, 1.0);
}

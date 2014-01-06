//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "../../../../../../shaders/common/gl/torque.glsl"

uniform vec2 oneOverTargetSize;
uniform vec4 rtParams0;

varying vec2 uv;

void main()
{
	gl_Position = gl_Vertex;   
   uv = viewportCoordToRenderTarget( gl_MultiTexCoord0.st, rtParams0 ); 
}

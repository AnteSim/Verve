//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "shadergen:/autogenConditioners.h"


varying vec2 uv0;
uniform sampler2D prepassTex;

void main()
{   
   vec3 normal = prepassUncondition( prepassTex, uv0 ).xyz;
   gl_FragColor = vec4( ( normal + 1.0 ) * 0.5, 1.0 );
}
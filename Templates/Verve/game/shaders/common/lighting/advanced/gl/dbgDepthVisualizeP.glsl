//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "shadergen:/autogenConditioners.h"

varying vec2 uv0;
uniform sampler2D prepassBuffer;
uniform sampler1D depthViz;

void main()
{
   float depth = prepassUncondition( prepassBuffer, uv0 ).w;
   gl_FragColor = vec4( texture1D( depthViz, depth ).rgb, 1 );
}
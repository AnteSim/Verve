//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "shadergen:/autogenConditioners.h"
#include "../../postfx/postFx.hlsl"


float4 main( PFXVertToPix IN,
             uniform sampler2D prepassTex : register(S0),
             uniform sampler1D depthViz : register(S1) ) : COLOR0
{
   float depth = prepassUncondition( prepassTex, IN.uv0 ).w;
   return float4( tex1D( depthViz, depth ).rgb, 1.0 );
}

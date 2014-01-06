//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "shadergen:/autogenConditioners.h"
#include "../../postfx/postFx.hlsl"


float4 main( PFXVertToPix IN,
             uniform sampler2D prepassTex : register(S0) ) : COLOR0
{   
   float3 normal = prepassUncondition( prepassTex, IN.uv0 ).xyz;   
   return float4( ( normal + 1.0 ) * 0.5, 1.0 );
}
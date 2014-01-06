//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "shadergen:/autogenConditioners.h"
#include "./postFx.hlsl"
#include "./../torque.hlsl"

uniform sampler2D prepassTex : register(S0);
uniform float3    eyePosWorld;
uniform float4    fogColor;
uniform float3    fogData;
uniform float4    rtParams0;

float4 main( PFXVertToPix IN ) : COLOR
{   
   //float2 prepassCoord = ( IN.uv0.xy * rtParams0.zw ) + rtParams0.xy;   
   float depth = prepassUncondition( prepassTex, IN.uv0 ).w;
   //return float4( depth, 0, 0, 0.7 );
   
   // Skip fogging the extreme far plane so that 
   // the canvas clear color always appears.
   clip( 0.9999 - depth );
   
   float factor = computeSceneFog( eyePosWorld,
                                   eyePosWorld + ( IN.wsEyeRay * depth ),
                                   fogData.x, 
                                   fogData.y, 
                                   fogData.z );

   return hdrEncode( float4( fogColor.rgb, 1.0 - saturate( factor ) ) );     
}
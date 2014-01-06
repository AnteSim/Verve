//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "./postFx.hlsl"

uniform sampler2D inputTex : register(S0);

float4 main( PFXVertToPix IN ) : COLOR
{
   return tex2D( inputTex, IN.uv0 );   
}
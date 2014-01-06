//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

float4 main( float4 color_in : COLOR0, 
             float2 texCoord_in : TEXCOORD0,
             uniform sampler2D diffuseMap : register(S0) ) : COLOR0
{
   return tex2D(diffuseMap, texCoord_in) * color_in;
}
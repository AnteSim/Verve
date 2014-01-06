//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

struct MaterialDecoratorConnectV
{
   float2 uv0 : TEXCOORD0;
};

float4 main( MaterialDecoratorConnectV IN, 
             uniform sampler2D shadowMap : register(S0),
             uniform sampler1D depthViz  : register(S1) ) : COLOR0
{   
   float depth = saturate( tex2D( shadowMap, IN.uv0 ).r );
   return float4( tex1D( depthViz, depth ).rgb, 1 );
}
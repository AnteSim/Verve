//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "./../postFx.hlsl"

float4 main( PFXVertToPix IN ) : COLOR
{  
   float power = pow( max( IN.uv0.x, 0 ), 0.1 );   
   return float4( power, 0, 0, 1 );   
}
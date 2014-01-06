//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

float4 main( const float2 inPosition : POSITION ) : POSITION
{
   return float4( inPosition, 0, 1 );
}
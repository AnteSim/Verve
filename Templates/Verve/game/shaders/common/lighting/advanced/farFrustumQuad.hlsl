//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

struct FarFrustumQuadConnectV
{
   float4 hpos : POSITION;
   float2 uv0 : TEXCOORD0;
   float3 wsEyeRay : TEXCOORD1;
   float3 vsEyeRay : TEXCOORD2;
};

struct FarFrustumQuadConnectP
{
   float2 uv0 : TEXCOORD0;
   float3 wsEyeRay : TEXCOORD1;
   float3 vsEyeRay : TEXCOORD2;
};


float2 getUVFromSSPos( float3 ssPos, float4 rtParams )
{
	float2 outPos = ( ssPos.xy + 1.0 ) / 2.0;
	outPos.y = 1.0 - outPos.y;
	outPos = ( outPos * rtParams.zw ) + rtParams.xy;
	return outPos;
}

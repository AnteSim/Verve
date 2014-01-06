//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "../../hlslStructs.h"
#include "farFrustumQuad.hlsl"


FarFrustumQuadConnectV main( VertexIn_PNT IN,
                             uniform float4 rtParams0 )
{
   FarFrustumQuadConnectV OUT;

   OUT.hpos = float4( IN.uv0, 0, 1 );

   // Get a RT-corrected UV from the SS coord
   OUT.uv0 = getUVFromSSPos( OUT.hpos.xyz, rtParams0 );
   
   // Interpolators will generate eye rays the 
   // from far-frustum corners.
   OUT.wsEyeRay = IN.pos.xyz;
   OUT.vsEyeRay = IN.normal.xyz;

   return OUT;
}

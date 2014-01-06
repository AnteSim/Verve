//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "../../../../../../shaders/common/postFx/postFx.hlsl"
#include "../../../../../../shaders/common/torque.hlsl"

float4 rtParams0;

struct VertToPix
{
   float4 hpos       : POSITION;
   float2 uv        : TEXCOORD0;
};

VertToPix main( PFXVert IN )
{
   VertToPix OUT;
   
   OUT.hpos = IN.pos;
   OUT.uv = viewportCoordToRenderTarget( IN.uv, rtParams0 );
               
   return OUT;
}

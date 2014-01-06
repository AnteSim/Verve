//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "hlslStructs.h"

struct MaterialDecoratorConnectV
{
   float4 hpos : POSITION;
   float2 uv0 : TEXCOORD0;
};

//-----------------------------------------------------------------------------
// Main                                                                        
//-----------------------------------------------------------------------------
MaterialDecoratorConnectV main( VertexIn_PCT IN,
                                    uniform float4x4 modelview : register(C0) )
{
   MaterialDecoratorConnectV OUT;

   OUT.hpos = mul(modelview, IN.pos);
   OUT.uv0 = IN.uv0;

   return OUT;
}

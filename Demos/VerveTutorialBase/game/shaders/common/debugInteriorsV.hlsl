//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "hlslStructs.h"

//-----------------------------------------------------------------------------
// Structures                                                                  
//-----------------------------------------------------------------------------

struct ConnectData
{
   float4 hpos            : POSITION;
   float2 outTexCoord     : TEXCOORD0;
};


//-----------------------------------------------------------------------------
// Main                                                                        
//-----------------------------------------------------------------------------
ConnectData main( VertexIn_PNTTTB IN,
                  uniform float4x4 modelview       : register(C0)
)
{
   ConnectData OUT;

   OUT.hpos = mul(modelview, IN.pos);
   OUT.outTexCoord = IN.uv0;

   return OUT;
}

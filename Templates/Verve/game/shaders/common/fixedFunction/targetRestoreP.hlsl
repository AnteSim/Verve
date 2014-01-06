//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

uniform sampler2D colorTarget0Texture : register(s0);

float4 main( float2 ScreenPos : VPOS ) : COLOR0
{
   float2 TexCoord = ScreenPos;
   float4 diffuse;
   asm { tfetch2D diffuse, TexCoord, colorTarget0Texture, UnnormalizedTextureCoords = true };
   return diffuse;
}
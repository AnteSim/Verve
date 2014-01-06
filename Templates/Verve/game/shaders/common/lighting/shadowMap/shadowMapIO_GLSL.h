//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//#define SM_Fmt_R8G8B8A8

#define pkDepthBitShft 65536.0
#define pkDepthChanMax 256.0
#define bias -0.5/255.0
#define coeff 0.9999991
//#define coeff 1.0

vec4 encodeShadowMap( float depth )
{
#if defined(SM_Fmt_R8G8B8A8)   
   return frac( vec4(1.0, 255.0, 65025.0, 160581375.0) * depth ) + vec4(bias);

   //float4 packedValue = frac((depth / coeff) * float4(16777216.0, 65536.0, 256.0, 1.0));
   //return (packedValue - packedValue.xxyz * float4(0, 1.0 / 256, 1.0 / 256, 1.0 / 256));
#else
   return vec4(depth);
#endif
}

float decodeShadowMap( vec4 smSample )
{
#if defined(SM_Fmt_R8G8B8A8)
   return dot( smSample, vec4(1.0, 1/255.0, 1/65025.0, 1/160581375.0) );
#else
   return smSample.x;  
#endif
}

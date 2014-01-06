//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "torque.hlsl"
   
// With advanced lighting we get soft particles.
#ifdef TORQUE_LINEAR_DEPTH
   #define SOFTPARTICLES  
#endif

#ifdef SOFTPARTICLES
   
   #include "shadergen:/autogenConditioners.h"
   
   uniform float oneOverSoftness;
   uniform float oneOverFar;
   uniform sampler2D prepassTex : register(S1);   
   //uniform float3 vEye;
   uniform float4 prePassTargetParams;
#endif

#define CLIP_Z // TODO: Make this a proper macro

struct Conn
{
   float4 color : TEXCOORD0;
   float2 uv0 : TEXCOORD1;
	float4 pos : TEXCOORD2;
};

uniform sampler2D diffuseMap : register(S0);

uniform sampler2D paraboloidLightMap : register(S2);

float4 lmSample( float3 nrm )
{
   bool calcBack = (nrm.z < 0.0);
   if ( calcBack )
      nrm.z = nrm.z * -1.0;

   float2 lmCoord;
   lmCoord.x = (nrm.x / (2*(1 + nrm.z))) + 0.5;
   lmCoord.y = 1-((nrm.y / (2*(1 + nrm.z))) + 0.5);


   // If this is the back, offset in the atlas
   if ( calcBack )
      lmCoord.x += 1.0;
      
   // Atlasing front and back maps, so scale
   lmCoord.x *= 0.5;

   return tex2D(paraboloidLightMap, lmCoord);
}


uniform float alphaFactor;
uniform float alphaScale;

float4 main( Conn IN ) : COLOR
{
   float softBlend = 1;
   
   #ifdef SOFTPARTICLES
      float2 tc = IN.pos.xy * float2(1.0, -1.0) / IN.pos.w;
      tc = viewportCoordToRenderTarget(saturate( ( tc + 1.0 ) * 0.5 ), prePassTargetParams); 
   
   	float sceneDepth = prepassUncondition( prepassTex, tc ).w;   	   	   			
   	float depth = IN.pos.w * oneOverFar;   	
	float diff = sceneDepth - depth;
	#ifdef CLIP_Z
	   // If drawing offscreen, this acts as the depth test, since we don't line up with the z-buffer
	   // When drawing high-res, though, we want to be able to take advantage of hi-z
	   // so this is #ifdef'd out
	   //clip(diff);
	#endif
      softBlend = saturate( diff * oneOverSoftness );
   #endif
	   
   float4 diffuse = tex2D( diffuseMap, IN.uv0 );
   
   //return float4( lmSample(float3(0, 0, -1)).rgb, IN.color.a * diffuse.a * softBlend * alphaScale);
   
   // Scale output color by the alpha factor (turn LerpAlpha into pre-multiplied alpha)
   float3 colorScale = ( alphaFactor < 0.0 ? IN.color.rgb * diffuse.rgb : ( alphaFactor > 0.0 ? IN.color.a * diffuse.a * alphaFactor * softBlend : softBlend ) );
   
   return hdrEncode( float4( IN.color.rgb * diffuse.rgb * colorScale,
                  IN.color.a * diffuse.a * softBlend * alphaScale ) );
}


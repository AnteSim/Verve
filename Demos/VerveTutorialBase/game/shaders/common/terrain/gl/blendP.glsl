//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "../terrain.glsl"
#include "../../gl/hlslCompat.glsl"

varying vec2 layerCoord;
varying vec2 texCoord;

uniform sampler2D layerTex;
uniform sampler2D textureMap;
uniform float texId;
uniform float layerSize;

void main()
{
   vec4 layerSample = round(texture2D( layerTex, layerCoord ) * 255.0);

   float blend = calcBlend( texId, layerCoord, layerSize, layerSample );

   if(blend - 0.0001 < 0.0)
      discard;

   gl_FragColor = vec4( texture2D( textureMap, texCoord ).rgb, blend );
}

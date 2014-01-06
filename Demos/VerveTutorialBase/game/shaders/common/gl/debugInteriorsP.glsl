//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

uniform sampler2D diffuseMap;
uniform vec4 shadeColor;

varying vec2 TEX0;

void main()
{
   vec4 diffuseColor = texture2D( diffuseMap, TEX0 );
   
   gl_FragColor = diffuseColor;
   gl_FragColor *= shadeColor;
}

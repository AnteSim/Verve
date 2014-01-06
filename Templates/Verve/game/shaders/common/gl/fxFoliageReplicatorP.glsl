//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Data
//-----------------------------------------------------------------------------
uniform sampler2D diffuseMap, alphaMap;
uniform vec4 groundAlpha;

varying vec4 color, groundAlphaCoeff;
varying vec2 outTexCoord, alphaLookup;

//-----------------------------------------------------------------------------
// Main                                                                        
//-----------------------------------------------------------------------------
void main()
{
   vec4 alpha = texture2D(alphaMap, alphaLookup);
   gl_FragColor = color * texture2D(diffuseMap, outTexCoord);
   gl_FragColor.a = gl_FragColor.a * min(alpha, groundAlpha + groundAlphaCoeff.x).x;
}

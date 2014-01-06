//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

varying vec2 uv0;
uniform sampler2D shadowMap;
uniform sampler1D depthViz;

void main()
{
   float depth = clamp( texture2DLod( shadowMap, uv0, 0 ).r, 0.0, 1.0 );
   gl_FragColor = vec4( texture1D( depthViz, depth ).rgb, 1.0 );
}
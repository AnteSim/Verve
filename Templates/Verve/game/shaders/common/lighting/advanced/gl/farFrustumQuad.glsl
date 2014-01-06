//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------


vec2 getUVFromSSPos( vec3 ssPos, vec4 rtParams )
{
	vec2 outPos = ( ssPos.xy + 1.0 ) / 2.0;
	outPos = ( outPos * rtParams.zw ) + rtParams.xy;
	//outPos.y = 1.0 - outPos.y;
	return outPos;
}

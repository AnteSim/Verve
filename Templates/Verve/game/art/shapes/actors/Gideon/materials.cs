//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

singleton Material( actor_gideon_head_material_dts )
{
	mapTo = "head_d";
	diffuseMap[0] = "head_d";
	normalMap[0] = "head_n";
	specularMap[0] = "head_s";
	pixelSpecular[0] = true;
	specular[0] = "0.99 0.91 0.81";
	specularPower[0] = 16.0;
};

singleton Material( actor_gideon_body_material_dts )
{
	mapTo = "body_d";
	diffuseMap[0] = "body_d";
	specularMap[0] = "body_s";
	normalMap[0] = "body_n";
	pixelSpecular[0] = true;
	specular[0] = "0.99 0.91 0.81";
	specularPower[0] = 16.0;
};

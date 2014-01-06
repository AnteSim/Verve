//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//--- cube.dae MATERIALS BEGIN ---
singleton Material(cube_GridMaterial)
{
	mapTo = "GridMaterial";

	diffuseMap[0] = "grid";
	normalMap[0] = "";
	specularMap[0] = "";

	diffuseColor[0] = "1 1 1 1";
	specular[0] = "0.9 0.9 0.9 1";
	specularPower[0] = 0.415939;
	pixelSpecular[0] = false;
	emissive[0] = false;

	doubleSided = false;
	translucent = false;
	translucentBlendOp = "None";
};

//--- cube.dae MATERIALS END ---


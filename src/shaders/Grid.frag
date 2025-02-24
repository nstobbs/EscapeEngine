#version 450 core

layout(set = 0, binding = 0) uniform SceneUniformBuffer {
    mat4 view;
    mat4 proj;
} scene;

layout(set = 1, binding = 0) uniform ObjectUniformBuffer {
    mat4 model;
} object;

layout(set = 2, binding = 0) uniform sampler _sampler;
layout(set = 2, binding = 1) uniform texture2D textures[1];

layout(push_constant) uniform TextureIndexPush {
    int textureIndex;
}texIndex;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

void main()
{
    // LineColors
	vec4 cellColor = vec4(0.1f, 0.1f, 0.1f, 0.1f);
	vec4 subCellColor = vec4(0.01f, 0.01f, 0.01f, 0.01f);

    // Cell Size
	float cellSize = 0.1f;
	float halfCellSize = cellSize * 0.5f;

	float subCellSize = 0.020f;
	float halfSubCellSize = subCellSize * 0.5f;

	// Line Thickness
	float cellLineThickness = 0.01f;
	float subCellLineThickness = 0.1f;

	// Cell Coords
	vec2 cellCoords = mod(fragTexCoord, cellSize) / cellSize;	
	vec2 subCellCoords = mod(fragTexCoord, subCellSize) / subCellSize;
	
	// Cell Distance
	vec2 distanceToCell = abs(cellCoords - halfCellSize);
	vec2 distanceToSubCell = abs(subCellCoords - halfSubCellSize);

	// Adjusted Thickness
	vec2 d = fwidth(fragTexCoord);
	vec2 adjustedCellLineThickness = 0.5 * (cellLineThickness + d);
	vec2 adjustedSubCellLineThickness = 0.5 * (subCellLineThickness + d);
	
	outColor = vec4(0.005f);
	if (distanceToSubCell.x < adjustedSubCellLineThickness.x ||
		distanceToSubCell.x < adjustedSubCellLineThickness.y ||
		distanceToSubCell.y < adjustedSubCellLineThickness.x ||
		distanceToSubCell.y < adjustedSubCellLineThickness.y)
	{
		outColor = subCellColor;
	};

	if (distanceToCell.x < adjustedCellLineThickness.x || 
		distanceToCell.x < adjustedCellLineThickness.y ||
		distanceToCell.y < adjustedCellLineThickness.x ||
		distanceToCell.y < adjustedCellLineThickness.y)
	{
		outColor = cellColor;
	};
}
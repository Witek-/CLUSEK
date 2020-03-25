#pragma once
#include <DirectXMath.h>

struct DynamicRenderSettings final
{
	DirectX::XMFLOAT3 DirectionalLightColor;
	float DirectionalLightStrength;
	DirectX::XMFLOAT3 DirectionalLightDirection;

	float ClearColor[4] = { 0.0f, 0.75f, 1.0f, 1.0f };

	float MinTerrainTessellationFactor;
	float MaxTerrainTessellationFactor;

	float MinTerrainTessellationDistance;
	float MaxTerrainTessellationDistance;

	DirectX::XMFLOAT3 FogColor;
	float FogDensity;
	DirectX::XMFLOAT3 FogLightColor;
	float FogMinDistance;
	float SkyConstantValue;
};

#pragma once
#include <DirectXMath.h>

struct DynamicRenderSettings final
{
	DirectX::XMFLOAT3 AmbientLightColor;
	float AmbientLightStrength;

	DirectX::XMFLOAT3 DirectionalLightColor;
	float DirectionalLightStrength;
	DirectX::XMFLOAT3 DirectionalLightDirection;

	float ClearColor[3] = { 0.0f, 0.75f, 1.0f };
};
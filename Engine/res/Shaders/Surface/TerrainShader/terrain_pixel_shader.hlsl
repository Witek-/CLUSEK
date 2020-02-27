#include "../../Includes/normal_utils.hlsli"
#include "../../Includes/pbr.hlsli"
#include "../../Includes/shadow_utils.hlsli"

cbuffer TerrainNormalBuffer : register(b0)
{
    float WorldCellSpace;
    float TexelSpace;
    float MaxHeight;
}

cbuffer TerrainUvBuffer : register(b1)
{
    float BaseTextureScale;
    float RedTextureScale;
    float GreenTextureScale;
    float BlueTextureScale;
}

cbuffer LightAndAlphaBuffer : register(b2)
{
    float3 DirectionalLightColor;
    float DirectionalLightStrength;
    float3 DirectionalLightDirection;
    float Alpha;
}

cbuffer CameraBuffer : register(b3)
{
    float3 CameraPosition;
}

cbuffer CascadeLevelsBuffer : register(b4)
{
    float4 CascadeEnds;
    float4 Biases;
}

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float3 WorldPosition : WORLD_POSITION;
    float4 LightSpacePosition[CASCADES_COUNT] : LIGHTSPACE_POSITION;
    float2 TextureCoord : TEXCOORD;
    float CameraDistanceZ : CAMERA_DISTANCE_Z;
};

Texture2D HeightmapTexture : register(t0);
Texture2D SplatmapTexture : register(t1);

Texture2D BaseAlbedoTexture : register(t2);
Texture2D RedAlbedoTexture : register(t3);
Texture2D GreenAlbedoTexture : register(t4);
Texture2D BlueAlbedoTexture : register(t5);

Texture2D BaseNormalTexture : register(t6);
Texture2D RedNormalTexture : register(t7);
Texture2D GreenNormalTexture : register(t8);
Texture2D BlueNormalTexture : register(t9);

Texture2D BaseMetalicSmoothnessTexture : register(t10);
Texture2D RedMetalicSmoothnessTexture : register(t11);
Texture2D GreenMetalicSmoothnessTexture : register(t12);
Texture2D BlueMetalicSmoothnessTexture : register(t13);

Texture2D BaseOcclusionTexture : register(t14);
Texture2D RedOcclusionTexture : register(t15);
Texture2D GreenOcclusionTexture : register(t16);
Texture2D BlueOcclusionTexture : register(t17);

TextureCube IrradianceTexture : register(t18);
TextureCube RadianceTexture : register(t19);
Texture2D BrdfLut : register(t20);

Texture2D ShadowMapCascade0 : register(t21);
Texture2D ShadowMapCascade1 : register(t22);
Texture2D ShadowMapCascade2 : register(t23);
Texture2D ShadowMapCascade3 : register(t24);

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);
SamplerState BrdfSampler : register(s2);
SamplerComparisonState ShadowSampler : register(s3);

float3 CalculateAlbedoColor(PS_INPUT input, float3 splatId)
{
    float3 color = BaseAlbedoTexture.Sample(WrapSampler, input.TextureCoord * BaseTextureScale).rgb;
    color = lerp(color, RedAlbedoTexture.Sample(WrapSampler, input.TextureCoord * RedTextureScale).rgb, splatId.r);
    color = lerp(color, GreenAlbedoTexture.Sample(WrapSampler, input.TextureCoord * GreenTextureScale).rgb, splatId.g);
    color = lerp(color, BlueAlbedoTexture.Sample(WrapSampler, input.TextureCoord * BlueTextureScale).rgb, splatId.b);
    
    return color;
}

float3 CalculateNormalColor(PS_INPUT input, float3 splatId)
{
    float3 color = BaseNormalTexture.Sample(WrapSampler, input.TextureCoord * BaseTextureScale).rgb;
    color = lerp(color, RedNormalTexture.Sample(WrapSampler, input.TextureCoord * RedTextureScale).rgb, splatId.r);
    color = lerp(color, GreenNormalTexture.Sample(WrapSampler, input.TextureCoord * GreenTextureScale).rgb, splatId.g);
    color = lerp(color, BlueNormalTexture.Sample(WrapSampler, input.TextureCoord * BlueTextureScale).rgb, splatId.b);
    
    return color;
}

float2 CalculateMetalicSmoothnessColor(PS_INPUT input, float3 splatId)
{
    float2 color = BaseMetalicSmoothnessTexture.Sample(WrapSampler, input.TextureCoord * BaseTextureScale).ra;
    color = lerp(color, RedMetalicSmoothnessTexture.Sample(WrapSampler, input.TextureCoord * RedTextureScale).ra, splatId.r);
    color = lerp(color, GreenMetalicSmoothnessTexture.Sample(WrapSampler, input.TextureCoord * GreenTextureScale).ra, splatId.g);
    color = lerp(color, BlueMetalicSmoothnessTexture.Sample(WrapSampler, input.TextureCoord * BlueTextureScale).ra, splatId.b);
    
    return color;
}

float CalculateOcclusionColor(PS_INPUT input, float3 splatId)
{
    float color = BaseOcclusionTexture.Sample(WrapSampler, input.TextureCoord * BaseTextureScale).r;
    color = lerp(color, RedOcclusionTexture.Sample(WrapSampler, input.TextureCoord * RedTextureScale).r, splatId.r);
    color = lerp(color, GreenOcclusionTexture.Sample(WrapSampler, input.TextureCoord * GreenTextureScale).r, splatId.g);
    color = lerp(color, BlueOcclusionTexture.Sample(WrapSampler, input.TextureCoord * BlueTextureScale).r, splatId.b);
    
    return color;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 tangent, bitangent, normal;
    CalculateNormalForTerrain(  input.TextureCoord, 
                                HeightmapTexture, 
                                ClampSampler, 
                                WorldCellSpace, 
                                TexelSpace, 
                                MaxHeight, 
                                tangent, 
                                bitangent, 
                                normal);
    
    float3x3 TBN = CalculateTBN(tangent, bitangent, normal);
    
    float3 splatId = SplatmapTexture.SampleLevel(ClampSampler, input.TextureCoord, 0).rgb;
    
    float3 albedoColor = CalculateAlbedoColor(input, splatId);
    float3 normalColor = CalculateNormalColor(input, splatId);
    float2 metalicSmoothnessColor = CalculateMetalicSmoothnessColor(input, splatId);
    float occlusionColor = CalculateOcclusionColor(input, splatId);
    
    float3 calculatedNormal = CalculateNormal(normalColor, TBN);
    float roughness = 1 - metalicSmoothnessColor.y;
    float3 lightColor = DirectionalLightColor * DirectionalLightStrength;
    
    float shadowMultiplier = 1.0f;
    
    if (input.CameraDistanceZ < CascadeEnds[0])
        shadowMultiplier = CalculateShadows(ShadowMapCascade0, ShadowSampler, input.LightSpacePosition[0], Biases[0]);
    else if (input.CameraDistanceZ < CascadeEnds[1])
        shadowMultiplier = CalculateShadows(ShadowMapCascade1, ShadowSampler, input.LightSpacePosition[1], Biases[1]);
    else if (input.CameraDistanceZ < CascadeEnds[2])
        shadowMultiplier = CalculateShadows(ShadowMapCascade2, ShadowSampler, input.LightSpacePosition[2], Biases[2]);
    else if (input.CameraDistanceZ < CascadeEnds[3])
        shadowMultiplier = CalculateShadows(ShadowMapCascade3, ShadowSampler, input.LightSpacePosition[3], Biases[3]);
    
    float3 finalColor = Pbr(albedoColor, calculatedNormal, metalicSmoothnessColor.r, roughness, occlusionColor,
                            IrradianceTexture, RadianceTexture, BrdfLut, WrapSampler, BrdfSampler,
                            DirectionalLightDirection, lightColor, CameraPosition, input.WorldPosition, shadowMultiplier);
    
    return float4(finalColor, Alpha);
}
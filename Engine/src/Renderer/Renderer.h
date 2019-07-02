#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "Vertex.h"
#include "VertexBuffer.h"

class Renderer final
{
public:
	bool Initialize(HWND hwnd, int width, int height, int fullscreen, int syncIntervals, int selectedAdapterId, 
		int refreshRateNumerator, int refreshRateDenominator, int multisamplesCount, int multisamplesQuality);
	void RenderFrame() const;
private:
	bool InitializeDirectX(HWND hwnd, int width, int height, int fullscreen, int selectedAdapterId,
		int refreshRateNumerator, int refreshRateDenominator, int multisamplesCount, int multisamplesQuality);
	bool InitializeShaders();
	bool InitializeScene();

	int SyncIntervals = 1;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;

	VertexShader UberVertexShader;
	PixelShader UberPixelShader;

	VertexBuffer<Vertex> TriangleVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndicesBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ExampleTexture;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> RasterizerState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerState;
};

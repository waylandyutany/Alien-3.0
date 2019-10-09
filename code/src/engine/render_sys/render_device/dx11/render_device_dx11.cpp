#include "StdAfx.h"

#include "render_device_dx11.h"
#include "render_window_dx11.h"
#include "utils_dx11.h"

#include "../../render_sys_init.h"

#include "../../../_sys/templates.h"

#include "../../../debug_sys/profiler/profiler.h"
#include "../../../debug_sys/output/outputs.h"

namespace ae { namespace render { namespace dx11 {

// *****************************************************************************************************
// *****************************************************************************************************

C_ShadowMap::C_ShadowMap() :
	m_DepthTexture(0), m_TextureRTV(0), m_TextureSRV(0),
	m_DepthStencilTexture(0), m_DepthStencilView(0)
{

}

C_ShadowMap::~C_ShadowMap()
{
	Release();
}

void C_ShadowMap::Create(ID3D11Device* device, ae::u32 width, ae::u32 height)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC textureDesc;
	ae::ZeroStruct(&textureDesc);
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	result = device->CreateTexture2D(&textureDesc, NULL, &m_DepthTexture);
	result = device->CreateRenderTargetView(m_DepthTexture, NULL, &m_TextureRTV);
	result = device->CreateShaderResourceView(m_DepthTexture, NULL, &m_TextureSRV);

	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	result = device->CreateTexture2D(&textureDesc, NULL, &m_DepthStencilTexture);
	result = device->CreateDepthStencilView(m_DepthStencilTexture, NULL, &m_DepthStencilView);

	m_ViewPort.Width = (float)width;
	m_ViewPort.Height = (float)height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
}

void C_ShadowMap::Release()
{
	ae::SafeRelease(m_DepthTexture);
	ae::SafeRelease(m_TextureRTV);
	ae::SafeRelease(m_TextureSRV);
	ae::SafeRelease(m_DepthStencilTexture);
	ae::SafeRelease(m_DepthStencilView);
}

// *****************************************************************************************************
// *****************************************************************************************************
C_RenderTarget::C_RenderTarget() :
	m_Texture(0),
	m_TextureRTV(0),
	m_TextureSRV(0),
	m_TextureUAV(0)
{
}

C_RenderTarget::~C_RenderTarget()
{
	Release();
}

void C_RenderTarget::Create(ID3D11Device* device, ae::u32 width, ae::u32 height, DXGI_FORMAT format, bool createUAV)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDesc;
	ae::ZeroStruct(&textureDesc);
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | (createUAV ? D3D11_BIND_UNORDERED_ACCESS : 0);
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDesc, NULL, &m_Texture);
	result = device->CreateRenderTargetView(m_Texture, NULL, &m_TextureRTV);
	result = device->CreateShaderResourceView(m_Texture, NULL, &m_TextureSRV);
	if(createUAV) result = device->CreateUnorderedAccessView(m_Texture, NULL, &m_TextureUAV);
}

void C_RenderTarget::Release()
{
	ae::SafeRelease(m_Texture);
	ae::SafeRelease(m_TextureRTV);
	ae::SafeRelease(m_TextureSRV);
	ae::SafeRelease(m_TextureUAV);
}

// *****************************************************************************************************
// *****************************************************************************************************
C_Sampler::C_Sampler() : m_SamplerState(0) {
}

C_Sampler::~C_Sampler() {
	Release();
}

void C_Sampler::Create(ID3D11Device* device, E_Filter filter, E_AddressMode am)
{
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_TEXTURE_ADDRESS_MODE addressMode(D3D11_TEXTURE_ADDRESS_CLAMP);
	if(am == E_AM_Mirror) addressMode = D3D11_TEXTURE_ADDRESS_MIRROR;
	else if(am == E_AM_Wrap)  addressMode = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.Filter = filter == E_F_LINEAR ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 1;
	samplerDesc.BorderColor[1] = 1;
	samplerDesc.BorderColor[2] = 1;
	samplerDesc.BorderColor[3] = 1;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &m_SamplerState);
}

void C_Sampler::Release() {
	ae::SafeRelease(m_SamplerState);
}

// *****************************************************************************************************
// *****************************************************************************************************

C_RenderDevice::C_RenderDevice(const S_Init& sInit, C_RenderWindow* window) :
	m_pRenderWindow(window),
	m_Factory(0),
	m_Adapter(0),
	m_Output(0),
	m_SwapChain(0),
	m_Device(0),
	m_DeviceContext(0),
	m_MainRenderTargetView(0),
	m_DepthStencilBuffer(0),
	m_DepthStencilState3D(0),
	m_DepthStencilState2D(0),
	m_DepthStencilShadowVolumes(0),
	m_DepthStencilShadows(0),
	m_BlendStateAlphaEnabled(0),
	m_BlendStateStencilShadows(0),
	m_Counter(0),
	m_DepthStencilView(0),
	m_RasterState(0),
	m_StencilShadowsRasterState(0),
	m_ShadowMapRasterState(0)
{
	DBG_OUT << "'DirectX11' render system initializing..." << std::endl;

	HRESULT result;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_Factory);
// *****************************************************************************************************
	PrintAdapterInfo(m_Factory);
// *****************************************************************************************************
	result = m_Factory->EnumAdapters(0, &m_Adapter);
	result = m_Adapter->EnumOutputs(0, &m_Output);
// *****************************************************************************************************
	DXGI_MODE_DESC optimalFSMode = {0};
	DXGI_MODE_DESC checkFSMode = {0};

	checkFSMode.Width = ::GetSystemMetrics(SM_CXFULLSCREEN);
	checkFSMode.Height = ::GetSystemMetrics(SM_CYFULLSCREEN);
	checkFSMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	checkFSMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	checkFSMode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	result = m_Output->FindClosestMatchingMode(&checkFSMode, &optimalFSMode, 0);
// *****************************************************************************************************
	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
// *****************************************************************************************************
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = sInit.windowWidth;
	swapChainDesc.BufferDesc.Height = sInit.windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = optimalFSMode.RefreshRate.Numerator;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = optimalFSMode.RefreshRate.Denominator;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->GetHWnd();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;
	swapChainDesc.BufferDesc.ScanlineOrdering = optimalFSMode.ScanlineOrdering;
	swapChainDesc.BufferDesc.Scaling = optimalFSMode.Scaling;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
					       D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);
// *****************************************************************************************************
// *****************************************************************************************************
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = false;
	result = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState3D);
// *****************************************************************************************************
	depthStencilDesc.DepthEnable = false;
	result = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState2D);
// *****************************************************************************************************
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	result = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilShadowVolumes);
// *****************************************************************************************************
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	result = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilShadows);
// *****************************************************************************************************
// *****************************************************************************************************
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	result = m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState);
// *****************************************************************************************************
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	result = m_Device->CreateRasterizerState(&rasterDesc, &m_StencilShadowsRasterState);

	rasterDesc.DepthClipEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	result = m_Device->CreateRasterizerState(&rasterDesc, &m_ShadowMapRasterState);
// *****************************************************************************************************
// *****************************************************************************************************
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	result = m_Device->CreateBlendState(&blendDesc, &m_BlendStateAlphaEnabled);
	// *****************************************************************************************************
	blendDesc.RenderTarget[0].BlendEnable = false;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
	result = m_Device->CreateBlendState(&blendDesc, &m_BlendStateStencilShadows);
// *****************************************************************************************************
// *****************************************************************************************************
	D3D11_COUNTER_DESC counterDesc;
	ae::ZeroStruct(&counterDesc);
	counterDesc.Counter = D3D11_COUNTER_DEVICE_DEPENDENT_0;
	result = m_Device->CreateCounter(&counterDesc, &m_Counter);
// *****************************************************************************************************
	SetClearColor(164, 209, 255, 255);
// *****************************************************************************************************
//	Creating and activating default texture samplers
	m_SamplerPointMirror.Create(m_Device, C_Sampler::E_F_POINT, C_Sampler::E_AM_Mirror);
	m_SamplerPointWrap.Create(m_Device, C_Sampler::E_F_POINT, C_Sampler::E_AM_Wrap);
	m_SamplerPointClamp.Create(m_Device, C_Sampler::E_F_POINT, C_Sampler::E_AM_Clamp);
	m_SamplerLinearMirror.Create(m_Device, C_Sampler::E_F_LINEAR, C_Sampler::E_AM_Mirror);
	m_SamplerLinearWrap.Create(m_Device, C_Sampler::E_F_LINEAR, C_Sampler::E_AM_Wrap);
	m_SamplerLinearClamp.Create(m_Device, C_Sampler::E_F_LINEAR, C_Sampler::E_AM_Clamp);

	ID3D11SamplerState* samplers[] = {	m_SamplerPointMirror.GetSampler(), 
										m_SamplerPointWrap.GetSampler(),
										m_SamplerPointClamp.GetSampler(),
										m_SamplerLinearMirror.GetSampler(),
										m_SamplerLinearWrap.GetSampler(),
										m_SamplerLinearClamp.GetSampler()};
	m_DeviceContext->PSSetSamplers(0, sizeof(samplers)/sizeof(*samplers), samplers);
// *****************************************************************************************************
	DBG_OUT << "'DirectX11' render system initialized." << std::endl;
}

C_RenderDevice::~C_RenderDevice()
{
	DBG_OUT << "'DirectX11' render system releasing..." << std::endl;

	if(m_SwapChain)
		m_SwapChain->SetFullscreenState(false, NULL);

	ae::SafeRelease(m_BlendStateAlphaEnabled);
	ae::SafeRelease(m_BlendStateStencilShadows);
	ae::SafeRelease(m_RasterState);
	ae::SafeRelease(m_StencilShadowsRasterState);
	ae::SafeRelease(m_ShadowMapRasterState);
	ae::SafeRelease(m_DepthStencilView);
	ae::SafeRelease(m_DepthStencilState3D);
	ae::SafeRelease(m_DepthStencilState2D);
	ae::SafeRelease(m_DepthStencilShadowVolumes);
	ae::SafeRelease(m_DepthStencilShadows);

	ae::SafeRelease(m_DepthStencilBuffer);
	ae::SafeRelease(m_MainRenderTargetView);
	ae::SafeRelease(m_Counter);
	ae::SafeRelease(m_DeviceContext);
	ae::SafeRelease(m_Device);
	ae::SafeRelease(m_SwapChain);
	ae::SafeRelease(m_Output);
	ae::SafeRelease(m_Adapter);
	ae::SafeRelease(m_Factory);

	DBG_OUT << "'DirectX11' render system released." << std::endl;
}

void C_RenderDevice::PrintAdapterInfo(IDXGIFactory* factory)
{
	DBG_OUT << "  Adapters :" << std::endl;
	IDXGIAdapter* pAdapter;
	IDXGIOutput* pOutput;

	DXGI_ADAPTER_DESC adapterDesc;
	DXGI_OUTPUT_DESC outputDesc;
	UINT iAdapter = 0;
// *****************************************************************************************************
	while(factory->EnumAdapters(iAdapter++, &pAdapter) == S_OK)
	{
		pAdapter->GetDesc(&adapterDesc);
// *****************************************************************************************************
		DBG_OUT << "  Adapter name '";
		const ae::u32 descriptionSize(sizeof(adapterDesc.Description) / sizeof *adapterDesc.Description);
		for(ae::u32 c = 0; c < descriptionSize && adapterDesc.Description[c]; c++)
			DBG_OUT << (char)adapterDesc.Description[c];
		DBG_OUT << "'";
		DBG_OUT << " VendorId : " << adapterDesc.VendorId;
		DBG_OUT << " DeviceId : " << adapterDesc.DeviceId;
		DBG_OUT << " SubSysId : " << adapterDesc.SubSysId;
		DBG_OUT << " Revision : " << adapterDesc.Revision;
		DBG_OUT << std::endl;

		DBG_OUT << "  Memory";
		DBG_OUT << " Video : " << (adapterDesc.DedicatedVideoMemory >> 20) << "MB";
		DBG_OUT << " System : " << (adapterDesc.DedicatedSystemMemory >> 20) << "MB";
		DBG_OUT << " Shared : " << (adapterDesc.SharedSystemMemory >> 20) << "MB";
		DBG_OUT << std::endl;
// *****************************************************************************************************
		UINT iOutput = 0;
		while(pAdapter->EnumOutputs(iOutput++, &pOutput) == S_OK)
		{
			pOutput->GetDesc(&outputDesc);
// *****************************************************************************************************
			DBG_OUT << "    Output name '";
			const ae::u32 descriptionSize(sizeof(outputDesc.DeviceName) / sizeof *outputDesc.DeviceName);
			for(ae::u32 c = 0; c < descriptionSize && outputDesc.DeviceName[c]; c++)
				DBG_OUT << (char)outputDesc.DeviceName[c];
			DBG_OUT << "' " << std::endl;
// *****************************************************************************************************
			pOutput->Release();
		}
// *****************************************************************************************************
		pAdapter->Release();
	}
}

void C_RenderDevice::ResizeBuffers(ae::u32 viewWidth, ae::u32 viewHeight)
{
	HRESULT result;
	m_DeviceContext->OMSetRenderTargets(0, 0, 0);
	// Clearing resource from stages since they hold referencies !!!
//	m_DeviceContext->PSSetShaderResources(
// *****************************************************************************************************
	ae::SafeRelease(m_DepthStencilBuffer);
	ae::SafeRelease(m_DepthStencilView);
	ae::SafeRelease(m_MainRenderTargetView);
	m_DiffuseRT.Release();
	m_NormalRT.Release();
	m_LightRT.Release();
	m_DepthRT.Release();
	m_ShadowMap.Release();
// *****************************************************************************************************
	m_SwapChain->ResizeBuffers(2, viewWidth, viewHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
// *****************************************************************************************************
	ID3D11Texture2D* backBufferPtr;
	result = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	result = m_Device->CreateRenderTargetView(backBufferPtr, NULL, &m_MainRenderTargetView);
	backBufferPtr->Release();
	backBufferPtr = 0;
// *****************************************************************************************************
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = viewWidth;
	depthBufferDesc.Height = viewHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	result = m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);
// *****************************************************************************************************
	m_DiffuseRT.Create(m_Device, viewWidth, viewHeight, DXGI_FORMAT_R8G8B8A8_UNORM, false); // Diffuse texture
	m_NormalRT.Create(m_Device, viewWidth, viewHeight, DXGI_FORMAT_R8G8B8A8_SNORM, false); // Normal texture
	m_LightRT.Create(m_Device, viewWidth, viewHeight, DXGI_FORMAT_R8G8B8A8_UNORM, false); // Light texture
	m_DepthRT.Create(m_Device, viewWidth, viewHeight, DXGI_FORMAT_R32_FLOAT, false); // 32 bit precise depth buffer

	m_ShadowMap.Create(m_Device, 4096, 4096);
// *****************************************************************************************************
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
// *****************************************************************************************************
	m_ViewPort.Width = (float)viewWidth;
	m_ViewPort.Height = (float)viewHeight;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
// *****************************************************************************************************
	CheckBuffers();
}

void C_RenderDevice::CheckBuffers()
{
	if(IsFullScreenMode())
	{
		HRESULT result;
		DXGI_SWAP_CHAIN_DESC currentSwapChainDesc;
		result = m_SwapChain->GetDesc(&currentSwapChainDesc);
		IDXGIOutput* pOutput;
		result = m_SwapChain->GetContainingOutput(&pOutput);
		DXGI_MODE_DESC optimalMode;
		result = pOutput->FindClosestMatchingMode(&currentSwapChainDesc.BufferDesc, &optimalMode, m_Device);
		ae::SafeRelease(pOutput);

		if(	currentSwapChainDesc.BufferDesc.Width != optimalMode.Width ||
			currentSwapChainDesc.BufferDesc.Height != optimalMode.Height ||
			currentSwapChainDesc.BufferDesc.RefreshRate.Numerator != optimalMode.RefreshRate.Numerator ||
			currentSwapChainDesc.BufferDesc.RefreshRate.Denominator != optimalMode.RefreshRate.Denominator ||
			currentSwapChainDesc.BufferDesc.Format != optimalMode.Format ||
			currentSwapChainDesc.BufferDesc.ScanlineOrdering != optimalMode.ScanlineOrdering ||
			currentSwapChainDesc.BufferDesc.Scaling != optimalMode.Scaling)
		{
			DBG_WRN << "Fullscreen swap chain buffer is not optimal !" << std::endl;

			DBG_WRN << "  Current settings : ";
			DBG_WRN << currentSwapChainDesc.BufferDesc.Width << "x" << currentSwapChainDesc.BufferDesc.Height;
			DBG_WRN << " Frq. N " << currentSwapChainDesc.BufferDesc.RefreshRate.Numerator;
			DBG_WRN << " D " << currentSwapChainDesc.BufferDesc.RefreshRate.Denominator;
			DBG_WRN << " Fmt. " << FormatToString(currentSwapChainDesc.BufferDesc.Format);
			DBG_WRN << " SLO. " << ScanLineModeToString(currentSwapChainDesc.BufferDesc.ScanlineOrdering);
			DBG_WRN << " Scl. " << ScalingModeToString(currentSwapChainDesc.BufferDesc.Scaling);
			DBG_WRN << std::endl;

			DBG_WRN << "  Optimal settings : ";
			DBG_WRN << optimalMode.Width << "x" << optimalMode.Height;
			DBG_WRN << " Frq. N " << optimalMode.RefreshRate.Numerator;
			DBG_WRN << " D " << optimalMode.RefreshRate.Denominator;
			DBG_WRN << " Fmt. " << FormatToString(optimalMode.Format);
			DBG_WRN << " SLO. " << ScanLineModeToString(optimalMode.ScanlineOrdering);
			DBG_WRN << " Scl. " << ScalingModeToString(optimalMode.Scaling);
			DBG_WRN << std::endl;
		}
	}
}

// *****************************************************************************************************
bool C_RenderDevice::IsFullScreenMode() const
{
	BOOL bIsFullScreen;
	m_SwapChain->GetFullscreenState(&bIsFullScreen,0);
	return bIsFullScreen ? true : false;
}

void C_RenderDevice::ChangeFullScreenMode(bool fullScreen)
{
	m_SwapChain->SetFullscreenState(fullScreen,0);
}

void C_RenderDevice::ChangeResolution(ae::u32 screenWidth, ae::u32 screenHeight, bool fullScreen)
{
	if(fullScreen)
	{
		if(!screenWidth) screenWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
		if(!screenHeight) screenHeight = ::GetSystemMetrics(SM_CYFULLSCREEN);

		m_pRenderWindow->SetWindowDimensions(screenWidth, screenHeight);
		m_SwapChain->SetFullscreenState(fullScreen,0);
	}
	else
	{
		m_SwapChain->SetFullscreenState(fullScreen,0);
		m_pRenderWindow->SetWindowDimensions(screenWidth, screenHeight);
	}
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_RenderDevice::BeginRender()
{
	m_DeviceContext->Begin(m_Counter);
	m_DeviceContext->RSSetState(m_RasterState);
}

void C_RenderDevice::BeginSceneRender()
{
	ID3D11RenderTargetView* RTVs[] = { m_DiffuseRT.m_TextureRTV, m_NormalRT.m_TextureRTV, m_DepthRT.m_TextureRTV };
	// OMSetRenderTargets must be called first !!!
	m_DeviceContext->OMSetRenderTargets((UINT)ae::ArraySize(RTVs), RTVs, m_DepthStencilView);

	m_DeviceContext->OMSetBlendState(NULL, 0, 0xffffffff);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState3D, 1);

	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	m_DeviceContext->ClearRenderTargetView(m_DiffuseRT.m_TextureRTV, m_ClearColor);
	m_DeviceContext->ClearRenderTargetView(m_NormalRT.m_TextureRTV, ae::math::float4(0,0,0,0));
	m_DeviceContext->ClearRenderTargetView(m_LightRT.m_TextureRTV, ae::math::float4(0,0,0,0));
	m_DeviceContext->ClearRenderTargetView(m_DepthRT.m_TextureRTV, ae::math::float4(1,1,1,1));
}

void C_RenderDevice::BeginStencilShadowsRender()
{
	m_DeviceContext->RSSetState(m_StencilShadowsRasterState);
	m_DeviceContext->OMSetBlendState(m_BlendStateStencilShadows, 0, 0xffffffff);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilShadowVolumes, 1);
}

void C_RenderDevice::BeginShadowMapRender()
{
	m_DeviceContext->OMSetRenderTargets(1, &m_ShadowMap.m_TextureRTV, m_ShadowMap.m_DepthStencilView);

	m_DeviceContext->ClearDepthStencilView(m_ShadowMap.m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_DeviceContext->ClearRenderTargetView(m_ShadowMap.m_TextureRTV, ae::math::float4(1,1,1,1));

	m_DeviceContext->OMSetBlendState(NULL, 0, 0xffffffff);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState3D, 1);
	m_DeviceContext->RSSetState(m_ShadowMapRasterState);
	m_DeviceContext->RSSetViewports(1, &m_ShadowMap.m_ViewPort);
}

void C_RenderDevice::BeginLightRender()
{
	m_DeviceContext->RSSetState(m_RasterState);
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
	// OMSetRenderTargets must be called first !!!
	m_DeviceContext->OMSetRenderTargets(1, &m_LightRT.m_TextureRTV, m_DepthStencilView);

	ID3D11ShaderResourceView* SRVs[] = { m_NormalRT.m_TextureSRV, m_DepthRT.m_TextureSRV, m_ShadowMap.m_TextureSRV };
	m_DeviceContext->PSSetShaderResources(0, (UINT)ae::ArraySize(SRVs), SRVs);

	m_DeviceContext->OMSetBlendState(NULL, 0, 0xffffffff);

	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilShadows, 0);
}

void C_RenderDevice::BeginDfrComposeRender()
{
	ID3D11ShaderResourceView* SRVs[] = { m_DiffuseRT.m_TextureSRV, m_LightRT.m_TextureSRV};

	// OMSetRenderTargets must be called first !!!
	m_DeviceContext->OMSetRenderTargets(1, &m_MainRenderTargetView, m_DepthStencilView);
	
	m_DeviceContext->PSSetShaderResources(0, (UINT)ae::ArraySize(SRVs), SRVs);

	m_DeviceContext->OMSetBlendState(NULL, 0, 0xffffffff);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState2D, 1);

	m_DeviceContext->ClearRenderTargetView(m_MainRenderTargetView, m_ClearColor);
}

void C_RenderDevice::Begin2DRender()
{
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState2D, 1);
	m_DeviceContext->OMSetBlendState(m_BlendStateAlphaEnabled, 0, 0xffffffff);
}
// *****************************************************************************************************

void C_RenderDevice::EndRender()
{
	m_DeviceContext->End(m_Counter);
	PROFILE_BLOCK_BEGIN(Present);
	m_SwapChain->Present(0, 0);
	PROFILE_BLOCK_END(Present);
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_RenderDevice::SetClearColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	const float recp(1.0f / 255.0f);
	m_ClearColor = ae::math::float4(r*recp, g*recp, b*recp, a*recp );
}

} } } // namespace ae { namespace render { namespace dx11 {

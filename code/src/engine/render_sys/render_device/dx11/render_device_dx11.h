#pragma once

#ifndef AE_RENDER_SYS_RENDER_DEVICE_DX11_H
#define AE_RENDER_SYS_RENDER_DEVICE_DX11_H

#include "../../../math_sys/types.h"

namespace ae { namespace render {
	class C_RenderDevice;
	struct S_Init;
} } // namespace ae { namespace render {

namespace ae { namespace render { namespace dx11 {

	class C_RenderWindow;

// *****************************************************************************************************
// *****************************************************************************************************
	// @TODO move to resources
	class C_RenderTarget
	{
	public:
		ID3D11Texture2D* m_Texture;
		ID3D11RenderTargetView* m_TextureRTV;
		ID3D11ShaderResourceView* m_TextureSRV;
		ID3D11UnorderedAccessView* m_TextureUAV;
	public:
		C_RenderTarget();
		~C_RenderTarget();
		void Create(ID3D11Device*, ae::u32 width, ae::u32 height, DXGI_FORMAT, bool createUAV);
		void Release();
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_Sampler
	{
		ID3D11SamplerState* m_SamplerState;
	public:
		enum E_AddressMode {
			E_AM_Wrap,
			E_AM_Clamp,
			E_AM_Mirror
		};
		enum E_Filter {
			E_F_POINT,
			E_F_LINEAR,
		};
		C_Sampler();
		~C_Sampler();

		ID3D11SamplerState* GetSampler() const { return m_SamplerState; }

		void Create(ID3D11Device*, E_Filter, E_AddressMode);
		void Release();
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_ShadowMap
	{
	public:
		D3D11_VIEWPORT m_ViewPort;
		ID3D11Texture2D* m_DepthTexture;
		ID3D11RenderTargetView* m_TextureRTV;
		ID3D11ShaderResourceView* m_TextureSRV;
		ID3D11Texture2D* m_DepthStencilTexture;
		ID3D11DepthStencilView* m_DepthStencilView;
	public:
		C_ShadowMap();
		~C_ShadowMap();
		void Create(ID3D11Device*, ae::u32 width, ae::u32 height);
		void Release();
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_RenderDevice
	{
		C_RenderDevice();
		NOCOPY_CLASS(C_RenderDevice);

		C_RenderDevice(const S_Init&, C_RenderWindow*);
		~C_RenderDevice();

		friend class ae::render::C_RenderDevice;
// *****************************************************************************************************
		C_RenderWindow* m_pRenderWindow;
		IDXGIFactory* m_Factory;
		IDXGIAdapter* m_Adapter;
		IDXGIOutput* m_Output;
// *****************************************************************************************************
		IDXGISwapChain* m_SwapChain;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
// *****************************************************************************************************
		ID3D11DepthStencilState* m_DepthStencilState3D,*m_DepthStencilState2D,*m_DepthStencilShadowVolumes,*m_DepthStencilShadows;

		ID3D11BlendState *m_BlendStateAlphaEnabled, *m_BlendStateStencilShadows;
// *****************************************************************************************************
		ID3D11Counter* m_Counter;
// *****************************************************************************************************
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11RasterizerState *m_RasterState,*m_StencilShadowsRasterState,*m_ShadowMapRasterState;
// *****************************************************************************************************
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11RenderTargetView* m_MainRenderTargetView;
		D3D11_VIEWPORT m_ViewPort;
// *****************************************************************************************************
		C_RenderTarget m_DiffuseRT;
		C_RenderTarget m_NormalRT;
		C_RenderTarget m_LightRT;
		C_RenderTarget m_DepthRT;
		C_ShadowMap m_ShadowMap;
// *****************************************************************************************************
		C_Sampler m_SamplerPointMirror, m_SamplerPointWrap, m_SamplerPointClamp;
		C_Sampler m_SamplerLinearMirror, m_SamplerLinearWrap, m_SamplerLinearClamp;
// *****************************************************************************************************
		void BeginRender();
		void BeginSceneRender();
		void BeginStencilShadowsRender();
		void BeginShadowMapRender();
		void BeginLightRender();
		void BeginDfrComposeRender();
		void Begin2DRender();
		void EndRender();
// *****************************************************************************************************
		void ResizeBuffers(ae::u32 viewWidth, ae::u32 viewHeight);
		void CheckBuffers(); // check if fullscreen has optimal buffer settings
// *****************************************************************************************************
		void PrintAdapterInfo(IDXGIFactory*);
// *****************************************************************************************************
		bool IsFullScreenMode() const;
		void ChangeFullScreenMode(bool fullScreen);
		void ChangeResolution(ae::u32 screenWidth, ae::u32 screenHeight, bool fullScreen);
// *****************************************************************************************************
		void SetClearColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
		ae::math::float4 m_ClearColor;
	public:
		ID3D11Device* GetDevice() { return m_Device; }
		ID3D11DeviceContext* GetContext() { return m_DeviceContext; }
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RENDER_DEVICE_DX11_H

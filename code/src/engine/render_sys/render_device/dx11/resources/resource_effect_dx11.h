#pragma once

#ifndef AE_RENDER_SYS_RESOURCE_EFFECT_DX11_H
#define AE_RENDER_SYS_RESOURCE_EFFECT_DX11_H

namespace ae { namespace render { namespace dx11 {

	class C_RenderResources;

	class C_EffectResource
	{
	public:
		C_EffectResource();
		void Activate(ID3D11DeviceContext*);
	private:
		friend class C_RenderResources;
		NOCOPY_CLASS(C_EffectResource);

		ID3D11VertexShader*		m_pVertexShader;
		ID3D11PixelShader*		m_pPixelShader;
		ID3D11GeometryShader*	m_pGeometryShader;
		ID3D11ComputeShader*	m_pComputeShader;
		ID3D11HullShader*		m_pHullShader;
		ID3D11DomainShader*		m_pDomainShader;
	};

} } } // namespace ae { namespace render { namespace dx11 {

#endif // #ifdef AE_RENDER_SYS_RESOURCE_EFFECT_DX11_H

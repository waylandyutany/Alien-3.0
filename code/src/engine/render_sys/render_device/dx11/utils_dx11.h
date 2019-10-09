#pragma once

#ifndef AE_RENDER_SYS_DX11_UTILS_H
#define AE_RENDER_SYS_DX11_UTILS_H

namespace ae { namespace render { namespace dx11 {

	const char* ScanLineModeToString(DXGI_MODE_SCANLINE_ORDER);
	const char* ScalingModeToString(DXGI_MODE_SCALING);
	const char* FormatToString(DXGI_FORMAT);
	const char* ErrorToString(HRESULT);

} } } // namespace ae { namespace render { namespace dx11 {

#endif // AE_RENDER_SYS_DX11_UTILS_H

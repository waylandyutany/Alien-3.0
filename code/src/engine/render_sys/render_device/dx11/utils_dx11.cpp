#include "StdAfx.h"

#include "utils_dx11.h"

namespace ae { namespace render { namespace dx11 {

	const char* ScanLineModeToString(DXGI_MODE_SCANLINE_ORDER scanLineOrderMode)
	{
		static const char* modeStrings [] = {	"DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED",
												"DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE",
												"DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST",
												"DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST" };
		static const ae::u32 numModes = sizeof(modeStrings) / sizeof *modeStrings;
		const ae::u32 mode(static_cast<ae::u32>(scanLineOrderMode));
		AE_ASSERT(mode < numModes && "Invalid scan line mode !");
		return modeStrings[mode];
	}


	const char* ScalingModeToString(DXGI_MODE_SCALING scalingMode)
	{
		static const char* modeStrings [] = {	"DXGI_MODE_SCALING_UNSPECIFIED",
												"DXGI_MODE_SCALING_CENTERED",
												"DXGI_MODE_SCALING_STRETCHED" };
		static const ae::u32 numModes = sizeof(modeStrings) / sizeof *modeStrings;
		const ae::u32 mode(static_cast<ae::u32>(scalingMode));
		AE_ASSERT(mode < numModes && "Invalid scaling mode !");
		return modeStrings[mode];
	}

	const char* FormatToString(DXGI_FORMAT format)
	{
		static const char* formatStrings [] = {	"DXGI_FORMAT_UNKNOWN",
												"DXGI_FORMAT_R32G32B32A32_TYPELESS",
												"DXGI_FORMAT_R32G32B32A32_FLOAT",
												"DXGI_FORMAT_R32G32B32A32_UINT",
												"DXGI_FORMAT_R32G32B32A32_SINT",
												"DXGI_FORMAT_R32G32B32_TYPELESS",
												"DXGI_FORMAT_R32G32B32_FLOAT",
												"DXGI_FORMAT_R32G32B32_UINT",
												"DXGI_FORMAT_R32G32B32_SINT",
												"DXGI_FORMAT_R16G16B16A16_TYPELESS",
												"DXGI_FORMAT_R16G16B16A16_FLOAT",
												"DXGI_FORMAT_R16G16B16A16_UNORM",
												"DXGI_FORMAT_R16G16B16A16_UINT",
												"DXGI_FORMAT_R16G16B16A16_SNORM",
												"DXGI_FORMAT_R16G16B16A16_SINT",
												"DXGI_FORMAT_R32G32_TYPELESS",
												"DXGI_FORMAT_R32G32_FLOAT",
												"DXGI_FORMAT_R32G32_UINT",
												"DXGI_FORMAT_R32G32_SINT",
												"DXGI_FORMAT_R32G8X24_TYPELESS",
												"DXGI_FORMAT_D32_FLOAT_S8X24_UINT",
												"DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS",
												"DXGI_FORMAT_X32_TYPELESS_G8X24_UINT",
												"DXGI_FORMAT_R10G10B10A2_TYPELESS",
												"DXGI_FORMAT_R10G10B10A2_UNORM",
												"DXGI_FORMAT_R10G10B10A2_UINT",
												"DXGI_FORMAT_R11G11B10_FLOAT",
												"DXGI_FORMAT_R8G8B8A8_TYPELESS",
												"DXGI_FORMAT_R8G8B8A8_UNORM",
												"DXGI_FORMAT_R8G8B8A8_UNORM_SRGB",
												"DXGI_FORMAT_R8G8B8A8_UINT",
												"DXGI_FORMAT_R8G8B8A8_SNORM",
												"DXGI_FORMAT_R8G8B8A8_SINT",
												"DXGI_FORMAT_R16G16_TYPELESS",
												"DXGI_FORMAT_R16G16_FLOAT",
												"DXGI_FORMAT_R16G16_UNORM",
												"DXGI_FORMAT_R16G16_UINT",
												"DXGI_FORMAT_R16G16_SNORM",
												"DXGI_FORMAT_R16G16_SINT",
												"DXGI_FORMAT_R32_TYPELESS",
												"DXGI_FORMAT_D32_FLOAT",
												"DXGI_FORMAT_R32_FLOAT",
												"DXGI_FORMAT_R32_UINT",
												"DXGI_FORMAT_R32_SINT",
												"DXGI_FORMAT_R24G8_TYPELESS",
												"DXGI_FORMAT_D24_UNORM_S8_UINT",
												"DXGI_FORMAT_R24_UNORM_X8_TYPELESS",
												"DXGI_FORMAT_X24_TYPELESS_G8_UINT",
												"DXGI_FORMAT_R8G8_TYPELESS",
												"DXGI_FORMAT_R8G8_UNORM",
												"DXGI_FORMAT_R8G8_UINT",
												"DXGI_FORMAT_R8G8_SNORM",
												"DXGI_FORMAT_R8G8_SINT",
												"DXGI_FORMAT_R16_TYPELESS",
												"DXGI_FORMAT_R16_FLOAT",
												"DXGI_FORMAT_D16_UNORM",
												"DXGI_FORMAT_R16_UNORM",
												"DXGI_FORMAT_R16_UINT",
												"DXGI_FORMAT_R16_SNORM",
												"DXGI_FORMAT_R16_SINT",
												"DXGI_FORMAT_R8_TYPELESS",
												"DXGI_FORMAT_R8_UNORM",
												"DXGI_FORMAT_R8_UINT",
												"DXGI_FORMAT_R8_SNORM",
												"DXGI_FORMAT_R8_SINT",
												"DXGI_FORMAT_A8_UNORM",
												"DXGI_FORMAT_R1_UNORM",
												"DXGI_FORMAT_R9G9B9E5_SHAREDEXP",
												"DXGI_FORMAT_R8G8_B8G8_UNORM",
												"DXGI_FORMAT_G8R8_G8B8_UNORM",
												"DXGI_FORMAT_BC1_TYPELESS",
												"DXGI_FORMAT_BC1_UNORM",
												"DXGI_FORMAT_BC1_UNORM_SRGB",
												"DXGI_FORMAT_BC2_TYPELESS",
												"DXGI_FORMAT_BC2_UNORM",
												"DXGI_FORMAT_BC2_UNORM_SRGB",
												"DXGI_FORMAT_BC3_TYPELESS",
												"DXGI_FORMAT_BC3_UNORM",
												"DXGI_FORMAT_BC3_UNORM_SRGB",
												"DXGI_FORMAT_BC4_TYPELESS",
												"DXGI_FORMAT_BC4_UNORM",
												"DXGI_FORMAT_BC4_SNORM",
												"DXGI_FORMAT_BC5_SNORM",
												"DXGI_FORMAT_B5G6R5_UNORM",
												"DXGI_FORMAT_B5G5R5A1_UNORM",
												"DXGI_FORMAT_B8G8R8A8_UNORM",
												"DXGI_FORMAT_B8G8R8X8_UNORM",
												"DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM",
												"DXGI_FORMAT_B8G8R8A8_TYPELESS",
												"DXGI_FORMAT_B8G8R8A8_UNORM_SRGB",
												"DXGI_FORMAT_B8G8R8X8_TYPELESS",
												"DXGI_FORMAT_B8G8R8X8_UNORM_SRGB",
												"DXGI_FORMAT_BC6H_TYPELESS",
												"DXGI_FORMAT_BC6H_UF16",
												"DXGI_FORMAT_BC6H_SF16",
												"DXGI_FORMAT_BC7_TYPELESS",
												"DXGI_FORMAT_BC7_UNORM",
												"DXGI_FORMAT_BC7_UNORM_SRGB"};
		static const ae::u32 numFormats = sizeof(formatStrings) / sizeof *formatStrings;
		const ae::u32 u32Format(static_cast<ae::u32>(format));
		AE_ASSERT(u32Format < numFormats && "Invalid format !");
		return formatStrings[u32Format];
	}

	const char* ErrorToString(HRESULT errorCode)
	{
		return DXGetErrorStringA(errorCode);
	}


} } } // namespace ae { namespace render { namespace dx11 {

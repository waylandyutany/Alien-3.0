#pragma once

#ifndef AE_RENDER_SYS_DX11_CONSTANTS_H
#define AE_RENDER_SYS_DX11_CONSTANTS_H

namespace ae { namespace render { namespace dx11 {

typedef ae::u32 T_BufferFlags;
static const T_BufferFlags E_BF_NONE= 0;
static const T_BufferFlags E_BF_VS	= 1;
static const T_BufferFlags E_BF_PS	= 2;
static const T_BufferFlags E_BF_GS	= 4;
static const T_BufferFlags E_BF_CS	= 8;
static const T_BufferFlags E_BF_HS	= 16;
static const T_BufferFlags E_BF_DS	= 32;

typedef ae::u32 T_BufferSlotIndex;
static const T_BufferSlotIndex E_BSI_None		= ae::maxu32;
static const T_BufferSlotIndex E_BSI_PerFrame	= 0;
static const T_BufferSlotIndex E_BSI_PerObject	= 1;
static const T_BufferSlotIndex E_BSI_PerMaterial= 2;

} } } // namespace ae { namespace render { namespace dx11 {

#endif // AE_RENDER_SYS_DX11_CONSTANTS_H

#pragma once
#ifndef AE_TYPES_H
#define AE_TYPES_H

namespace ae {

	typedef unsigned __int64	u64;
	typedef unsigned __int32	u32;
	typedef unsigned __int16	u16;
	typedef unsigned __int8		u8;

	typedef __int64				s64;
	typedef __int32				s32;
	typedef __int16				s16;
	typedef __int8				s8;

	static const u64 maxu64 = 0xffffffffffffffff;
	static const u64 minu64 = 0;
	static const u32 maxu32 = 0xffffffff;
	static const u32 minu32 = 0;
	static const u16 maxu16 = 0xffff;
	static const u16 minu16 = 0;
	static const u8  maxu8	= 0xff;
	static const u8  minu8	= 0;

} // namespace ae {

#endif // #ifdef AE_TYPES_H

#pragma once

#ifndef AE_DEBUG_SYS_PROFILER_H
#define AE_DEBUG_SYS_PROFILER_H

#include "../../base_sys/hash.h"
#include "../../base_sys/time/time.h"
#include "../../base_sys/memory/allocators/incremental_allocator.h"

namespace ae { namespace dbg {

// *****************************************************************************************************************
// *****************************************************************************************************************
class C_Profiler
{
	NOCOPY_CLASS(C_Profiler);
// *****************************************************************************************************************
	ae::u64 m_FrameCounter;
	ae::u64 m_FrameTime;
	ae::u64 m_FrameFPSCounter;
	ae::u64 m_FrameFPSCountdown;
	ae::u64 m_FrameFPS;
	float	m_FrameMSTime;
// *****************************************************************************************************************
	struct S_Counter
	{
		union
		{
			size_t value32;
			ae::u64 value64;
		};
		ae::base::S_HashID hid;
		S_Counter* next;
	};

	char* m_pBuffer;
	S_Counter* m_Counters;
	ae::base::C_IncrementalAllocator m_Allocator;
public:
	C_Profiler();
	~C_Profiler();
// *****************************************************************************************************************
	void BeginFrame();
	ae::u32 GetFrameFPS() const;
	float GetFrameMS() const;
	ae::u64 GetFrameCounter() const;
// *****************************************************************************************************************
	typedef ae::u32 T_Counter;
	T_Counter GetCounter(const char* cName);
	void SetCounterValue(T_Counter, size_t value);
	size_t GetCounterValue(T_Counter);
	void SetCounterValueU64(T_Counter, ae::u64 value);
	ae::u64 GetCounterValueU64(T_Counter);
// *****************************************************************************************************************
};

extern C_Profiler profiler;
// *****************************************************************************************************************
// *****************************************************************************************************************
class C_ProfileBlocksTree
{
	friend class C_ProfileBlock;
	friend class C_Profiler;
	void EnterBlock(const char* blockName);
	void LeaveBlock(const char* blockName, ae::u64 blockTime);
	void BeginFrame();
public:
	C_ProfileBlocksTree();
	struct S_BlockInfo	{
		S_BlockInfo(const std::string& bName, ae::u64 bTime) : blockName(bName), blockTime(bTime) {}
		std::string blockName; // Block name, may contain other block names if BEGIN_BLOCK is used nested
		ae::u64 blockTime; // Block time in microseconds
	};
	typedef std::vector<S_BlockInfo> T_BlocksInfo;
	void GetBlocksInfo(T_BlocksInfo& blocksInfo) const;
private:
	T_BlocksInfo m_TmpBlocksInfo;
	T_BlocksInfo m_BlocksInfo;
	typedef std::vector<std::string> T_BlockNames;
	T_BlockNames m_BlockNames;
};

extern C_ProfileBlocksTree profilerBlocksTree;
// *****************************************************************************************************************
// *****************************************************************************************************************
class C_ProfileBlock
{
	ae::u64 m_StartTime;
	ae::u64 m_DeltaTime;

	const char* m_BlockName;
public:
	C_ProfileBlock(const char* blockName);
	~C_ProfileBlock();
	void EndBlock();

	float GetSeconds();
	friend std::ostream& operator<<(std::ostream& os, const C_ProfileBlock& pb);
};

// *****************************************************************************************************************
// *****************************************************************************************************************
#define PROFILE_BLOCK_BEGIN(blockName) ae::dbg::C_ProfileBlock blockName(#blockName)
#define PROFILE_BLOCK_END(blockName) blockName.EndBlock()
// *****************************************************************************************************************
// *****************************************************************************************************************

} } // namespace ae { namespace dbg {

#include "profiler.inl"

#endif // #ifdef AE_DEBUG_SYS_PROFILER_H

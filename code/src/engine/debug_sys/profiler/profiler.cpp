#include "StdAfx.h"

#include "profiler.h"
#include "../output/outputs.h"
#include "../../_sys/templates.h"

namespace ae { namespace dbg {

// *****************************************************************************************************************
// *****************************************************************************************************************
C_Profiler::C_Profiler() :
	m_FrameCounter(0),
	m_FrameTime(0),
	m_FrameFPSCounter(0),
	m_FrameFPSCountdown(1000000),
	m_FrameFPS(0),
	m_FrameMSTime(0.0f),
	m_pBuffer(0),
	m_Counters(0)
{
	const ae::u32 bufferSize(1024 * 256);
	m_pBuffer = (char*)::malloc(bufferSize);
	m_Allocator.Create(m_pBuffer, bufferSize);
	m_Allocator.Reset(true);
}

C_Profiler::~C_Profiler()
{
	::free(m_pBuffer);
}

C_Profiler::T_Counter C_Profiler::GetCounter(const char* cName)
{
	S_Counter* pCounter(ae::FindStructHID<S_Counter>(cName, m_Counters));
	if(pCounter) return reinterpret_cast<T_Counter>(pCounter);
	pCounter = ae::CreateStruct<S_Counter>(m_Counters, m_Allocator);
	pCounter->hid.idHash = ae::base::CalculateHashI32(cName);
	return reinterpret_cast<T_Counter>(pCounter);
}

// *****************************************************************************************************************
// *****************************************************************************************************************

C_Profiler profiler;
C_ProfileBlocksTree profilerBlocksTree;

// *****************************************************************************************************************
// *****************************************************************************************************************
std::ostream& operator<<(std::ostream& os, const C_ProfileBlock& pb)
{
	const ae::u64 deltaTimeMLS = pb.m_DeltaTime / 1000;

	os << "Profile '" << pb.m_BlockName << "' MLS [" << dec_fmt(6) << deltaTimeMLS << "] MCS [" << dec_fmt(6) << pb.m_DeltaTime << "]" << std::endl;

	return os;
}
// *****************************************************************************************************************
// *****************************************************************************************************************
void C_ProfileBlocksTree::EnterBlock(const char* blockName)
{
	REFERENCE(blockName);
//	m_BlockNames.push_back(blockName);
}

void C_ProfileBlocksTree::LeaveBlock(const char* blockName, ae::u64 blockTime)
{
	REFERENCE(blockName);
	REFERENCE(blockTime);
/*
	REFERENCE(blockName);
	std::string blockNames;
	const size_t s(m_BlockNames.size());
	AE_ASSERT(s && "LeaveBlock call without earlier EnterBlock called ?");
	AE_ASSERT(m_BlockNames[s-1] == blockName && "Enter/LeaveBlock invalid call order !");
	for(size_t i(0); i < s; i++)
		blockNames += m_BlockNames[i] + ":";

	m_BlockNames.pop_back();
	m_TmpBlocksInfo.push_back(S_BlockInfo(blockNames, blockTime));
*/
}

} } // namespace ae { namespace dbg {

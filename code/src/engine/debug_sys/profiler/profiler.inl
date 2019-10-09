namespace ae { namespace dbg {

// *****************************************************************************************************************
// *****************************************************************************************************************
inline void C_Profiler::BeginFrame()
{
	// notify profilerBlocksTree then new frame has began
	profilerBlocksTree.BeginFrame();
// *****************************************************************************************************************
	if(!m_FrameTime)
		m_FrameTime = ae::base::time.GetTimeMicroseconds();

	ae::u64 currentFrameTime(ae::base::time.GetTimeMicroseconds());
	ae::u64 deltaFrameTime(currentFrameTime - m_FrameTime);

	if(m_FrameFPSCountdown > deltaFrameTime)
	{
		m_FrameFPSCountdown -= deltaFrameTime;
		m_FrameFPSCounter++;
	}
	else
	{
		m_FrameFPS = m_FrameFPSCounter;
		m_FrameFPSCounter = 0;
		m_FrameFPSCountdown = 1000000;
	}

	m_FrameMSTime = static_cast<float>(deltaFrameTime ? deltaFrameTime / 1000.0f : 0.0f);
	m_FrameCounter++;
	m_FrameTime = currentFrameTime;
}

inline ae::u32 C_Profiler::GetFrameFPS() const
{
	return static_cast<ae::u32>(m_FrameFPS);
}

inline float C_Profiler::GetFrameMS() const
{
	return m_FrameMSTime;
}

inline ae::u64 C_Profiler::GetFrameCounter() const
{
	return m_FrameCounter;
}

inline void C_Profiler::SetCounterValue(T_Counter counter, size_t value)
{
	if(!counter) return;
	S_Counter* pCounter(reinterpret_cast<S_Counter*>(counter));
	pCounter->value32 = value;
}

inline size_t C_Profiler::GetCounterValue(T_Counter counter)
{
	if(!counter) return 0;
	S_Counter* pCounter(reinterpret_cast<S_Counter*>(counter));
	return pCounter->value32;
}

inline void C_Profiler::SetCounterValueU64(T_Counter counter, ae::u64 value)
{
	if(!counter) return;
	S_Counter* pCounter(reinterpret_cast<S_Counter*>(counter));
	pCounter->value64 = value;
}

inline ae::u64 C_Profiler::GetCounterValueU64(T_Counter counter)
{
	if(!counter) return 0;
	S_Counter* pCounter(reinterpret_cast<S_Counter*>(counter));
	return pCounter->value64;
}

inline float C_ProfileBlock::GetSeconds()
{
	return m_DeltaTime / 1000000.0f;
}

// *****************************************************************************************************************
// *****************************************************************************************************************

inline C_ProfileBlocksTree::C_ProfileBlocksTree()
{
	m_BlocksInfo.reserve(128);
	m_TmpBlocksInfo.reserve(128);
	m_BlockNames.reserve(128);
}

inline void C_ProfileBlocksTree::BeginFrame()
{
	m_BlocksInfo.clear();
	m_BlocksInfo = m_TmpBlocksInfo;
	m_TmpBlocksInfo.clear();
	m_BlockNames.clear();
}

inline void C_ProfileBlocksTree::GetBlocksInfo(T_BlocksInfo& blocksInfo) const
{
	blocksInfo = m_BlocksInfo;
}

// *****************************************************************************************************************
// *****************************************************************************************************************

inline C_ProfileBlock::C_ProfileBlock(const char* blockName) :
	m_StartTime(ae::base::time.GetTimeMicroseconds()),
	m_DeltaTime(0),
	m_BlockName(blockName)
{
	profilerBlocksTree.EnterBlock(m_BlockName);
}

inline C_ProfileBlock::~C_ProfileBlock()
{
	EndBlock();
}

inline void C_ProfileBlock::EndBlock()
{
	if(!m_DeltaTime)
	{
		m_DeltaTime = ae::base::time.GetTimeMicroseconds() - m_StartTime;
		profilerBlocksTree.LeaveBlock(m_BlockName, m_DeltaTime);
	}
}

// *****************************************************************************************************************
// *****************************************************************************************************************

} } // namespace ae { namespace dbg {

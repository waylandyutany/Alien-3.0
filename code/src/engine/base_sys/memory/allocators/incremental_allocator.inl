
namespace ae { namespace base {

inline C_IncrementalAllocator::C_IncrementalAllocator() :
	m_pBegin(0),m_pEnd(0),m_pPos(0)
{
}

inline C_IncrementalAllocator::~C_IncrementalAllocator()
{
}

inline void C_IncrementalAllocator::Create(char* buff, size_t size)
{
	AE_ASSERT(buff && size && "Invalid parameters passed to C_IncrementalAllocator::Create !");

	m_pBegin = m_pPos = buff;
	m_pEnd = &m_pBegin[size];
}

inline char* C_IncrementalAllocator::Alloc(size_t size)
{
	char* memBegin = m_pPos;
	char* memEnd = memBegin + size;

	AE_ASSERT(memEnd <= m_pEnd && "C_IncrementalAllocator out of memory !");

	m_pPos = memEnd;
	return memBegin;
}

inline char* C_IncrementalAllocator::Alloc(size_t size, size_t alignment)
{
	char* memBegin = m_pPos;

	size_t alg_mod = (size_t)memBegin % alignment;
	if(alg_mod)
		memBegin = (char*)((((size_t)memBegin / alignment) + 1) * alignment);

	char* memEnd = memBegin + size;

	AE_ASSERT(memEnd <= m_pEnd && "C_IncrementalAllocator out of memory !");

	m_pPos = memEnd;
	return memBegin;
}

inline char* C_IncrementalAllocator::Alloc(size_t size, bool zero)
{
	char* memBegin = Alloc(size);
	if(memBegin && zero) memset(memBegin, 0, size);
	return memBegin;
}

inline char* C_IncrementalAllocator::Alloc(size_t size, size_t alignment, bool zero)
{
	char* memBegin = Alloc(size, alignment);
	if(memBegin && zero) memset(memBegin, 0, size);
	return memBegin;
}

inline void C_IncrementalAllocator::Reset(bool zero)
{
	m_pPos = m_pBegin;

	if(zero)
		memset(m_pBegin, 0 , static_cast<size_t>(m_pEnd - m_pBegin));
}

inline void C_IncrementalAllocator::ResetToPointer(char* p, bool zero)
{
	m_pPos = p;
	if(zero)
		memset(p, 0 , static_cast<size_t>(m_pEnd - p));
}

inline size_t C_IncrementalAllocator::GetUsage() const
{
	return static_cast<ae::u32>(m_pPos - m_pBegin);
}

inline size_t C_IncrementalAllocator::GetRelativeUsage(void* p) const
{
	char* pchr = static_cast<char*>(p);
	if(m_pBegin <= p && p <= m_pPos)
		return static_cast<ae::u32>(m_pPos - pchr);
	return 0;
}

inline char* C_IncrementalAllocator::GetTopPointer() const
{
	return static_cast<char*>(m_pPos);
}

inline char* C_IncrementalAllocator::GetBottomPointer() const
{
	return static_cast<char*>(m_pBegin);
}

// ************************************************************************************
// ************************************************************************************

} } // namespace ae { namespace base {

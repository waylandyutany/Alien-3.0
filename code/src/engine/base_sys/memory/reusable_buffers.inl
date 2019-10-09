
namespace ae { namespace base {

inline C_ReusableBuffers::C_ReusableBuffers(ae::u32 bufferSize, ae::u32 maxBuffers) : 
	m_BufferSize(bufferSize),
	m_MaxBuffers(maxBuffers)
{
}

inline C_ReusableBuffers::~C_ReusableBuffers()
{
	if(m_MaxBuffers)
	{
		AE_ASSERT(m_MaxBuffers == m_UnacquiredBuffers.size() && "Unqauired buffers detected !");
	}
}

inline void C_ReusableBuffers::AddBuffer(void* pBuffer, ae::u32 bufferSize)
{
	if(bufferSize != m_BufferSize) {
		AE_ASSERT(bufferSize == m_BufferSize && "Invalid buffer size !");
	}
	m_UnacquiredBuffers.push_back(pBuffer);
}

inline void* C_ReusableBuffers::AcquireBuffer(ae::u32 bufferSize)
{
	if(bufferSize != m_BufferSize) {
		AE_ASSERT(bufferSize == m_BufferSize && "Invalid buffer size to acquire !");
	}

	if(m_UnacquiredBuffers.size())
	{
		void* pRet(m_UnacquiredBuffers.back());
		m_UnacquiredBuffers.pop_back();
		return pRet;
	}
	return 0;
}

inline void C_ReusableBuffers::UnacquireBuffer(void* pBuffer)
{
	m_UnacquiredBuffers.push_back(pBuffer);
}

} } // namespace ae { namespace base {

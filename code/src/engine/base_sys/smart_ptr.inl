namespace ae { namespace base {

// *****************************************************************************************************
template <typename T>
inline C_SmartPtr<T>::C_SmartPtr( const C_SmartPtr<T>& src )
{
	m_Ptr = src.m_Ptr;
	AddRef();
}
// *****************************************************************************************************
template <typename T>
inline T* C_SmartPtr<T>::operator ->() const
{
	AE_ASSERT(m_Ptr != 0);
	return m_Ptr;
}
// *****************************************************************************************************
template <typename T>
inline C_SmartPtr<T>& C_SmartPtr<T>::operator = ( const C_SmartPtr<T>& src )
{
	Release();

	m_Ptr = src.m_Ptr;
	AddRef();

	return *this;
}
// *****************************************************************************************************
template <typename T>
template <typename U>
inline C_SmartPtr<T>& C_SmartPtr<T>::operator = (const C_SmartPtr<U>& src)
{
	Release();

	m_Ptr = src.get();
	AddRef();

	return *this;
}
// *****************************************************************************************************
template <typename T>
inline void C_SmartPtr<T>::AddRef()
{
	if ( m_Ptr )
		m_Ptr->AddRef();
}
// *****************************************************************************************************
template <typename T>
inline void C_SmartPtr<T>::Release()
{
	if ( (m_Ptr) && (0 == m_Ptr->ReleaseRef()) )
		delete m_Ptr;
}
// *****************************************************************************************************
} } // namespace ae { namespace base {

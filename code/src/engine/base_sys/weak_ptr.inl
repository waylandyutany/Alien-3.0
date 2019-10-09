namespace ae { namespace base {

// *****************************************************************************************************
// *****************************************************************************************************
inline C_WeakPtrProxyBase::C_WeakPtrProxyBase() : m_RefCnt(0) 
{
}

inline void C_WeakPtrProxyBase::AddRef() const
{
	AE_ASSERT(m_RefCnt >= 0);
	++m_RefCnt;
}

inline void C_WeakPtrProxyBase::DecRef() const
{
	AE_ASSERT(m_RefCnt > 0);

	if (--m_RefCnt == 0)
	{
		delete this;
	}
}
// *****************************************************************************************************
// *****************************************************************************************************

template <typename T>
inline C_WeakPtrProxy<T>::C_WeakPtrProxy(T* ptr) : m_Ptr(ptr)
{
}

template <typename T>
inline T* C_WeakPtrProxy<T>::GetPtr() const
{
	return m_Ptr;
}

template <typename T>
inline void C_WeakPtrProxy<T>::NotifyObjDied()
{
	m_Ptr = NULL;
}
// *****************************************************************************************************
// *****************************************************************************************************
template <typename T>
inline C_WeakPtr<T>::C_WeakPtr() : m_Proxy(NULL)
{
}

template <typename T>
inline C_WeakPtr<T>::C_WeakPtr(T * other) : m_Proxy(NULL)
{
	operator=(other);
}

template <typename T>
inline C_WeakPtr<T>::C_WeakPtr(const C_WeakPtr<T> &other) : m_Proxy(NULL)
{
	operator=(other);
}

template <typename T>
inline C_WeakPtr<T>::~C_WeakPtr()
{
	if (m_Proxy)
		m_Proxy->DecRef();
}

template <typename T>
inline C_WeakPtr<T>::operator T* () const
{
	return m_Proxy ? m_Proxy->GetPtr() : NULL;
}

template <typename T>
inline T* C_WeakPtr<T>::operator *() const
{
	return m_Proxy ? m_Proxy->GetPtr() : NULL;
}

template <typename T>
inline bool C_WeakPtr<T>::operator!() const
{
	return m_Proxy ? !m_Proxy->GetPtr() : true;
}

template <typename T>
inline T* C_WeakPtr<T>::operator->() const
{
	return m_Proxy ? m_Proxy->GetPtr() : NULL;
}

template <typename T>
template <typename U>
inline C_WeakPtr<T>& C_WeakPtr<T>::operator = (U * other)
{
	if (m_Proxy)
		m_Proxy->DecRef();

	m_Proxy	= other ? reinterpret_cast<C_WeakPtrProxy<T> *>(other->GetWeakPtrProxy()) : NULL;
	return *this;
}

template <typename T>
template <typename U>
inline C_WeakPtr<T>::operator C_WeakPtr<U>() const
{
	C_WeakPtr<U> res;
	res.SetFrom(m_Proxy ? m_Proxy->GetPtr() : NULL);
	return res;
}

template <typename T>
inline C_WeakPtr<T>& C_WeakPtr<T>::operator = (const C_WeakPtr<T> &other)
{
	if (m_Proxy)
		m_Proxy->DecRef();

	if ((m_Proxy = other.m_Proxy))
		m_Proxy->AddRef();

	return *this;
}

template <typename T>
inline T* C_WeakPtr<T>::Get() const
{
	return m_Proxy ? m_Proxy->GetPtr() : NULL;
}

template <typename T>
inline bool C_WeakPtr<T>::IsNULL() const
{
	return m_Proxy ? !m_Proxy->GetPtr() : true;
}

template <typename T>
inline void C_WeakPtr<T>::Release()
{
	if (m_Proxy)
	{
		m_Proxy->DecRef();
		m_Proxy = NULL;
	}
}

template <typename T>
template <typename U>
inline T* C_WeakPtr<T>::SetFrom (U* other)
{
	if (m_Proxy)
		m_Proxy->DecRef();

	m_Proxy	= other ? reinterpret_cast<C_WeakPtrProxy<T> *>(other->GetWeakPtrProxy()) : NULL;
	return m_Proxy ? m_Proxy->GetPtr() : NULL;
}

// *****************************************************************************************************
// *****************************************************************************************************
template <typename T>
inline C_WeakRefObj<T>::C_WeakRefObj(T * obj) : m_Proxy(NULL), m_Obj(obj)
{
}

template <typename T>
inline C_WeakRefObj<T>::~C_WeakRefObj()
{
	if (m_Proxy)
	{
		m_Proxy->NotifyObjDied();
		m_Proxy->DecRef();
	}
}

template <typename T>
inline C_WeakPtrProxy<T>* C_WeakRefObj<T>::GetWeakPtrProxy() const
{
	if (!m_Proxy)
	{
		m_Proxy = new T_Proxy(m_Obj);
		m_Proxy->AddRef();
	}

	m_Proxy->AddRef();
	return m_Proxy;
}
// *****************************************************************************************************
// *****************************************************************************************************
} } // namespace ae { namespace base {

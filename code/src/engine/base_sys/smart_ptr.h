#pragma once

#ifndef AE_BASE_SYS_SMART_PTR_H
#define AE_BASE_SYS_SMART_PTR_H

namespace ae { namespace base {

template <typename T> class C_SmartPtr;
class C_RefCounter
{
	template <typename T> friend class C_SmartPtr;
public:
	C_RefCounter() : m_RefCount(0) {}
private:
	void AddRef() const { ++m_RefCount; }
	int ReleaseRef() const { AE_ASSERT(m_RefCount > 0); return --m_RefCount; }
	mutable ae::u32	m_RefCount;
};

template <typename T>
class C_SmartPtr
{
public:
	explicit C_SmartPtr( T *ptr ) : m_Ptr(ptr)
	{ AddRef(); }

	C_SmartPtr( const C_SmartPtr& src );

	template <typename U>
	C_SmartPtr( const C_SmartPtr<U> &src ) : m_Ptr(src.get())
	{ AddRef();	}

	~C_SmartPtr()
	{ Release(); }

	T* operator ->() const;
	T* get() const { return m_Ptr; }
	C_SmartPtr& operator =( const C_SmartPtr& src );

	template <typename U>
	C_SmartPtr& operator=(const C_SmartPtr<U>& src);

	bool operator ==(const C_SmartPtr& src) const { return m_Ptr == src.m_Ptr; }
	bool operator !=(const C_SmartPtr& src) const { return m_Ptr != src.m_Ptr; }

private:
	void AddRef();
	void Release();

	T *m_Ptr;
};

} } // namespace ae { namespace base {

#include "smart_ptr.inl"

#endif // #ifdef AE_BASE_SYS_SMART_PTR_H

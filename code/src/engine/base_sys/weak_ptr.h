#pragma once

#ifndef AE_BASE_SYS_WEAK_PTR_H
#define AE_BASE_SYS_WEAK_PTR_H

namespace ae { namespace base {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_WeakPtrProxyBase
	{
	public:
		C_WeakPtrProxyBase();
		virtual ~C_WeakPtrProxyBase() { AE_ASSERT(m_RefCnt == 0); }
		void AddRef() const;
		void DecRef() const;
	private:
		mutable int m_RefCnt;
	};
// *****************************************************************************************************
// *****************************************************************************************************
	template <typename T>
	class C_WeakPtrProxy : public C_WeakPtrProxyBase
	{
	public:
		C_WeakPtrProxy(T * ptr);
		T* GetPtr() const;
		void NotifyObjDied();
	private:
		T* m_Ptr;
	};
// *****************************************************************************************************
// *****************************************************************************************************
	template <typename T>
	class C_WeakPtr
	{
	public:

		typedef T value_type;
		typedef T T_ValueType;

		template <typename U>
		struct rebind 
		{ 
			typedef C_WeakPtr<U> T_Result; 
		};

// *****************************************************************************************************
		C_WeakPtr();
		C_WeakPtr(T * other);
		C_WeakPtr(const C_WeakPtr<T> &other);
		~C_WeakPtr();
		operator T * () const;
		T * operator *() const;
		bool operator!() const;
		T * operator->() const;
		template <typename U>
		C_WeakPtr<T>& operator = (U * other);
		template <typename U>
		operator C_WeakPtr<U>() const;
		C_WeakPtr<T>& operator = (const C_WeakPtr<T> &other);
		T * Get() const;
		bool IsNULL() const;
		void Release();

		template <typename U>
		T * SetFrom (U * other);
	private:
		mutable C_WeakPtrProxy<T>	* m_Proxy;
	};
// *****************************************************************************************************
// *****************************************************************************************************
	template <typename T>
	class C_WeakRefObj
	{
	public:
		C_WeakRefObj(T * obj);
		~C_WeakRefObj();
		C_WeakPtrProxy<T> * GetWeakPtrProxy() const;
	private:
		typedef C_WeakPtrProxy<T> T_Proxy;
		mutable T_Proxy* m_Proxy;
		T* m_Obj;
	};
// *****************************************************************************************************
// *****************************************************************************************************
} } // namespace ae { namespace base {

#include "weak_ptr.inl"

#endif // #ifdef AE_BASE_SYS_WEAK_PTR_H

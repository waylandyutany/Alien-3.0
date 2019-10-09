#pragma once
#ifndef AE_TEMPLATES_H
#define AE_TEMPLATES_H

namespace ae {
	
	template<typename T_Object>
	void SafeRelease(T_Object*&);

	template<typename T_Allocator>
	const char* AllocString(T_Allocator& allocator, const char* str);

	template<typename T_Struct,typename T_Allocator>
	T_Struct* CreateStruct(T_Struct*& beginStruct, T_Allocator&);

	template<typename T>
	void AppendStruct(T* newStruct, T** structs);

	template<typename T_Struct>
	T_Struct* FindStruct(const char* id, T_Struct*& beginStruct);

	template<typename T_Struct>
	T_Struct* FindStructHID(const char* id, T_Struct*& beginStruct);

	template<typename T_Struct>
	void ZeroStruct(T_Struct* pStruct);

	template<typename T_Container>
	void Purge(T_Container& container);

	template<typename T_Container, typename T_Element>
	void RemoveElement(T_Container&, T_Element element);

	template<typename T_Array>
	size_t ArraySize(const T_Array& a);

} // namespace ae {

#include "templates.inl"

#endif // #ifdef AE_TEMPLATES_H

#pragma once

#ifndef AE_RESOURCE_SYS_RESOURCE_HANDLER_H
#define AE_RESOURCE_SYS_RESOURCE_HANDLER_H

#include "../resource.h"

namespace ae { namespace resource {
// *********************************************************************************************************
class C_ResourcesManager;
// *********************************************************************************************************
// *********************************************************************************************************
class I_ResourceElementsEnumerator
{
public:
	virtual void OnElement(const char* elementName, T_ResourceType elementType) = 0;
	virtual ~I_ResourceElementsEnumerator() {}
};
// *********************************************************************************************************
// *********************************************************************************************************
class I_ResourceHandler
{
	NOCOPY_CLASS(I_ResourceHandler);
	friend class C_ResourcesManager;
// *********************************************************************************************************
	S_Resource *m_pBeginResource,*m_pEndResource;
	void AssignResource(S_Resource* pBeginResource, S_Resource* pEndResource);
protected:
	I_ResourceHandler(S_Resource* pResource);
	S_Resource* GetResource();
	S_Resource* GetEndResource();
	void ResolvePointers();
// *********************************************************************************************************
	// Activate is called after resource has been loaded in memory
	virtual void Activate();
	// Deactivate is called before resource is unloaded from memory
	virtual void Deactivate();
// *********************************************************************************************************
	virtual void EnumerateElements(I_ResourceElementsEnumerator&) {}
	virtual void* FindElement(const char* /*elementName*/) { return 0; };
// *********************************************************************************************************
	virtual void PrintDebug();
public:
	I_ResourceHandler() {};
	virtual ~I_ResourceHandler() {}
};
// *********************************************************************************************************
// *********************************************************************************************************
} } // namespace ae { namespace resource {

#include "resource_handler.inl"

#endif // #ifdef AE_RESOURCE_SYS_RESOURCE_HANDLER_H

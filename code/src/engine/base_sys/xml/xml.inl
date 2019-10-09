#include "../../debug_sys/output/outputs.h"

namespace ae { namespace base {

template<typename T_Allocator>
inline C_XML<T_Allocator>::C_XML(T_Allocator& allocator) : 
	m_Allocator(allocator),
	m_pRootNode(0)
{
	Destroy();
}

template<typename T_Allocator>
inline C_XML<T_Allocator>::~C_XML()
{
	DestroyNode(m_pRootNode);
}

template<typename T_Allocator>
inline void C_XML<T_Allocator>::Destroy()
{
	DestroyNode(m_pRootNode);

	m_pRootNode = static_cast<S_XMLNode*>(m_Allocator.Alloc(sizeof S_XMLNode));
	ae::ZeroStruct(m_pRootNode);

	S_XMLNode* pNode = ae::CreateStruct(m_pRootNode->child, m_Allocator);
	ae::ZeroStruct(pNode);
	pNode->hid.id = ae::AllocString(m_Allocator, "root");
	pNode->hid.idHash = CalculateHashI32(pNode->hid.id);
}

template<typename T_Allocator>
inline const S_XMLNode& C_XML<T_Allocator>::Root() const
{
	AE_ASSERT(m_pRootNode && "Root node is still valid !");
	return *m_pRootNode->child;
}

template<typename T_Allocator>
inline void C_XML<T_Allocator>::PrintLevelSpaces(ae::u32 level)
{
	for(ae::u32 i = 0; i < level; i++)
		DBG_OUT << " ";
}

template<typename T_Allocator>
inline void C_XML<T_Allocator>::PrintDebug(S_XMLNode* pParentNode, ae::u32 level)
{
	S_XMLNode* pNode(pParentNode);
	while(pNode)
	{
		PrintLevelSpaces(level);
		DBG_OUT << "<" << pNode->hid.id << ">" << std::endl;

		PrintDebug(pNode->child, level + 3);
// *****************************************************************************************************
// Element
		if(pNode->element)
		{
			PrintLevelSpaces(level + 1);
			DBG_OUT << "[" << pNode->element << "]" << std::endl;
		}
// Attributes
		S_XMLAttribute* pAttribute(pNode->attributes);
		while(pAttribute)
		{
			PrintLevelSpaces(level + 1);
			DBG_OUT << "[" << pAttribute->hid.id << " : " << pAttribute->value << "]" << std::endl;
			pAttribute = pAttribute->next;
		}
// *****************************************************************************************************

		PrintLevelSpaces(level);
		DBG_OUT << "</" << pNode->hid.id << ">" << std::endl;

		pNode = pNode->next;
	}
}

template<typename T_Allocator>
inline void C_XML<T_Allocator>::PrintDebug()
{
	if(m_pRootNode)
		PrintDebug(m_pRootNode->child, 0);
}

template<typename T_Allocator>
inline void C_XML<T_Allocator>::DestroyAttribute(S_XMLAttribute* pAttribute)
{
	m_Allocator.Free(const_cast<char*>(pAttribute->hid.id));
	m_Allocator.Free(const_cast<char*>(pAttribute->value));
	m_Allocator.Free(pAttribute);
}

template<typename T_Allocator>
inline void C_XML<T_Allocator>::DestroyNode(S_XMLNode* pParentNode)
{
	S_XMLNode* pNode(pParentNode);
	while(pNode)
	{
		DestroyNode(pNode->child);
		S_XMLNode* pDeletableNode(pNode);
		pNode = pNode->next;

		S_XMLAttribute* pAttribute(pDeletableNode->attributes);
		while(pAttribute)
		{
			S_XMLAttribute* pDeletableAttribute(pAttribute);
			pAttribute = pAttribute->next;

			DestroyAttribute(pDeletableAttribute);
		}

		m_Allocator.Free(const_cast<char*>(pDeletableNode->element));
		m_Allocator.Free(const_cast<char*>(pDeletableNode->hid.id));
		m_Allocator.Free(pDeletableNode);
	}
}

} } // namespace ae { namespace base {

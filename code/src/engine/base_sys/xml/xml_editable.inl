namespace ae { namespace base {

template<typename T_Allocator>
inline C_XML_Editable<T_Allocator>::C_XML_Editable(T_Allocator& allocator) :
	C_XML(allocator)
{
}

template<typename T_Allocator>
inline C_XML_Editable<T_Allocator>::~C_XML_Editable()
{
}


template<typename T_Allocator>
inline S_XMLNode& C_XML_Editable<T_Allocator>::Root()
{
	AE_ASSERT(m_pRootNode && "Root node is still valid !");
	return *m_pRootNode->child;
}

template<typename T_Allocator>
inline S_XMLNode& C_XML_Editable<T_Allocator>::AddNode(S_XMLNode& parent, const char* nodeName)
{
	if(!parent.IsValid()) return g_DummyXMLNode;

	S_XMLNode* pNode = ae::CreateStruct(parent.child, m_Allocator);
	ae::ZeroStruct(pNode);
	pNode->hid.id = ae::AllocString(m_Allocator, nodeName);
	pNode->hid.idHash = CalculateHashI32(pNode->hid.id);

	return *pNode;
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::DelNode(S_XMLNode& parent, const char* nodeName)
{
	T_Hash32 idHash = ae::base::CalculateHashI32(nodeName);
	if(!parent.child) return;

	if(parent.child->hid.idHash == idHash)
	{
		S_XMLNode* pNodeDeletable(parent.child);
		parent.child = pNodeDeletable->next;
		pNodeDeletable->next = 0;
		DestroyNode(pNodeDeletable);
	}
	else
	{
		S_XMLNode* pNode(parent.child);
		while(pNode->next)
		{
			if(pNode->next->hid.idHash == idHash)
			{
				S_XMLNode* pNodeDeletable(pNode->next);
				pNode->next = pNodeDeletable->next;
				pNodeDeletable->next = 0;
				DestroyNode(pNodeDeletable);
				return;
			}

			pNode = pNode->next;
		}
	}
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::DelNode(S_XMLNode& parent, S_XMLNode* pNodeToDelete)
{
	if(!parent.child) return;
	if(parent.child == pNodeToDelete)
	{
		parent.child = pNodeToDelete->next;
		pNodeToDelete->next = 0;
		DestroyNode(pNodeToDelete);
	} else {
		S_XMLNode* pNode(parent.child);
		while(pNode->next)
		{
			if(pNode->next == pNodeToDelete)
			{
				pNode->next = pNodeToDelete->next;
				pNodeToDelete->next = 0;
				DestroyNode(pNodeToDelete);
				return;
			}

			pNode = pNode->next;
		}
	}
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::RenameNode(S_XMLNode& node, const char* nodeName)
{
	if(!node.IsValid()) return;

	m_Allocator.Free(const_cast<char*>(node.hid.id));
	node.hid.id = ae::AllocString(m_Allocator, nodeName);
	node.hid.idHash = CalculateHashI32(nodeName);
}

template<typename T_Allocator>
inline S_XMLAttribute* C_XML_Editable<T_Allocator>::GetValidAttribute(S_XMLNode& node, const char* attributeName)
{
	S_XMLAttribute* pAttribute = ae::FindStructHID(attributeName, const_cast<S_XMLAttribute*>(node.attributes));
	if(!pAttribute)
	{
		pAttribute = ae::CreateStruct(node.attributes, m_Allocator);
		ae::ZeroStruct(pAttribute);

		pAttribute->hid.id = ae::AllocString(m_Allocator, attributeName);
		pAttribute->hid.idHash = CalculateHashI32(attributeName);
	}
	return pAttribute;
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::SetAttributeString(S_XMLNode& node, const char* attributeName, const char* value)
{
	if(!node.IsValid()) return;
	S_XMLAttribute* attribute(GetValidAttribute(node, attributeName));
	m_Allocator.Free(const_cast<char*>(attribute->value));
	attribute->value = ae::AllocString(m_Allocator, value);
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::SetAttributeFloat(S_XMLNode& node, const char* attributeName, float value)
{
	char str[32];

	if(!node.IsValid()) return;
	S_XMLAttribute* attribute(GetValidAttribute(node, attributeName));

	sprintf_s(str, 32, "%f", value);
	m_Allocator.Free(const_cast<char*>(attribute->value));
	attribute->value = ae::AllocString(m_Allocator, str);
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::SetAttributeUInt(S_XMLNode& node, const char* attributeName, size_t value)
{
	char str[32];

	if(!node.IsValid()) return;
	S_XMLAttribute* attribute(GetValidAttribute(node, attributeName));

	sprintf_s(str, 32, "%i", value);
	m_Allocator.Free(const_cast<char*>(attribute->value));
	attribute->value = ae::AllocString(m_Allocator, str);
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::SetAttributeInt(S_XMLNode& node, const char* attributeName, ptrdiff_t value)
{
	char str[32];

	if(!node.IsValid()) return;
	S_XMLAttribute* attribute(GetValidAttribute(node, attributeName));

	sprintf_s(str, 32, "%i", value);
	m_Allocator.Free(const_cast<char*>(attribute->value));
	attribute->value = ae::AllocString(m_Allocator, str);
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::DelAttribute(S_XMLNode& node, const char* attrName)
{
	T_Hash32 idHash = ae::base::CalculateHashI32(attrName);
	if(!node.attributes) return;

	if(node.attributes->hid.idHash == idHash)
	{
		S_XMLAttribute* pAttributeDeletable(node.attributes);
		node.attributes = pAttributeDeletable->next;
		pAttributeDeletable->next = 0;
		DestroyAttribute(pAttributeDeletable);
	}
	else
	{
		S_XMLAttribute* pAttribute(node.attributes);
		while(pAttribute->next)
		{
			if(pAttribute->next->hid.idHash == idHash)
			{
				S_XMLAttribute* pAttributeDeletable(pAttribute->next);
				pAttribute->next = pAttributeDeletable->next;
				pAttributeDeletable->next = 0;
				DestroyAttribute(pAttributeDeletable);
				return;
			}

			pAttribute = pAttribute->next;
		}
	}
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::SetElement(S_XMLNode& node, const char* element)
{
	if(!node.IsValid()) return;
	m_Allocator.Free(const_cast<char*>(node.element));
	node.element = ae::AllocString(m_Allocator, element);
}

template<typename T_Allocator>
inline void C_XML_Editable<T_Allocator>::DelElement(S_XMLNode& node)
{
	if(!node.IsValid()) return;
	m_Allocator.Free(const_cast<char*>(node.element));
	node.element = 0;
}

} } // namespace ae { namespace base {

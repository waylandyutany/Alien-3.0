#pragma once

#ifndef AE_BASE_SYS_XML_EDITABLE_H
#define AE_BASE_SYS_XML_EDITABLE_H

#include "xml.h"

namespace ae { namespace base {

	template<typename T_Allocator = C_DefaultAllocator>
	class C_XML_Editable : public C_XML<T_Allocator>
	{
		S_XMLAttribute* GetValidAttribute(S_XMLNode& node, const char* attributeName);
	public:
		C_XML_Editable(T_Allocator& allocator = theDefaultAllocator);
		~C_XML_Editable();

		S_XMLNode& Root();

		S_XMLNode& AddNode(S_XMLNode& parent, const char* nodeName);
		void DelNode(S_XMLNode& parent, const char* nodeName);
		void DelNode(S_XMLNode& parent, S_XMLNode* pNodeToDelete);
		void RenameNode(S_XMLNode& node, const char* nodeName);

		void SetAttributeString(S_XMLNode& node, const char* attributeName, const char* value);
		void SetAttributeFloat(S_XMLNode& node, const char* attributeName, float value);
		void SetAttributeUInt(S_XMLNode& node, const char* attributeName, size_t value);
		void SetAttributeInt(S_XMLNode& node, const char* attributeName, ptrdiff_t value);
		void DelAttribute(S_XMLNode& node, const char* attrName);

		void SetElement(S_XMLNode& node, const char* element);
		void DelElement(S_XMLNode& node);
	};

} } // namespace ae { namespace base {

#include "xml_editable.inl"

#endif // #ifdef AE_BASE_SYS_XML_EDITABLE_H

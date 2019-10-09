#pragma once

#ifndef AE_BASE_SYS_XML_STRUCTS_H
#define AE_BASE_SYS_XML_STRUCTS_H

#include "../hash.h"

namespace ae { namespace base {

	struct S_XMLAttribute
	{
		S_HashID hid;
		const char* value;
		S_XMLAttribute* next;

		size_t AsUInt(size_t defaultUInt = 0) const;
		int AsInt(int defaultInt = 0) const;
		float AsFloat(float defaultFloat = 0.0f) const;
		const char* AsString(const char* defaultString = "") const;

		bool IsValid() const;
		bool Is(const char* name) const { return hid.Is(name); }
	};

	struct S_XMLNode
	{
		S_HashID hid;
		S_XMLNode* next;
		S_XMLNode* child;
		S_XMLAttribute* attributes;
		const char* element;

		const S_XMLNode& Node(const char*) const;
		S_XMLNode& Node(const char*);

		const S_XMLAttribute& Attribute(const char*) const;
		S_XMLAttribute& Attribute(const char*);

		const char* Element(const char* defaultValue = "") const;
		bool IsValid() const;
		bool Is(const char* name) const { return hid.Is(name); }
	};

	extern S_XMLNode g_DummyXMLNode;
	extern S_XMLAttribute g_DummyXMLAttribute;

} } // namespace ae { namespace base {

#include "xml_structs.inl"

#endif // #ifdef AE_BASE_SYS_XML_STRUCTS_H

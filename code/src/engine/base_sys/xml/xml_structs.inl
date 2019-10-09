#include "../../_sys/templates.h"

namespace ae { namespace base {

	
inline bool S_XMLNode::IsValid() const
{
	return hid.id ? true : false;
}

inline bool S_XMLAttribute::IsValid() const
{
	return hid.id ? true : false;
}

inline const S_XMLNode& S_XMLNode::Node(const char* name) const
{
	S_XMLNode* pNode = ae::FindStructHID<>(name, const_cast<S_XMLNode*>(child));
	return pNode ? *pNode : g_DummyXMLNode;
}

inline S_XMLNode& S_XMLNode::Node(const char* name)
{
	S_XMLNode* pNode = ae::FindStructHID<>(name, const_cast<S_XMLNode*>(child));
	return pNode ? *pNode : g_DummyXMLNode;
}

inline const S_XMLAttribute& S_XMLNode::Attribute(const char* name) const
{
	S_XMLAttribute* pAttribute = ae::FindStructHID(name, const_cast<S_XMLAttribute*>(attributes));
	return pAttribute ? *pAttribute : g_DummyXMLAttribute;
}

inline S_XMLAttribute& S_XMLNode::Attribute(const char* name)
{
	S_XMLAttribute* pAttribute = ae::FindStructHID(name, const_cast<S_XMLAttribute*>(attributes));
	return pAttribute ? *pAttribute : g_DummyXMLAttribute;
}

inline const char* S_XMLNode::Element(const char* defaultValue) const
{
	return element ? element : defaultValue;
}

inline size_t S_XMLAttribute::AsUInt(size_t defaultUInt) const
{
	return value ? std::atoi(value) : defaultUInt;
}

inline int S_XMLAttribute::AsInt(int defaultInt) const
{
	return value ? std::atoi(value) : defaultInt;
}

inline float S_XMLAttribute::AsFloat(float defaultFloat) const
{
	return value ? (float)std::atof(value) : defaultFloat;
}

inline const char* S_XMLAttribute::AsString(const char* defaultString) const
{
	return value ? value : defaultString;
}

} } // namespace ae { namespace base {

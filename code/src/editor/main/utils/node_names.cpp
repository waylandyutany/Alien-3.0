#include "stdafx.h"

#if defined(AE_EDITOR)

#include "node_names.h"

#include "../../../engine/base_sys/xml/xml.h"

namespace ae { namespace editor {

bool IsNodeRenameable(ae::base::S_XMLNode& node) {
	return node.Attribute("name").IsValid();
}

bool IsNodeNameUnique(ae::base::S_XMLNode& parentNode, const char* name)
{
	ae::base::S_XMLNode* pNode(parentNode.child);
	while(pNode)
	{
		if(!_stricmp(pNode->Attribute("name").AsString(), name))
			return false;
		pNode = pNode->next;
	}
	return true;
}

std::string GetUniqueName(ae::base::S_XMLNode& parentNode, const char* name, const char* postfix)
{
	std::stringstream retName(name);
	ae::u32 inc(1);
	while(!IsNodeNameUnique(parentNode, retName.str().c_str())) {
		retName.str("");
		retName << name;
		retName << postfix;
		retName << std::setw(3) << std::setfill('0');
		retName << inc;
		inc++;
	}
	return retName.str();
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

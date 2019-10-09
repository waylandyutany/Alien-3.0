
#include "xml_structs.h"

namespace ae { namespace base {

inline void C_XML_Text_Writer::WriteLevel(std::ostream& out, int level)
{
	for(int i = 0; i < level; i++)
		out << " ";
}

inline void C_XML_Text_Writer::WriteNodeLevel(const S_XMLNode& node, std::ostream& out, int level)
{
	WriteLevel(out, level);
	out << "<" << node.hid.id;

	const S_XMLAttribute* pAttribute(node.attributes);
	while(pAttribute)
	{
		out << " " << pAttribute->hid.id << "=\"" << pAttribute->value << "\"";
		pAttribute = pAttribute->next;
	}

	if(!node.child && (node.attributes || !node.element) && level)
	{
		out << "/>" << std::endl;
		return;
	}

	out << ">";

	if(node.element) out << "\"" << node.element << "\"";

	if(node.child || !level) out << std::endl;

	const S_XMLNode* pNode(node.child);
	while(pNode)
	{
		WriteNodeLevel(*pNode, out, level + 3);
		pNode = pNode->next;
	}

	if(node.child) WriteLevel(out, level);
	out << "</" << node.hid.id << ">" << std::endl;
}

inline C_XML_Text_Writer::C_XML_Text_Writer(const C_XML<>& xml, std::ostream& out)
{
	if(xml.Root().IsValid())
		WriteNodeLevel(xml.Root(), out, 0);
}

} } // namespace ae { namespace base {

#include "xml_text_reader.h"
#include "xml_structs.h"
#include "xml.h"

#include "../streams/i_input_stream.h"

#include "../../_sys/templates.h"

namespace ae { namespace base {

template<typename T_Allocator>
inline C_XML_Text_Reader<T_Allocator>::C_XMLInputStream::C_XMLInputStream(I_InputStream& input) :
	m_Input(input)
{
	const size_t streamSize(input.Size());
	m_pBegin = (char*)::malloc(streamSize);
	size_t readed;
	input.Read(m_pBegin, streamSize, readed);
	m_pEnd = m_pBegin + streamSize;
}

template<typename T_Allocator>
inline C_XML_Text_Reader<T_Allocator>::C_XMLInputStream::~C_XMLInputStream()
{
	::free(m_pBegin);
}

template<typename T_Allocator>
C_XML_Text_Reader<T_Allocator>::T_CharParserRules::S_Token C_XML_Text_Reader<T_Allocator>::symbolsTokens[] =	{
													{"<!--"	,E_XML_COMMENT_BEGIN},
													{"-->"	,E_XML_COMMENT_END},
													{"</"	,E_XML_NODE_LEFT_END},
													{"/>"	,E_XML_NODE_RIGHT_END},
													{"<"	,E_XML_NODE_LEFT_BEGIN},
													{">"	,E_XML_NODE_RIGHT_BEGIN},
													{"="	,E_XML_EQUAL},
													{0,0},
												};

template<typename T_Allocator>
inline C_XML_Text_Reader<T_Allocator>::C_XML_Text_Reader(I_InputStream& input, C_XML<T_Allocator>& xml) : 
	m_Input(input),
	m_Rules(0, symbolsTokens),
	m_Parser(m_Input, m_Rules),
	m_pRootNode(xml.m_pRootNode),
	m_Allocator(xml.m_Allocator)
{
	xml.DestroyNode(m_pRootNode);
	m_pRootNode = static_cast<S_XMLNode*>(m_Allocator.Alloc(sizeof S_XMLNode));
	ae::ZeroStruct(m_pRootNode);
	m_NodesQueue.push(m_pRootNode);
	ParseNode(0);
}

template<typename T_Allocator>
inline void C_XML_Text_Reader<T_Allocator>::ParseNode(const char* parentNodeName)
{
	T_Parser::S_TokenInfo tokenInfo;
	std::string nodeName, attributeName;
	bool bShouldBeAttributeValue = false;

	while(m_Parser.NextToken(tokenInfo))
	{
		if(tokenInfo.iTokenType == T_Parser::E_TT_SYMBOL)
		{
// *****************************************************************************************************
			// XML Comments
			if(tokenInfo.iTokenId == E_XML_COMMENT_BEGIN)
			{
				while(m_Parser.NextToken(tokenInfo))
					if(tokenInfo.iTokenType == T_Parser::E_TT_SYMBOL && tokenInfo.iTokenId == E_XML_COMMENT_END)
						break;
			} 
// *****************************************************************************************************
			// Node begin
			else if(tokenInfo.iTokenId == E_XML_NODE_LEFT_BEGIN)
			{
				if(m_Parser.NextToken(tokenInfo) && tokenInfo.iTokenType == T_Parser::E_TT_IDENTIFIER)
				{
					nodeName = tokenInfo.sLexem;
					OnNodeStart(nodeName.c_str());

					while(m_Parser.NextToken(tokenInfo))
					{
// *****************************************************************************************************
						// Node begin and end
						if(tokenInfo.iTokenType == T_Parser::E_TT_SYMBOL)
						{
							if(tokenInfo.iTokenId == E_XML_NODE_RIGHT_BEGIN) // >
							{
								ParseNode(nodeName.c_str());
								break;
							}
							else if(tokenInfo.iTokenId == E_XML_NODE_RIGHT_END) // />
							{
								OnNodeEnd(nodeName.c_str());
								break;
							}
							else if(tokenInfo.iTokenId == E_XML_EQUAL && attributeName.length()) // =
							{
								bShouldBeAttributeValue = true;
							}
						}
// *****************************************************************************************************
						// Attributes
						else if(tokenInfo.iTokenType == T_Parser::E_TT_IDENTIFIER)
						{
							attributeName = tokenInfo.sLexem;
							bShouldBeAttributeValue = false;
						}
						else if(tokenInfo.iTokenType == T_Parser::E_TT_STRING  && bShouldBeAttributeValue)
						{
							OnAttribute(attributeName.c_str(), tokenInfo.sLexem);
							attributeName.clear();
							bShouldBeAttributeValue = false;
						}
					}
				}
			}
// *****************************************************************************************************
			// Node end
			else if(tokenInfo.iTokenId == E_XML_NODE_LEFT_END) // </
			{
				if(m_Parser.NextToken(tokenInfo) && tokenInfo.iTokenType == T_Parser::E_TT_IDENTIFIER)
					if( !strcmp(parentNodeName, tokenInfo.sLexem)  && m_Parser.NextToken(tokenInfo)) 
					{
						OnNodeEnd(parentNodeName);
						break;
					}
			}
// *****************************************************************************************************
			// Element strings
			else if(strlen(tokenInfo.sLexem)) OnElement(tokenInfo.sLexem);
		}
		else if(strlen(tokenInfo.sLexem)) OnElement(tokenInfo.sLexem);
	}
}

template<typename T_Allocator>
inline void C_XML_Text_Reader<T_Allocator>::OnNodeStart(const char* name)
{
	S_XMLNode* parent = m_NodesQueue.top();
	S_XMLNode* pNode = ae::CreateStruct(parent->child, m_Allocator);

	ae::ZeroStruct(pNode);
	pNode->hid.id = ae::AllocString(m_Allocator, name);
	pNode->hid.idHash = CalculateHashI32(pNode->hid.id);
	m_NodesQueue.push(pNode);
}

template<typename T_Allocator>
inline void C_XML_Text_Reader<T_Allocator>::OnNodeEnd(const char* /*name*/)
{
	m_NodesQueue.pop();
}

template<typename T_Allocator>
inline void C_XML_Text_Reader<T_Allocator>::OnAttribute(const char* name, const char* value)
{
	S_XMLNode* parent = m_NodesQueue.top();
	S_XMLAttribute* pAttribute = ae::CreateStruct(parent->attributes, m_Allocator);
	ae::ZeroStruct(pAttribute);

	pAttribute->hid.id = ae::AllocString(m_Allocator, name);
	pAttribute->hid.idHash = CalculateHashI32(name);
	pAttribute->value = ae::AllocString(m_Allocator, value);
}

template<typename T_Allocator>
inline void C_XML_Text_Reader<T_Allocator>::OnElement(const char* string)
{
	S_XMLNode* parent = m_NodesQueue.top();
	if(!parent->element)
		parent->element = ae::AllocString(m_Allocator, string);
}

} } // namespace ae { namespace base {

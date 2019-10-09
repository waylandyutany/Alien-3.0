#pragma once

#ifndef AE_BASE_SYS_XML_TEXT_READER_H
#define AE_BASE_SYS_XML_TEXT_READER_H

#include "xml.h"

#include "../parser/base_parser_rules.h"
#include "../parser/parser_input_stream.h"
#include "../parser/parser.h"

#include "../memory/allocators/allocator.h"

namespace ae { namespace base {

class I_InputStream;
struct S_XMLNode;

template<typename T_Allocator = C_DefaultAllocator>
class C_XML_Text_Reader
{
	class C_XMLInputStream
	{
		I_InputStream& m_Input;
		NOCOPY_CLASS(C_XMLInputStream);
		char *m_pBegin, *m_pEnd;
	public:
		typedef char* T_Iterator;
		C_XMLInputStream(I_InputStream& input);
		~C_XMLInputStream();
		T_Iterator Begin() const { return m_pBegin; }
		T_Iterator End() const {return m_pEnd; }
	};

	enum E_XMLEnum
	{
		E_XML_COMMENT_BEGIN,	// <!--
		E_XML_COMMENT_END,		// -->
		E_XML_NODE_LEFT_BEGIN,	// <
		E_XML_NODE_LEFT_END,	// </
		E_XML_NODE_RIGHT_BEGIN,	// >
		E_XML_NODE_RIGHT_END,	// />
		E_XML_EQUAL,			// =
	};
	
	C_XMLInputStream m_Input;
	typedef ae::base::C_BaseParserRules<char> T_CharParserRules;
	typedef ae::base::C_Parser<char, C_XMLInputStream, T_CharParserRules, 512> T_Parser;
	typedef typename T_Parser::S_TokenInfo T_TokenInfo;

	T_CharParserRules m_Rules;
	T_Parser m_Parser;
	static T_CharParserRules::S_Token symbolsTokens[];
	S_XMLNode*& m_pRootNode;
	T_Allocator& m_Allocator;

	std::stack<S_XMLNode*> m_NodesQueue;

	void ParseNode(const char*);
	void OnNodeStart(const char* name);
	void OnNodeEnd(const char* name);
	void OnAttribute(const char* name, const char* value);
	void OnElement(const char* string);
public:
	C_XML_Text_Reader(I_InputStream& input, C_XML<T_Allocator>& xml);
};


} } // namespace ae { namespace base {

#include "xml_text_reader.inl"

#endif // #ifdef AE_BASE_SYS_XML_TEXT_READER_H

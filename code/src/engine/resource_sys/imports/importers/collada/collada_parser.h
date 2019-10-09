#pragma once

#ifndef AE_RESOURCE_SYS_COLLADA_PARSER_H
#define AE_RESOURCE_SYS_COLLADA_PARSER_H

#include "../../../../base_sys/parser/base_parser_rules.h"
#include "../../../../base_sys/parser/parser_input_stream.h"
#include "../../../../base_sys/parser/parser.h"
#include "../../../../math_sys/types.h"

namespace ae { namespace resource {

class C_ColladaParser
{
public:
	typedef ae::base::C_ParserInputStream<char> T_FileInputStream;

private:
	enum E_ColladaSymbolsXmlEnum
	{
		E_CSXE_NODE_BEGIN,
		E_CSXE_NODE_END,
		E_CSXE_EQUAL,
		E_CSXE_IGNORE
	};

	typedef ae::base::C_BaseParserRules<char> T_CharParserRules;
	typedef ae::base::C_Parser<char, T_FileInputStream, T_CharParserRules, 512> T_Parser;
	typedef T_Parser::S_TokenInfo T_TokenInfo;

	T_CharParserRules m_Rules;
	T_Parser m_Parser;
	T_Parser::S_TokenInfo m_TokenInfo;

	static T_CharParserRules::S_Token symbolsTokens[];

public:
	C_ColladaParser(T_FileInputStream& input);
	~C_ColladaParser();

	const char* NextBeginNode(); // return string to next begin node name
	const char* NextNode(); // return string to next node name

	bool GotoNode(const char* nodeName); // goto to node with given name. Not case sensitive !
	bool GotoAttribute(const char* attributeName); // goto attribute with given name. Not case sensitive !
	bool GotoNode(const char* nodeName, const char* endNode); // goto node before given end node </endNode>

	float GetFloatValue(); // Parse to next string, float or integer token and return it's to float converted value.
	ae::u32 GetUInt32Value(); // Parse to next string, float or integer token and return it's to float converted value.
	const char* GetStringValue(); // Parse to next string token and return it's value.
	std::string GetElementString(); // Parse entire element until end of node

	friend C_ColladaParser& operator >> (C_ColladaParser&, ae::math::float2&);
	friend C_ColladaParser& operator >> (C_ColladaParser&, ae::math::float3&);
	friend C_ColladaParser& operator >> (C_ColladaParser&, ae::math::float4&);
	friend C_ColladaParser& operator >> (C_ColladaParser&, float&);
	friend C_ColladaParser& operator >> (C_ColladaParser&, ae::math::float4x4&);

};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_COLLADA_PARSER_H

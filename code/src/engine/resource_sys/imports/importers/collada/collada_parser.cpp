#include "StdAfx.h"

#include "collada_parser.h"

namespace ae { namespace resource {

C_ColladaParser::T_CharParserRules::S_Token C_ColladaParser::symbolsTokens[] =	{
													{"<?"	,E_CSXE_IGNORE},
													{"?>"	,E_CSXE_IGNORE},
													{"</"	,E_CSXE_NODE_END},

													{"<"	,E_CSXE_NODE_BEGIN},
													{"="	,E_CSXE_EQUAL},
													{0,0},
												};

C_ColladaParser::C_ColladaParser(T_FileInputStream& input) :
	m_Rules(0, symbolsTokens),
	m_Parser(input, m_Rules)
{
}

C_ColladaParser::~C_ColladaParser()
{
}

const char* C_ColladaParser::NextBeginNode()
{
	while(m_Parser.NextToken(m_TokenInfo))
		if(m_TokenInfo.iTokenType == T_Parser::E_TT_SYMBOL && m_TokenInfo.iTokenId == E_CSXE_NODE_BEGIN)
			if(m_Parser.NextToken(m_TokenInfo) && m_TokenInfo.iTokenType == T_Parser::E_TT_IDENTIFIER)
			{
				m_Parser.NextCharacter(); // skip '>' or ' ' after node indentifier
				return m_TokenInfo.sLexem;
			}

	return 0;
}

const char* C_ColladaParser::NextNode()
{
	while(m_Parser.NextToken(m_TokenInfo))
		if(m_TokenInfo.iTokenType == T_Parser::E_TT_SYMBOL && (m_TokenInfo.iTokenId == E_CSXE_NODE_BEGIN || m_TokenInfo.iTokenId == E_CSXE_NODE_END))
			if(m_Parser.NextToken(m_TokenInfo) && m_TokenInfo.iTokenType == T_Parser::E_TT_IDENTIFIER)
			{
				m_Parser.NextCharacter(); // skip '>' or ' ' after node indentifier
				return m_TokenInfo.sLexem;
			}
	return 0;
}

bool C_ColladaParser::GotoNode(const char* nodeName)
{
	const char* nextNode = NextBeginNode();

	while(nextNode)
	{
		if(!_stricmp(nextNode, nodeName))
			return true;

		nextNode = NextBeginNode();
	}

	return false;
}

bool C_ColladaParser::GotoNode(const char* nodeName, const char* endNode)
{
	const char* nextNode = NextNode();
	while(nextNode)
	{
		if(!_stricmp(nextNode, endNode))
			return false;
		else if(!_stricmp(nextNode, nodeName))
			return true;

		nextNode = NextNode();
	}

	return false;
}

bool C_ColladaParser::GotoAttribute(const char* attributeName)
{
	while(m_Parser.NextToken(m_TokenInfo))
		if(m_TokenInfo.iTokenType == T_Parser::E_TT_IDENTIFIER && !_stricmp(m_TokenInfo.sLexem, attributeName))
			if(m_Parser.NextToken(m_TokenInfo) && m_TokenInfo.iTokenType == T_Parser::E_TT_SYMBOL && m_TokenInfo.iTokenId == E_CSXE_EQUAL)
				return true;

	return false;
}

std::string C_ColladaParser::GetElementString()
{
	std::string str;
	while(char chr = m_Parser.NextCharacter())
	{
		if(chr == '<') {
			m_Parser.PreviousCharacter();
			break;
		}
		else str += chr;
	}

	return str;
}

float C_ColladaParser::GetFloatValue()
{
	while(m_Parser.NextToken(m_TokenInfo))
		switch(m_TokenInfo.iTokenType)
		{
		case T_Parser::E_TT_NUMBER:
		case T_Parser::E_TT_REAL_NUMBER:
		case T_Parser::E_TT_STRING:
			return static_cast<float>(atof(m_TokenInfo.sLexem));
		}
	return 0.0f;
}

ae::u32 C_ColladaParser::GetUInt32Value()
{
	return static_cast<ae::u32>(GetFloatValue());
}

const char* C_ColladaParser::GetStringValue()
{
	T_TokenInfo tokenInfo;

	while(m_Parser.NextToken(tokenInfo))
		if(tokenInfo.iTokenType == T_Parser::E_TT_STRING)
			return tokenInfo.sLexem;

	return 0;
}

C_ColladaParser& operator >> (C_ColladaParser& cp, ae::math::float2& flt2)
{
	flt2.x = cp.GetFloatValue();
	flt2.y = cp.GetFloatValue();

	return cp;
}

C_ColladaParser& operator >> (C_ColladaParser& cp, ae::math::float3& flt3)
{
	flt3.x = cp.GetFloatValue();
	flt3.y = cp.GetFloatValue();
	flt3.z = cp.GetFloatValue();

	return cp;
}

C_ColladaParser& operator >> (C_ColladaParser& cp, ae::math::float4& flt4)
{
	flt4.x = cp.GetFloatValue();
	flt4.y = cp.GetFloatValue();
	flt4.z = cp.GetFloatValue();
	flt4.w = cp.GetFloatValue();

	return cp;
}

C_ColladaParser& operator >> (C_ColladaParser& cp, float& flt)
{
	flt = cp.GetFloatValue();

	return cp;
}

C_ColladaParser& operator >> (C_ColladaParser& cp, ae::math::float4x4& flt4x4)
{
	// First row
	cp >> flt4x4.m[0][0] >> flt4x4.m[0][1] >> flt4x4.m[0][2] >> flt4x4.m[0][3];

	// Second row
	cp >> flt4x4.m[1][0] >> flt4x4.m[1][1] >> flt4x4.m[1][2] >> flt4x4.m[1][3];

	// Third row
	cp >> flt4x4.m[2][0] >> flt4x4.m[2][1] >> flt4x4.m[2][2] >> flt4x4.m[2][3];

	// Fourth row
	cp >> flt4x4.m[3][0] >> flt4x4.m[3][1] >> flt4x4.m[3][2] >> flt4x4.m[3][3];

	return cp;
}

} } // namespace ae { namespace resource {


#pragma once
#ifndef AE_BASE_PARSER_RULES_H
#define AE_BASE_PARSER_RULES_H

#include "../memory/allocators/allocator.h"

namespace ae { namespace base {
/*!
	@brief Base class that hold's rules used by the parser.

	Parser using this rules for detecting and separating lexems and assigning their token ids.
	It contains information which litterals are alphabetic or numeric, which lexems are keywords or identifiers and holds keywords.
	This class has 2 specializations. One for chat litterals and second for wchar_t litterals.
	For behaviour changing you can inherit and override methods you want to act differently.

	Keywords, symbols and whitespaces are stored in one compact buffer which may prevent cache misses and therefore improve the performance

	@todo Nice to sort symbols inside constructor.
	@todo We should add symbol's starting litterals for speed up symbols parsing.
*/
	template < typename T_Char, typename T_Allocator = C_DefaultAllocator>
	class C_BaseParserRules
	{
	public:
		struct S_Token
		{
			const T_Char* m_sLexem;
			ae::u32 m_TokenId;
		};
	
/*!
	@brief Constructor that creates BaseParserRules object

	By this constructor you are intializing keywords and symbols that parser should parse

	@param keyWords -	array of keywords and their's token id's.
						!!! For keywords only alphabetic and numeric litterals cen be used !!!
	@param symbols -	array of symbols and their's token id's. 
						!!! Symbols must be sorted with longest symbol's length as first !!!
						!!! No alphabetics or numeric litterals must be used !!!
*/		
		C_BaseParserRules( const S_Token* keyWords, const S_Token* symbols );

/*!
	@brief Constructor that creates BaseParserRules object with whitespaces explicitly defined

	By this constructor you are intializing whitespaces, keywords and symbols that parser should parse

	@param whiteSpaces - zero terminated string that holds whitespaces litterals
	@param keyWords -	array of keywords and their's token id's.
						!!! For keywords only alphabetic and numeric litterals can be used !!!
	@param symbols -	array of symbols and their's token id's. 
						!!! Symbols must be sorted with longest symbol's length as first !!!
						!!! No alphabetics or numeric litterals can be used !!!
*/		
		C_BaseParserRules( const T_Char* whiteSpaces, const S_Token* keyWords, const S_Token* symbols );
		~C_BaseParserRules();

		bool IsWhitespace(T_Char) const;
		bool IsAlphabet(T_Char) const;
		bool IsNumber(T_Char) const;
		bool IsFloatingPoint(T_Char) const;
		bool IsMinusSign(T_Char) const;
		bool IsStringSeparator(T_Char) const;
		bool IsNewLine(T_Char) const;
		bool IsSymbol(const T_Char* strBegin, const T_Char* strEnd, size_t& length, ae::u32& tokenID) const;
		bool IsKeyWord(const T_Char* strBegin, const T_Char* strEnd, ae::u32& tokenID) const;

	private:
		struct S_CompactToken
		{
			size_t m_uiLexemLength;
			T_Char* m_sLexem;
			ae::u32 m_TokenId;
			S_CompactToken* m_pNextCompactToken;
		};

		T_Char*	m_sWhitespaces;
		S_CompactToken*	m_pKeyWords;
		S_CompactToken*	m_pSymbols;
		T_Allocator m_Allocator;	
		char* m_pCompactBuffer;

		void CreateCompactBuffer(	const T_Char* whiteSpaces, 
									const S_Token* keyWords, 
									const S_Token* symbols,
									size_t (*getstrlen)(const T_Char*),
									errno_t (*copystr)(T_Char*, size_t, const T_Char*)
									);
		void DestroyCompactBuffer();

		bool IsToken(bool bCompareLength, S_CompactToken*, const T_Char* strBegin, const T_Char* strEnd, size_t& length, ae::u32& tokenID) const;
	};

} } // namespace ae { namespace base {

#include "base_parser_rules.inl"

#endif // #ifndef AE_BASE_PARSER_RULES_H
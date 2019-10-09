#pragma once
#ifndef AE_BASE_PARSER_H
#define AE_BASE_PARSER_H

namespace ae { namespace base {

/*!
	@brief Parser class that parsing given input and notificating given callback about parsed lexems(with token IDs).
	
	This class parse every litteral from given input stream and separate lexems according given rules.
	!!! Maximum lexem size + zero termination is specified by maxLexemSize !!!
	!!! If parsed lexem exceeded this length, it's truncated to his length which may cause unpredicted behaviour in parser notifications !!!

	@todo Handle lexem buffer overrun detection and make notifications about error.
	@todo Maybe we can speedup parsing and notificating by adding some sort of tokens cache and do notifications after cache is full
		  this may prevent some cache misses. But first we have to profile it anyway.
	@todo We shoud trie to have T_StateFuncParamcreated dynamicaly created after the memory when parser rules compact buffer is allocated
		  that may encrease performance by preventing l1 to l2 caches misses
*/
	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize = 256 >
	class C_Parser
	{
	public:
		enum E_TokenType
		{
			E_TT_IDENTIFIER,
			E_TT_KEYWORD,
			E_TT_SYMBOL,
			E_TT_STRING,
			E_TT_NUMBER,
			E_TT_REAL_NUMBER,
			E_TT_NEW_LINE,
			E_TT_END_OF_INPUT,
			E_TT_UNKNOWN,
			E_TT_ERROR
		};

		struct S_TokenInfo
		{
			S_TokenInfo(const T_Char* lexem, ae::u32 tokenType, ae::u32 tokenId) :
				sLexem(lexem), iTokenType(tokenType), iTokenId(tokenId)
			{}
			S_TokenInfo() :
				sLexem(0), iTokenType(E_TT_UNKNOWN), iTokenId(0)
			{}

			const T_Char* sLexem;
			E_TokenType iTokenType;
			ae::u32 iTokenId;
		};

		typedef S_TokenInfo T_TokenInfo;

	private:
		const C_Parser& operator=(const C_Parser&);

		T_ParserRules& m_Rules;
		T_InputStream& m_Input;

		typedef typename T_InputStream::T_Iterator T_InputIterator;

		enum E_Parser_Token_State
		{
			E_PTS_UNKNOWN = 0,
			E_PTS_IDENTIFIER,
			E_PTS_STRING,
			E_PTS_NUMBER,
			E_PTS_REAL_NUMBER,
			E_PTS_MAX
		};

		struct S_StateFuncParam
		{
			T_InputIterator beginLiteralIt;
			T_InputIterator endLiteralIt;
			T_InputIterator literalIt;
			T_Char lexemStr[maxLexemSize];
			T_Char *pLexem, *pMaxLexem;
			bool bTokenParsed;
			S_StateFuncParam() : pLexem(lexemStr), pMaxLexem(&lexemStr[maxLexemSize-1]), bTokenParsed(false) {}

			void ClearLexem() {
				pLexem = lexemStr;
				memset(lexemStr,0,sizeof(lexemStr));
			}

			void AddToLexem(T_Char _lit) {
				// TODO - Handle buffer overflow
				if(pLexem < pMaxLexem)
					*pLexem++ = _lit;
			}
		};

		typedef S_StateFuncParam T_StateFuncParam;
		typedef void (C_Parser::*T_TokenStateFunc)(T_StateFuncParam&);

		E_Parser_Token_State m_TokenState;
		T_TokenStateFunc m_pTokenStateFunc;
		T_StateFuncParam m_StateFuncPrm;
		S_TokenInfo m_TokenInfo;

		void SetTokenState(E_Parser_Token_State);

		void UnknownTokenState(T_StateFuncParam&);
		void IdentifierTokenState(T_StateFuncParam&);
		void StringTokenState(T_StateFuncParam&);
		void NumberTokenState(T_StateFuncParam&);
		void RealNumberTokenState(T_StateFuncParam&);

		void OnToken(bool bKeepCurrentLiteral, T_StateFuncParam&, E_TokenType tokenType, ae::u32 tokenID = 0);
	public:
		/// Construct parser object with input stream, parser rules and parser callback
		C_Parser(T_InputStream&, T_ParserRules&);

		bool NextToken(T_TokenInfo&);
		T_Char NextCharacter();
		T_Char PreviousCharacter();
	};

} } // namespace ae { namespace base {

#include "parser.inl"

#endif // #ifndef AE_BASE_PARSER_H
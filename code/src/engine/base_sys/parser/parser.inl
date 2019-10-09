namespace ae { namespace base {

// *****************************************************************************************

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		C_Parser(T_InputStream& input, T_ParserRules& rules) :
		m_Input(input), m_Rules(rules)
	{
		SetTokenState(E_PTS_UNKNOWN);

		m_StateFuncPrm.literalIt = m_StateFuncPrm.beginLiteralIt = m_Input.Begin();
		m_StateFuncPrm.endLiteralIt = m_Input.End();
		m_StateFuncPrm.ClearLexem();
	}

// *****************************************************************************************

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline void C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		SetTokenState(E_Parser_Token_State tokenState)
	{
		static struct S_TokenState2Func
		{
			E_Parser_Token_State tokenState;
			T_TokenStateFunc tokenStateFunc;
		} tokenStates2Func[] = {
			{E_PTS_UNKNOWN,		&C_Parser::UnknownTokenState},
			{E_PTS_IDENTIFIER,	&C_Parser::IdentifierTokenState},
			{E_PTS_STRING,		&C_Parser::StringTokenState},
			{E_PTS_NUMBER,		&C_Parser::NumberTokenState},
			{E_PTS_REAL_NUMBER, &C_Parser::RealNumberTokenState}
		};
		static const ae::u32 stateFunctionsCount = sizeof(tokenStates2Func)/sizeof(*tokenStates2Func);

		for(int i = 0; i < stateFunctionsCount; i++)
			if(tokenStates2Func[i].tokenState == tokenState)
			{
				m_TokenState = tokenState;
				m_pTokenStateFunc = tokenStates2Func[i].tokenStateFunc;
				return;
			}

		AE_ASSERT(0 && "Invalid token state given !");
	}


// *****************************************************************************************

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline void C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		UnknownTokenState(T_StateFuncParam& prm)
		{
			size_t symbolLength;
			ae::u32 symbolTokenID;

			if(m_Rules.IsAlphabet(*prm.literalIt))
			{
				prm.AddToLexem(*prm.literalIt);
				SetTokenState(E_PTS_IDENTIFIER);
			}
			else if(m_Rules.IsNewLine(*prm.literalIt))
			{
				OnToken(false, prm, E_TT_NEW_LINE);
			}
			else if(m_Rules.IsWhitespace(*prm.literalIt))
			{
			}
			else if(m_Rules.IsStringSeparator(*prm.literalIt))
			{
				SetTokenState(E_PTS_STRING);
			}
			else if(m_Rules.IsMinusSign(*prm.literalIt) && m_Rules.IsNumber(prm.literalIt[1]))
			{
				prm.AddToLexem(*prm.literalIt);
				SetTokenState(E_PTS_NUMBER);
			}
			else if(m_Rules.IsNumber(*prm.literalIt))
			{
				prm.AddToLexem(*prm.literalIt);
				SetTokenState(E_PTS_NUMBER);
			}
			// m_Rules.IsSymbol is most time consuming so must be called at the very end
			else if(m_Rules.IsSymbol(prm.literalIt, prm.endLiteralIt, symbolLength, symbolTokenID))
			{
				for(ae::u32 i = 0; i < symbolLength; i++)
					prm.AddToLexem(*prm.literalIt++);

				OnToken(true, prm, E_TT_SYMBOL, symbolTokenID);
			}
		}

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline void C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		IdentifierTokenState(T_StateFuncParam& prm)
		{
			if(m_Rules.IsAlphabet(*prm.literalIt) || m_Rules.IsNumber(*prm.literalIt))
				prm.AddToLexem(*prm.literalIt);
			else
			{
				ae::u32 keywordTokenID;
				if(m_Rules.IsKeyWord(prm.lexemStr, prm.pLexem, keywordTokenID))
					OnToken(true, prm, E_TT_KEYWORD,keywordTokenID);
				else
					OnToken(true, prm, E_TT_IDENTIFIER);
			}
		}

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline void C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		StringTokenState(T_StateFuncParam& prm)
		{
			if(m_Rules.IsStringSeparator(*prm.literalIt))
				OnToken(false, prm, E_TT_STRING);
			else
				prm.AddToLexem(*prm.literalIt);
		}

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline void C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		NumberTokenState(T_StateFuncParam& prm)
		{
			if(m_Rules.IsFloatingPoint(*prm.literalIt) && m_Rules.IsNumber(prm.literalIt[1]))
			{
				prm.AddToLexem(*prm.literalIt);
				SetTokenState(E_PTS_REAL_NUMBER);
			}
			else if(m_Rules.IsNumber(*prm.literalIt))
				prm.AddToLexem(*prm.literalIt);
			else
				OnToken(true, prm, E_TT_NUMBER);
		}

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline void C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		RealNumberTokenState(T_StateFuncParam& prm)
		{
			if(m_Rules.IsNumber(*prm.literalIt))
				prm.AddToLexem(*prm.literalIt);
			else
				OnToken(true, prm, E_TT_REAL_NUMBER);
		}

// *****************************************************************************************
	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline T_Char C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::PreviousCharacter()
	{
		if(m_StateFuncPrm.literalIt > m_StateFuncPrm.beginLiteralIt)
			return *--m_StateFuncPrm.literalIt;
		return 0;
	}

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline T_Char C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::NextCharacter()
	{
		if(m_StateFuncPrm.literalIt < m_StateFuncPrm.endLiteralIt)
			return *m_StateFuncPrm.literalIt++;
		return 0;
	}
// *****************************************************************************************
	
	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline bool C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		NextToken(T_TokenInfo& tokenInfo)
	{
		m_StateFuncPrm.ClearLexem();
		m_StateFuncPrm.bTokenParsed = false;

		while(m_StateFuncPrm.literalIt < m_StateFuncPrm.endLiteralIt)
		{
			(this->*m_pTokenStateFunc)(m_StateFuncPrm);
			m_StateFuncPrm.literalIt++;

			if(m_StateFuncPrm.bTokenParsed)
			{
				tokenInfo = m_TokenInfo;
				return true;
			}
		}

		m_StateFuncPrm.ClearLexem();
		OnToken(false, m_StateFuncPrm, E_TT_END_OF_INPUT);
		tokenInfo = m_TokenInfo;

		return false;
	}

	template < typename T_Char, typename T_InputStream, typename T_ParserRules, ae::u32 maxLexemSize>
	inline void C_Parser< T_Char, T_InputStream, T_ParserRules, maxLexemSize >::
		OnToken(bool bKeepCurrentLiteral, T_StateFuncParam& prm, E_TokenType tokenType, ae::u32 tokenID)
	{
		m_TokenInfo.sLexem	= prm.lexemStr;
		m_TokenInfo.iTokenType = tokenType;
		m_TokenInfo.iTokenId = tokenID;

		// after every token func call, next litteral is taken from the input stream
		// by prm.literalIt-- we ensure that current litteral is kept
		if(bKeepCurrentLiteral)
			prm.literalIt--;

		prm.bTokenParsed = true;

		SetTokenState(E_PTS_UNKNOWN);
	}

} } // namespace ae { namespace base {

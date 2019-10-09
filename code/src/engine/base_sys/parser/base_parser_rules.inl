namespace ae { namespace base {

	template < typename T_Char, typename T_Allocator >
	C_BaseParserRules<T_Char, T_Allocator>::C_BaseParserRules( const S_Token* keyWords, const S_Token* symbols)
	{
		AE_CT_ASSERT(0 && "Only char and wchar_t specialization can be used !");
	}

	template < typename T_Char, typename T_Allocator >
	C_BaseParserRules<T_Char, T_Allocator>::C_BaseParserRules( const T_Char* whiteSpaces, const S_Token* keyWords, const S_Token* symbols)
	{
		AE_CT_ASSERT(0 && "Only char and wchar_t specialization can be used !");
	}
 
	template < typename T_Char, typename T_Allocator >
	C_BaseParserRules<T_Char, T_Allocator>::~C_BaseParserRules()
	{
		DestroyCompactBuffer();
	}

	template < typename T_Char, typename T_Allocator >
	inline bool C_BaseParserRules<T_Char, T_Allocator>::IsAlphabet(T_Char char_literal) const {
		return (T_Char('a') <= char_literal && char_literal <= T_Char('z')) || 
			(T_Char('A') <= char_literal && char_literal <= T_Char('Z')) ||
			char_literal == T_Char('_');
	}

	template < typename T_Char, typename T_Allocator >
	inline bool C_BaseParserRules<T_Char, T_Allocator>::IsNumber(T_Char char_literal) const {
		return T_Char('0') <= char_literal && char_literal <= T_Char('9');
	}

	template < typename T_Char, typename T_Allocator >
	inline bool C_BaseParserRules<T_Char, T_Allocator>::IsFloatingPoint(T_Char char_literal) const {
		return char_literal == T_Char('.') ? true : false;
	}

	template < typename T_Char, typename T_Allocator >
	inline bool C_BaseParserRules<T_Char, T_Allocator>::IsMinusSign(T_Char char_literal) const {
		return char_literal == T_Char('-') ? true : false;
	}

	template < typename T_Char, typename T_Allocator >
	inline bool C_BaseParserRules<T_Char, T_Allocator>::IsStringSeparator(T_Char char_literal) const {
		return (char_literal == T_Char('\"') || char_literal == T_Char('\'')) ? true : false;
	}

	template < typename T_Char, typename T_Allocator >
	inline bool C_BaseParserRules<T_Char, T_Allocator>::IsNewLine(T_Char char_literal) const {
		return (char_literal == T_Char('\n')) ? true : false;
	}

	template < typename T_Char, typename T_Allocator >
	inline bool C_BaseParserRules<T_Char, T_Allocator>::IsWhitespace(T_Char char_literal) const {
		const T_Char* strBegin(m_sWhitespaces);
		while(*strBegin)
		{
			if(*strBegin++ == char_literal)
				return true;
		}

		return false;
	}

	template < typename T_Char, typename T_Allocator >
	void C_BaseParserRules<T_Char, T_Allocator>::CreateCompactBuffer(
		const T_Char* whiteSpaces, 
		const S_Token* keyWords, 
		const S_Token* symbols,
		size_t (*getstrlen)(const T_Char*),
		errno_t (*copystr)(T_Char*, size_t, const T_Char*)
		)
	{
		const size_t charSizeShift(sizeof(T_Char)-1);
		const size_t whiteSpacesBufferSize((getstrlen(whiteSpaces) + 1) << charSizeShift); 
		size_t buffSize(whiteSpacesBufferSize);

		// keyword tokens
		const S_Token* tokens(keyWords);
		while(tokens && tokens->m_sLexem)
		{
			buffSize += (getstrlen(tokens->m_sLexem) + 1) << charSizeShift;
			buffSize += sizeof(S_CompactToken);
			tokens++;
		}

		// symbol tokens
		tokens = symbols;
		while(tokens && tokens->m_sLexem)
		{
			buffSize += (getstrlen(tokens->m_sLexem) + 1) << charSizeShift;
			buffSize += sizeof(S_CompactToken);
			tokens++;
		}

		// buffer allocation
		m_pCompactBuffer = (char*)m_Allocator.Alloc(buffSize << 1);
		// fill entire buffer with zero
		memset(m_pCompactBuffer,0,buffSize << 1);

		// whitespace copying
		m_sWhitespaces = (T_Char*)m_pCompactBuffer;
		copystr(m_sWhitespaces, whiteSpacesBufferSize, whiteSpaces);

		// keyword tokens copying
		tokens = keyWords;
		if(tokens && tokens->m_sLexem) // fill buffer only when at least one token is available
		{
			m_pKeyWords = (S_CompactToken*)(m_pCompactBuffer + whiteSpacesBufferSize);
			S_CompactToken* compactToken(m_pKeyWords);
			
			compactToken->m_uiLexemLength = getstrlen(tokens->m_sLexem);
			compactToken->m_TokenId = tokens->m_TokenId;
			compactToken->m_sLexem = (T_Char*)((char*)compactToken + sizeof(S_CompactToken));
			copystr(compactToken->m_sLexem, (compactToken->m_uiLexemLength + 1) << charSizeShift, tokens->m_sLexem);

			tokens++;

			while(tokens->m_sLexem)
			{
				compactToken->m_pNextCompactToken = (S_CompactToken*)((char*)compactToken + sizeof(S_CompactToken) + ((compactToken->m_uiLexemLength + 1) << charSizeShift));
				compactToken = compactToken->m_pNextCompactToken;

				compactToken->m_uiLexemLength = getstrlen(tokens->m_sLexem);
				compactToken->m_TokenId = tokens->m_TokenId;
				compactToken->m_sLexem = (T_Char*)((char*)compactToken + sizeof(S_CompactToken));
				copystr(compactToken->m_sLexem, (compactToken->m_uiLexemLength + 1) << charSizeShift, tokens->m_sLexem);

				tokens++;
			}

			m_pSymbols = (S_CompactToken*)((char*)compactToken + sizeof(S_CompactToken) + ((compactToken->m_uiLexemLength + 1) << charSizeShift));
		}
		else
			m_pKeyWords = 0;

		// symbol tokens copying
		tokens = symbols;
		if(tokens && tokens->m_sLexem) // fill buffer only when at least one token is available
		{
			if(!m_pSymbols)
				m_pSymbols = (S_CompactToken*)(m_pCompactBuffer + whiteSpacesBufferSize);

			S_CompactToken* compactToken(m_pSymbols);
			
			compactToken->m_uiLexemLength = getstrlen(tokens->m_sLexem);
			compactToken->m_TokenId = tokens->m_TokenId;
			compactToken->m_sLexem = (T_Char*)((char*)compactToken + sizeof(S_CompactToken));
			copystr(compactToken->m_sLexem, (compactToken->m_uiLexemLength + 1) << charSizeShift, tokens->m_sLexem);

			tokens++;

			while(tokens->m_sLexem)
			{
				compactToken->m_pNextCompactToken = (S_CompactToken*)((char*)compactToken + sizeof(S_CompactToken) + ((compactToken->m_uiLexemLength + 1) << charSizeShift));
				compactToken = compactToken->m_pNextCompactToken;

				compactToken->m_uiLexemLength = getstrlen(tokens->m_sLexem);
				compactToken->m_TokenId = tokens->m_TokenId;
				compactToken->m_sLexem = (T_Char*)((char*)compactToken + sizeof(S_CompactToken));
				copystr(compactToken->m_sLexem, (compactToken->m_uiLexemLength + 1) << charSizeShift, tokens->m_sLexem);

				tokens++;
			}
		}
		else
			m_pSymbols = 0;
	}

	template < typename T_Char, typename T_Allocator >
	void C_BaseParserRules<T_Char, T_Allocator>::
		DestroyCompactBuffer() {
		m_Allocator.Free(m_pCompactBuffer);
	}

	template < typename T_Char, typename T_Allocator >
	bool C_BaseParserRules<T_Char, T_Allocator>::
		IsSymbol(const T_Char* strBegin, const T_Char* strEnd, size_t& length, ae::u32& tokenID) const
	{
		return IsToken(false, m_pSymbols, strBegin, strEnd, length, tokenID);
	}

	template < typename T_Char, typename T_Allocator >
	bool C_BaseParserRules<T_Char, T_Allocator>::
		IsKeyWord(const T_Char* strBegin, const T_Char* strEnd, ae::u32& tokenID) const
	{
		size_t length;
		return IsToken(true, m_pKeyWords, strBegin, strEnd, length, tokenID);
	}

	template < typename T_Char, typename T_Allocator >
	bool C_BaseParserRules<T_Char, T_Allocator>::
		IsToken(bool bCompareLength, S_CompactToken* compactToken, const T_Char* strBegin, const T_Char* strEnd, size_t& length, ae::u32& tokenID) const
	{
		while(compactToken)
		{
			const T_Char* lexemBegin(compactToken->m_sLexem);
			const T_Char* lexemEnd(&compactToken->m_sLexem[compactToken->m_uiLexemLength]);
			const T_Char* c0(strBegin);
			const T_Char* c1(lexemBegin);
			ae::u32 matchCount(0);

			if(bCompareLength && ((lexemEnd - lexemBegin) != (strEnd - strBegin)))
			{
				compactToken = compactToken->m_pNextCompactToken;
				continue;
			}

			for(; c0 < strEnd && c1 < lexemEnd; c0++, c1++)
				if(*c0 == *c1) 
					matchCount++;
				else 
					break;

			if(matchCount == compactToken->m_uiLexemLength)
			{
				length = compactToken->m_uiLexemLength;
				tokenID = compactToken->m_TokenId;

				return true;
			}

			compactToken = compactToken->m_pNextCompactToken;
		}

		return false;
	}

// *****************************************************************************************
// *****************************************************************************************

	inline errno_t copybuff(char* dst, size_t size, const char* src) {
		return memcpy_s(dst,size,src,size);
	}

	inline errno_t wcopybuff(wchar_t* dst, size_t size, const wchar_t* src) {
		return wmemcpy_s(dst,size,src,size);
	}

	template < >
	inline C_BaseParserRules<char>::C_BaseParserRules( const S_Token* keyWords, const S_Token* symbols) :
		m_sWhitespaces(0), m_pKeyWords(0), m_pSymbols(0), m_pCompactBuffer(0)
	{
		static const char* sWhitespaces("\r\n \t");
		CreateCompactBuffer(sWhitespaces, keyWords, symbols, &strlen, &copybuff);
	}

	template < >
	inline C_BaseParserRules<char>::C_BaseParserRules( const char* whiteSpaces, const S_Token* keyWords, const S_Token* symbols) :
		m_sWhitespaces(0), m_pKeyWords(0), m_pSymbols(0), m_pCompactBuffer(0)
	{
		CreateCompactBuffer(whiteSpaces, keyWords, symbols, &strlen, &copybuff);
	}

// *****************************************************************************************
// *****************************************************************************************

	template < >
	inline C_BaseParserRules<wchar_t>::C_BaseParserRules( const S_Token* keyWords, const S_Token* symbols) :
		m_sWhitespaces(0), m_pKeyWords(0), m_pSymbols(0), m_pCompactBuffer(0)
	{
		static const wchar_t* ssWhitespaces(L"\r\n \t");
		CreateCompactBuffer(ssWhitespaces, keyWords, symbols, &wcslen, &wcopybuff);
	}

	template < >
	inline C_BaseParserRules<wchar_t>::C_BaseParserRules( const wchar_t* whiteSpaces, const S_Token* keyWords, const S_Token* symbols) :
		m_sWhitespaces(0), m_pKeyWords(0), m_pSymbols(0), m_pCompactBuffer(0)
	{
		CreateCompactBuffer(whiteSpaces, keyWords, symbols, &wcslen, &wcopybuff);
	}

} } // namespace ae { namespace base {

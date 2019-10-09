#pragma once
#ifndef AE_BASE_PARSER_INPUT_STREAM_H
#define AE_BASE_PARSER_INPUT_STREAM_H

namespace ae { namespace base {

	template < typename T_Char >
	class C_ParserInputStream
	{
		T_Char *m_pBeginIterator, *m_pEndIterator;

	public:
		typedef T_Char* T_Iterator;

		C_ParserInputStream( T_Char *begin, T_Char *end );

		T_Iterator Begin() const { return m_pBeginIterator; }
		T_Iterator End() const { return m_pEndIterator; }
	};

} } // namespace ae { namespace base {

#include "parser_input_stream.inl"

#endif // #ifndef AE_BASE_PARSER_INPUT_STREAM_H
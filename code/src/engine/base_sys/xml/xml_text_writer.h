#pragma once

#ifndef AE_BASE_SYS_XML_TEXT_WRITER_H
#define AE_BASE_SYS_XML_TEXT_WRITER_H

#include "xml.h"

namespace ae { namespace base {

	class C_XML_Text_Writer
	{
		void WriteLevel(std::ostream& out, int level);
		void WriteNodeLevel(const S_XMLNode& node, std::ostream& out, int level);
	public:
		C_XML_Text_Writer(const C_XML<>& xml, std::ostream& out);
	};

} } // namespace ae { namespace base {

#include "xml_text_writer.inl"

#endif // #ifdef AE_BASE_SYS_XML_TEXT_WRITER_H

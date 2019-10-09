#pragma once

#ifndef AE_BASE_SYS_XML_H
#define AE_BASE_SYS_XML_H

#include "xml_structs.h"

#include "../memory/allocators/allocator.h"

namespace ae { namespace base {

	class I_InputStream;

	template<typename T_Allocator = C_DefaultAllocator>
	class C_XML
	{
		void PrintLevelSpaces(ae::u32 level);
		void PrintDebug(S_XMLNode*, ae::u32 level);
		NOCOPY_CLASS(C_XML);
	protected:
		T_Allocator& m_Allocator;
		S_XMLNode* m_pRootNode;

		void DestroyNode(S_XMLNode*);
		void DestroyAttribute(S_XMLAttribute*);

		template<typename U_Allocator> friend class C_XML_Text_Reader;
		friend class C_XML_Text_Writer;
	public:
		C_XML(T_Allocator& allocator = theDefaultAllocator);
		~C_XML();

		void PrintDebug();
		void Destroy();
		const S_XMLNode& Root() const;
	};

} } // namespace ae { namespace base {

#include "xml.inl"

#endif // #ifdef AE_BASE_SYS_XML_H

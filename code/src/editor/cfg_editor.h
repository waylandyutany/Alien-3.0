#pragma once

#ifndef AE_EDITOR_CFG_H
#define AE_EDITOR_CFG_H

namespace ae { namespace editor {

	class C_BuildInfo
	{
	public:
		static char* GetName();
		static wchar_t* GetNameW();
	};

} } // namespace ae { namespace editor {

#endif // AE_EDITOR_CFG_H

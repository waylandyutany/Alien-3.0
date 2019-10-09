#include "stdafx.h"

#if defined(AE_EDITOR)

#include "clip_board.h"

namespace ae { namespace editor {

void C_ClipBoard::PushResource(const char* resourcePath)
{
	m_sResource = resourcePath;
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

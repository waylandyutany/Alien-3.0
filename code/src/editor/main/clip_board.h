#pragma once
#if !defined(AE_CLIP_BOARD_H) && defined(AE_EDITOR)
#define AE_CLIP_BOARD_H

namespace ae { namespace editor {

	class C_ClipBoard
	{
		std::string m_sResource;
	public:
		void PushResource(const char* resourcePath);
		const std::string& PushedResource() const { return m_sResource; }
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_CLIP_BOARD_H) && defined(AE_EDITOR)

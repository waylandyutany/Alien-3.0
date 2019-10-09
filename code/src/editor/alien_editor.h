#pragma once
#if !defined(AE_EDITOR_H) && defined(AE_EDITOR)
#define AE_EDITOR_H

#include "main/clip_board.h"
#include "main/gizmo.h"

namespace ae { namespace editor {

	struct S_Init;
	class C_MainWindow;

	class C_AlienEditor
	{
		static C_AlienEditor* m_pInstance;
		C_AlienEditor(const S_Init&);
		C_AlienEditor();
		~C_AlienEditor();
		NOCOPY_CLASS(C_AlienEditor);

		C_MainWindow* m_pMainWindow;
		C_ClipBoard m_ClipBoard;
		C_Gizmo gizmo;
	public:
// *****************************************************************************************************
		static bool Create(const S_Init&);
		static void Release();
		static C_AlienEditor& Instance();
// *****************************************************************************************************
		C_MainWindow& MainWindow() { return *m_pMainWindow; }
		C_ClipBoard& ClipBoard() { return m_ClipBoard; }
		C_Gizmo& Gizmo() { return gizmo; }

		void Update(float);
// *****************************************************************************************************
	};
} } //namespace ae { namespace editor {

#endif // #if !defined(AE_EDITOR_H) && defined(AE_EDITOR)

#include "stdafx.h"

#if defined(AE_EDITOR)

#include "alien_editor.h"
#include "alien_editor_init.h"
#include "main/main_window.h"

namespace ae { namespace editor {

C_AlienEditor* C_AlienEditor::m_pInstance = 0;

C_AlienEditor::C_AlienEditor(const S_Init& sInit) :
	m_pMainWindow(new ae::editor::C_MainWindow(sInit))
{
}

C_AlienEditor::~C_AlienEditor()
{
	delete m_pMainWindow;
}

bool C_AlienEditor::Create(const S_Init& sInit)
{
	AE_ASSERT(!m_pInstance && "C_AlienEditor instance already created !");
	m_pInstance = new C_AlienEditor(sInit);
	return true;
}

void C_AlienEditor::Release()
{
	m_pInstance->MainWindow().SaveConfig();
	AE_ASSERT(m_pInstance && "Uninitialized C_AlienEditor class instance !");
	delete m_pInstance;
}

C_AlienEditor& C_AlienEditor::Instance()
{
	AE_ASSERT(m_pInstance && "Uninitialized C_AlienEditor class instance !");
	return *m_pInstance;
}

void C_AlienEditor::Update( float deltaTime)
{
	gizmo.Update();

	m_pMainWindow->Update(deltaTime);
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

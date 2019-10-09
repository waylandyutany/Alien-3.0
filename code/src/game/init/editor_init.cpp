#include "StdAfx.h"

#if defined(AE_EDITOR)

#include "editor_init.h"
#include "../../editor/main/main_window.h"
#include "../../editor/alien_editor.h"
#include "../../editor/alien_editor_init.h"

namespace ag {

C_AlienEditorInit::C_AlienEditorInit()
{
	ae::editor::S_Init sInit;
	ae::editor::C_AlienEditor::Create(sInit);
	ae::editor::C_AlienEditor::Instance().MainWindow().LoadConfig();

}

C_AlienEditorInit::~C_AlienEditorInit()
{
	ae::editor::C_AlienEditor::Release();
}

void C_AlienEditorInit::Update(float deltaTime )
{
	ae::editor::C_AlienEditor::Instance().Update(deltaTime);
}

} // namespace ag {

#endif // #if defined(AE_EDITOR)


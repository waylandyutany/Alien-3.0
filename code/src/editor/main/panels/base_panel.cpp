#include "stdafx.h"

#if defined(AE_EDITOR)

#include "base_panel.h"
#include "../main_window.h"

#include "../../alien_editor.h"

#include "../../../engine/_sys/templates.h"
#include "../../../engine/debug_sys/output/outputs.h"

namespace ae { namespace editor {

C_BasePanel::C_BasePanel(C_Widget* pParent) : 
	C_Widget(pParent),
	bDirty(false)
{
}

C_BasePanel::~C_BasePanel() {
}

/*virtual*/ void C_BasePanel::OnNew() {
	bDirty = false;
}

/*virtual*/ void C_BasePanel::OnSave() {
	bDirty = false;
	ae::editor::C_AlienEditor::Instance().MainWindow().ResourceSaved();
}

/*virtual*/ void C_BasePanel::OnLoad() {
	bDirty = false;
	ae::editor::C_AlienEditor::Instance().MainWindow().ReloadResource();
}

bool C_BasePanel::IsDirty() const {
	return bDirty;
}

void C_BasePanel::MarkDirty() {
	DBG_OUT << "Resource marked as dirty." << std::endl;
	OnSave();
	bDirty = true;
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

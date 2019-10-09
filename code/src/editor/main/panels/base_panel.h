#pragma once
#if !defined(AE_BASE_PANEL_H) && defined(AE_EDITOR)
#define AE_BASE_PANEL_H

#include "../../windows_sys/widgets/widget.h"
#include "../../windows_sys/widgets/buttons.h"
#include "../../windows_sys/widgets/group_box.h"
#include "../../windows_sys/widgets/static_text.h"
#include "../../windows_sys/widgets/combo_box.h"
#include "../../windows_sys/widgets/edit_box.h"
#include "../../windows_sys/widgets/number_spins.h"
#include "../../windows_sys/widgets/tree_view.h"
#include "../../windows_sys/widgets/message_box.h"

#include "../../windows_sys/layouts/hbox_layout.h"
#include "../../windows_sys/layouts/vbox_layout.h"
#include "../../windows_sys/layouts/grid_layout.h"

#include "../../../engine/base_sys/xml/xml.h"
#include "../../../engine/base_sys/xml/xml_editable.h"

using namespace ae::windows_sys;

namespace ae { namespace editor {

	class C_BasePanel : public C_Widget
	{
		C_BasePanel();
		NOCOPY_CLASS(C_BasePanel);
		bool bDirty;
	public:
		C_BasePanel(C_Widget*);
		~C_BasePanel();

		virtual void LoadCfg(const ae::base::C_XML<>& xml) {};
		virtual void SaveCfg(ae::base::C_XML_Editable<>& xml) {};
		virtual void Activate() {};
		virtual void Deactivate() {};
		virtual void Edit(const char* resourceName, const char* elementName) {};
		virtual void Update(float deltaTime) {}
		// ! ! !Must be called from inherited class after save has been made ! ! !
		virtual void OnSave();
		// ! ! !Must be called from inherited class after load has been made ! ! !
		virtual void OnLoad();

		virtual void OnNew();
		bool IsDirty() const;
		void MarkDirty();
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_BASE_PANEL_H) && defined(AE_EDITOR)

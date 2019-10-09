#pragma once
#if !defined(AE_DUPLICATION_DIALOG_H) && defined(AE_EDITOR)
#define AE_DUPLICATION_DIALOG_H

#include "../../windows_sys/layouts/vbox_layout.h"
#include "../../windows_sys/layouts/hbox_layout.h"
#include "../../windows_sys/widgets/group_box.h"
#include "../../windows_sys/widgets/wdialog.h"
#include "../../windows_sys/widgets/static_text.h"
#include "../../windows_sys/widgets/number_spins.h"
#include "../../windows_sys/widgets/buttons.h"

#include "../properties/prop_panel_transformation.h"

using namespace ae::windows_sys;

namespace ae { namespace editor {

	class C_DuplicationsDialog;
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_ConfigBase
	{
		bool isLoaded;
	public:
		C_ConfigBase() : isLoaded(false) {}
		virtual void LoadCfg(const ae::base::C_XML<>& xml, const ae::base::S_XMLNode&);
		virtual void SaveCfg(ae::base::C_XML_Editable<>& xml, ae::base::S_XMLNode&);
		bool IsLoaded() const { return isLoaded; }
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_DuplicationDialogConfig : public C_ConfigBase
	{
		ae::math::float3 pos, rot, scale;
		ae::u32 numDuplications;
		ae::windows_sys::S_Rect windowRect;

		friend class C_DuplicationsDialog;
	public:
		virtual void LoadCfg(const ae::base::C_XML<>& xml, const ae::base::S_XMLNode&);
		virtual void SaveCfg(ae::base::C_XML_Editable<>& xml, ae::base::S_XMLNode&);
	};
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_DuplicationsDialog : public C_WDialog
	{
		C_DuplicationsDialog();
		NOCOPY_CLASS(C_DuplicationsDialog);

		ae::windows_sys::C_GroupBox groupBox;
		ae::windows_sys::C_VBoxLayout groupBoxLayout, vLayout;
		ae::windows_sys::C_HBoxLayout duplicationsLayout, buttonsLayout;

		ae::windows_sys::C_IntegerSpin numDuplications;
		ae::windows_sys::C_StaticText textNumDuplications;
		ae::windows_sys::C_PushButton buttonOK, buttonCancel, buttonDefault;

		C_PropPanelTransformation propPanelTransformation;
		virtual void OnCommand(T_ControlID);

	public:
		C_DuplicationsDialog(C_Widget*);
		~C_DuplicationsDialog();
		void GetTransformation(ae::math::float3& pos, ae::math::float3& rot, ae::math::float3& scale) const; 
		ae::u32 GetNumDuplications() const;

		void GetConfig(C_DuplicationDialogConfig&) const;
		void SetConfig(const C_DuplicationDialogConfig&);
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_DUPLICATION_DIALOG_H) && defined(AE_EDITOR)

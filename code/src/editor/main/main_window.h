#pragma once
#if !defined(AE_MAIN_WINDOW_H) && defined(AE_EDITOR)
#define AE_MAIN_WINDOW_H

#include "../windows_sys/widgets/main_window.h"
#include "../windows_sys/base/menu.h"
#include "../windows_sys/layouts/hbox_layout.h"

#include "../../engine/resource_sys/resource_types.h"

using namespace ae::windows_sys;

namespace ae { namespace editor {

	struct S_Init;
	class C_AlienEditor;
	class C_BasePanel;

	class C_MainWindow : public ae::windows_sys::C_MainWindow
	{
		NOCOPY_CLASS(C_MainWindow);
// *****************************************************************************************************
		ae::windows_sys::C_Menu m_Menu;
		C_HBoxLayout* m_pHBoxLayout;
		// configuration save may come from main editor close or from calling ae::input::C_InputSys::Inst().Quit();
		// this ensure that configuration is saved only once and valid
		bool bConfigSaved;
// *****************************************************************************************************
		struct S_Panel
		{
			ae::resource::T_ResourceType resourceType;
			C_BasePanel* pPanel;
			ae::windows_sys::T_ControlID controlID;

			S_Panel(ae::resource::T_ResourceType _resourceType, C_BasePanel* _pPanel, ae::windows_sys::T_ControlID _controlID) :
				resourceType(_resourceType), pPanel(_pPanel), controlID(_controlID) {}
		};
		typedef std::deque<S_Panel> T_Panels;
		T_Panels m_Panels;
		C_BasePanel* m_pActivePanel;
// *****************************************************************************************************
		std::string editingPackage; // name of the package just being edited
// *****************************************************************************************************
		C_MainWindow();
		~C_MainWindow();
		C_MainWindow(const S_Init&);
		friend class C_AlienEditor;
// *****************************************************************************************************
		virtual void OnDestroy();
		virtual void OnClose();
		virtual void OnCommand(ae::windows_sys::T_ControlID);
// *****************************************************************************************************
		void ActivatePanel(C_BasePanel*);
	public:
// *****************************************************************************************************
		void LoadConfig();
		void SaveConfig();
// *****************************************************************************************************
		bool IsResourceEditable(ae::resource::T_ResourceType);
		void EditResource(const char* resourcePath, ae::resource::T_ResourceType resourceType, const char* elementName = 0);
		void EditingPackage(const char* packageName); // is called when package is switched and is being edited
		void ResourceSaved(); // is called when resource has been saved, it will cause to pack current package and reload it to engine
		void ReloadResource();
		void Update(float);
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_MAIN_WINDOW_H) && defined(AE_EDITOR)

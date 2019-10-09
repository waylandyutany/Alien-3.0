#include "stdafx.h"

#if defined(AE_EDITOR)

#include "main_window.h"

#include "panels/resource_panel/resource_panel.h"
#include "panels/scene_panel/scene_panel.h"
#include "panels/prefabs_panel/prefabs_panel.h"
#include "panels/profile_panel/profile_panel.h"

#include "../windows_sys/base/menu.h"

#include "../../engine/input_sys/input_sys.h"
#include "../../engine/debug_sys/debug_sys.h"

#include "../../engine/base_sys/streams/i_input_stream.h"
#include "../../engine/base_sys/file_system/file_system.h"

#include "../../engine/base_sys/xml/xml_editable.h"
#include "../../engine/base_sys/xml/xml.h"
#include "../../engine/base_sys/xml/xml_text_writer.h"
#include "../../engine/base_sys/xml/xml_text_reader.h"

namespace ae { namespace editor {
C_MainWindow::C_MainWindow(const S_Init&) :
	bConfigSaved(false),
	m_pActivePanel(0)
{
	SetText(ae::editor::C_BuildInfo::GetName());
	ShowWindow(true);
// *****************************************************************************************************
	ae::windows_sys::C_Menu& pannelsMenu(m_Menu.AppendSubMenu("Panels"));
	pannelsMenu.AppendMenu("Resources", 100);
	pannelsMenu.AppendMenu("Profile", 101);
//	pannelsMenu.AppendMenu("Scenes", 200);
//	pannelsMenu.AppendMenu("Scene instancies", 300);
	SetMenu(m_Menu);
// *****************************************************************************************************
	m_Panels.push_back(S_Panel(0, new C_ResourcePanel(this), 100));
	m_Panels.push_back(S_Panel(0, new C_ProfilePanel(this), 101));
	m_Panels.push_back(S_Panel(ae::resource::resourceTypeScene, new C_ScenePanel(this), 200));
	m_Panels.push_back(S_Panel(ae::resource::resourceTypePrefabs, new C_ScenePrefabsPanel(this), 300));
// *****************************************************************************************************
	m_pHBoxLayout = new C_HBoxLayout(20);
	OnCommand(100);
}

C_MainWindow::~C_MainWindow()
{
	for(size_t i = 0; i < m_Panels.size(); i++)
		delete m_Panels[i].pPanel;
	delete m_pHBoxLayout;
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_MainWindow::LoadConfig()
{
	ae::base::I_InputStream* pInputStream(ae::base::C_FileSys::Inst().CreateInputStream("/edit/config/editor.xml"));
	if(pInputStream)
	{
		ae::base::C_XML<> xml;
		ae::base::C_XML_Text_Reader<> xmlTextReader(*pInputStream, xml);

		const ae::base::S_XMLNode& windowNode = xml.Root().Node("Window");
		S_Rect windowRect(windowNode.Attribute("Width").AsInt(300),windowNode.Attribute("Height").AsInt(400));
		windowRect.Move(windowNode.Attribute("X").AsInt(0), windowNode.Attribute("Y").AsInt(0));
		SetRect(windowRect);

		for(size_t i = 0; i < m_Panels.size(); i++)
			m_Panels[i].pPanel->LoadCfg(xml);

		delete pInputStream;
	}
}

void C_MainWindow::SaveConfig()
{
	if(bConfigSaved) return;
	bConfigSaved = true;

	ae::base::C_XML_Editable<> xml;

	S_Rect windowRect;
	GetRect(windowRect);

	ae::base::S_XMLNode& windowNode = xml.AddNode(xml.Root(), "Window");
	xml.SetAttributeInt(windowNode,"X",windowRect.left);
	xml.SetAttributeInt(windowNode,"Y",windowRect.top);
	xml.SetAttributeInt(windowNode,"Width",windowRect.Width());
	xml.SetAttributeInt(windowNode,"Height",windowRect.Height());

		for(size_t i = 0; i < m_Panels.size(); i++)
			m_Panels[i].pPanel->SaveCfg(xml);

	std::ostream* pOstream(ae::base::C_FileSys::Inst().CreateOutputStream("/edit/config/editor.xml", true));
	ae::base::C_XML_Text_Writer xml_save_writer(xml, *pOstream);
	delete pOstream;
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_MainWindow::OnClose()
{
	SaveConfig();
	ae::input::C_InputSys::Inst().Quit();
}


void C_MainWindow::OnDestroy()
{
	RemoveMenu();
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_MainWindow::OnCommand(ae::windows_sys::T_ControlID commandID)
{
	if(100 <= commandID && commandID <= 1000)
	{
		for(size_t i = 0; i < m_Panels.size(); i++)
			if(m_Panels[i].controlID == commandID)	{
				ActivatePanel(m_Panels[i].pPanel);
				return;
			}
	}
}

void C_MainWindow::ActivatePanel(C_BasePanel* pPanelToActivate)
{
	if(pPanelToActivate != m_pActivePanel)
	{
		if(m_pActivePanel)
		{
			m_pHBoxLayout->RemoveLayoutItem(m_pActivePanel);
			m_pActivePanel->ShowWindow(false);
			m_pActivePanel->Deactivate();
		}

		m_pActivePanel = pPanelToActivate;

		m_pHBoxLayout->AddLayoutItem(m_pActivePanel,300, 200, 700);
		m_pActivePanel->ShowWindow(true);
		m_pActivePanel->Activate();
		SetLayout(m_pHBoxLayout);
	}
}

void C_MainWindow::Update(float deltaTime)
{
	if(m_pActivePanel)
		m_pActivePanel->Update(deltaTime);
}

// *****************************************************************************************************
// *****************************************************************************************************
bool C_MainWindow::IsResourceEditable(ae::resource::T_ResourceType resourceType)
{
	for(size_t i = 0; i < m_Panels.size(); i++)
		if(m_Panels[i].resourceType == resourceType)
			return true;
	return false;
}

void C_MainWindow::EditResource(const char* resourcePath, ae::resource::T_ResourceType resourceType, const char* elementName)
{
	for(size_t i = 0; i < m_Panels.size(); i++)
		if(m_Panels[i].resourceType == resourceType)
		{
			ActivatePanel(m_Panels[i].pPanel);
			m_Panels[i].pPanel->Edit(resourcePath, elementName);
			return;
		}
}

void C_MainWindow::EditingPackage(const char* packageName)
{
	editingPackage = packageName;
}

void C_MainWindow::ResourceSaved()
{
	if(!editingPackage.empty())
	{
		C_ResourceSys::Inst().Packages().PackPackageResource(editingPackage.c_str());

		if(C_ResourceSys::Inst().Resources().IsPackageLoaded(editingPackage.c_str()))
		{
			C_ResourceSys::Inst().Resources().UnloadPackage(editingPackage.c_str());
			C_ResourceSys::Inst().Resources().LoadPackage(editingPackage.c_str());
		}
	}
}

void C_MainWindow::ReloadResource()
{
	if(!editingPackage.empty())
	{
		if(C_ResourceSys::Inst().Resources().IsPackageLoaded(editingPackage.c_str()))
		{
			C_ResourceSys::Inst().Resources().UnloadPackage(editingPackage.c_str());
			C_ResourceSys::Inst().Resources().LoadPackage(editingPackage.c_str());
		}
	}
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

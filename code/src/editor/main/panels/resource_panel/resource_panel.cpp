#include "stdafx.h"

#if defined(AE_EDITOR)

#include "resource_panel.h"

#include "../../../alien_editor.h"
#include "../../main_window.h"

#include "../../../windows_sys/widgets/message_box.h"
#include "../../../windows_sys/base/menu.h"

#include "../../../../engine/resource_sys/imports/imports.h"
#include "../../../../engine/base_sys/xml/xml_text_writer.h"

namespace ae { namespace editor {

C_ResourcePanel::S_Node* C_ResourcePanel::S_Node::SrcExist(const char* name, ae::resource::T_ResourceType rscType)
{
	std::string sRsrcType = ae::resource::ResourceType2String(rscType);

	for(size_t i=0; i < childs.size(); i++)
	{
		std::string attrName(childs[i]->node.Attribute("name").AsString());
		std::string attrType(childs[i]->node.Attribute("type").AsString());
		if(childs[i]->node.Is("src") &&  attrName == name && attrType == sRsrcType)
			return childs[i];
	}

	return 0;
}

C_ResourcePanel::S_Node* C_ResourcePanel::S_Node::DirExist(const char* name)
{
	for(size_t i=0; i < childs.size(); i++)
		if(childs[i]->node.Is("dir") && childs[i]->node.Attribute("name").AsString() == name)
			return childs[i];
	return 0;
}
// *****************************************************************************************************
// *****************************************************************************************************
C_ResourcePanel::C_ResourcePanel(C_Widget* pParent) : 
	C_BasePanel(pParent),
	m_CBPackageCallback(*this),
	m_TVResourcesCallback(*this),
	m_VBoxLayout(300),
	m_HBoxLayout(20),
	m_GBPackage(this, " Package "),
	m_CBPackage(&m_CBPackageCallback, windowStyle_LowerCase, this, (ae::windows_sys::T_ControlID)200),
	m_TVResources(&m_TVResourcesCallback, this, (ae::windows_sys::T_ControlID)300),
	m_BTNewPackage(this, "New", (ae::windows_sys::T_ControlID)100),
	m_BTSavePackage(this, "Save", (ae::windows_sys::T_ControlID)101),
	m_BTLoadPackage(this, "Load", (ae::windows_sys::T_ControlID)102),
	m_PackagesManager(ae::resource::C_ResourceSys::Inst().Packages()),
	m_pNode(0)
{
	m_HBoxLayout.SetMarging(0,0);
	m_HBoxLayout.AddLayoutItem(&m_CBPackage,100,400);
	m_HBoxLayout.AddLayoutItem(&m_BTLoadPackage,30);
	m_HBoxLayout.AddLayoutItem(&m_BTSavePackage,30);
	m_HBoxLayout.AddLayoutItem(&m_BTNewPackage,30);
	m_GBPackage.SetLayout(&m_HBoxLayout);

	m_VBoxLayout.AddLayoutItem(&m_GBPackage, 48, 200, 0);
	m_VBoxLayout.AddLayoutItem(&m_TVResources, 200, 200, 600);
	SetLayout(&m_VBoxLayout);

	ReloadPackages();
}

C_ResourcePanel::~C_ResourcePanel()
{
	delete m_pNode;
}

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_ResourcePanel::LoadCfg(const ae::base::C_XML<>& xml)
{
	LoadPackage(xml.Root().Node("Resource_Panel").Attribute("PackageSource").AsString(""));
}

/*virtual*/ void C_ResourcePanel::SaveCfg(ae::base::C_XML_Editable<>& xml)
{
	ae::base::S_XMLNode& resourcePanelNode = xml.AddNode(xml.Root(), "Resource_Panel");
	xml.SetAttributeString(resourcePanelNode,"PackageSource",m_sPackageName.c_str());
}

/*virtual*/ void C_ResourcePanel::OnCommand(T_ControlID commandID)
{
	if(m_BTNewPackage.GetControlID() == commandID) NewPackage();
	else if(m_BTSavePackage.GetControlID() == commandID) OnSave();
	else if(m_BTLoadPackage.GetControlID() == commandID) OnLoad();
}

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_CBPackageCallback::OnStringSelected(ae::windows_sys::T_ControlID controID, const char* string)
{
	m_Parent.LoadPackage(string);
}

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_TVResourcesCallback::OnMouseLDBClick(T_ControlID, const S_ItemInfo& itemInfo)
{
	C_ResourcePanel::S_Node* pNode((C_ResourcePanel::S_Node*)itemInfo.param);
	switch(pNode->NodeType())
	{
		case C_ResourcePanel::E_NT_SRC :
		{
			ae::resource::T_ResourceType resourceType = ae::resource::String2ResourceType(pNode->node.Attribute("type").AsString());
			if(C_AlienEditor::Instance().MainWindow().IsResourceEditable(resourceType))
				C_AlienEditor::Instance().MainWindow().EditResource(pNode->node.Node("path").Attribute("path").AsString(), resourceType);
			break;
		}
		case C_ResourcePanel::E_NT_ELEMENT : {
			ae::resource::T_ResourceType resourceType = ae::resource::String2ResourceType(pNode->pParent->node.Attribute("type").AsString());
			if(C_AlienEditor::Instance().MainWindow().IsResourceEditable(resourceType))
				C_AlienEditor::Instance().MainWindow().EditResource(pNode->pParent->node.Node("path").Attribute("path").AsString(), resourceType, itemInfo.string);
			break;
		}
	}
}

// *****************************************************************************************************
void C_TVResourcesCallback::OnMouseRClick(T_ControlID, const S_ItemInfo& itemInfo)
{
	C_Menu menu;
	C_Menu& subMenu = menu.AppendSubMenu("");
	C_ResourcePanel::S_Node* pNode((C_ResourcePanel::S_Node*)itemInfo.param);

	switch(pNode->NodeType()) {
// *****************************************************************************************************
	case C_ResourcePanel::E_NT_ROOT : {
			subMenu.AppendMenu("Delete package\tDel", 1);
			subMenu.AppendMenu("Rename package", 2);
			subMenu.AppendSeparator();
			subMenu.AppendMenu("New subdir", 3);

			// Add new resources submenu
			subMenu.AppendSeparator();
			C_Menu& addNewSubmenu(subMenu.AppendSubMenu("Add new"));
			addNewSubmenu.AppendMenu("Scene", 400);
			addNewSubmenu.AppendMenu("Scene prefabs", 401);

			switch(subMenu.TrackPopupMenu(m_Parent)) {
				case 1 : m_Parent.DeletePackage(itemInfo.string); break;
				case 2 : m_Parent.m_TVResources.BeginEditItem(itemInfo.item); break;
				case 3 : m_Parent.AddSubDir(*pNode, "Subdir"); break;
				case 400:m_Parent.AddNewScene(pNode); break;
				case 401:m_Parent.AddNewScenePrefabs(pNode); break;
			}
			break;
		}
// *****************************************************************************************************
	case C_ResourcePanel::E_NT_DIR : {
			subMenu.AppendMenu("New subdir", 1);
			subMenu.AppendMenu("Delete dir\tDel", 2);
			subMenu.AppendMenu("Rename dir", 3);

			// Add new resources submenu
			subMenu.AppendSeparator();
			C_Menu& addNewSubmenu(subMenu.AppendSubMenu("Add new"));
			addNewSubmenu.AppendMenu("Scene", 400);
			addNewSubmenu.AppendMenu("Scene prefabs", 401);

			switch(subMenu.TrackPopupMenu(m_Parent)) {
				case 1 : m_Parent.AddSubDir(*pNode, "subdir"); break;
				case 2 : m_Parent.RemoveSubDir(pNode); break;
				case 3 : m_Parent.m_TVResources.BeginEditItem(itemInfo.item); break;
				case 400:m_Parent.AddNewScene(pNode); break;
				case 401:m_Parent.AddNewScenePrefabs(pNode); break;
			}
			break;
		}
		break;
// *****************************************************************************************************
	case C_ResourcePanel::E_NT_SRC :
		{
			subMenu.AppendMenu("Delete resource\tDel", 1);
			subMenu.AppendMenu("Rename resource", 2);
			subMenu.AppendMenu("Insert to clipboard", 3);
			subMenu.AppendMenu("Reload elements", 4);

			ae::resource::T_ResourceType resourceType = ae::resource::String2ResourceType(pNode->node.Attribute("type").AsString());
			if(C_AlienEditor::Instance().MainWindow().IsResourceEditable(resourceType))	{
				subMenu.AppendSeparator();
				subMenu.AppendMenu("Edit\tLDblClk", 5);
			}

			// *****************************************************************************************************
			// It's ugly we need better system for handling action for each property individually
			if(resourceType == ae::resource::resourceTypeCollision)
			{
				if(!pNode->node.Node("prop").Attribute("AsConvex").AsUInt(0))
				{
					subMenu.AppendSeparator();
					subMenu.AppendMenu("Import as convex", 6);
				} else	{
					subMenu.AppendSeparator();
					subMenu.AppendMenu("Import as mesh", 6);
				}
			}
			// *****************************************************************************************************

			switch(subMenu.TrackPopupMenu(m_Parent)) {
				case 1 : m_Parent.RemoveSrc(pNode); break;
				case 2 : m_Parent.m_TVResources.BeginEditItem(itemInfo.item); break;
				case 3 : C_AlienEditor::Instance().ClipBoard().PushResource(m_Parent.GetNodePath(*pNode).c_str()); break;
				case 4 : m_Parent.LoadElements(pNode); break;
				case 5 : C_AlienEditor::Instance().MainWindow().EditResource(pNode->node.Node("path").Attribute("path").AsString(), resourceType);break;

				// *****************************************************************************************************
				// It's ugly we need better system for handling action for each property individually
				case 6 : {
					if(!pNode->node.Node("prop").IsValid())
						m_Parent.m_PackageSrc.AddNode(pNode->node,"prop");

					if(!pNode->node.Node("prop").Attribute("AsConvex").AsUInt(0))
						m_Parent.m_PackageSrc.SetAttributeUInt(pNode->node.Node("prop"), "AsConvex", 1);
					else
						m_Parent.m_PackageSrc.SetAttributeUInt(pNode->node.Node("prop"), "AsConvex", 0);
				}
				 // *****************************************************************************************************
			}
			break;
		}
// *****************************************************************************************************
	case C_ResourcePanel::E_NT_ELEMENT :
		{
			subMenu.AppendMenu("Insert to clipboard", 1);

			ae::resource::T_ResourceType resourceType = ae::resource::String2ResourceType(pNode->pParent->node.Attribute("type").AsString());
			if(C_AlienEditor::Instance().MainWindow().IsResourceEditable(resourceType)) {
				subMenu.AppendSeparator();
				subMenu.AppendMenu("Edit\tLDblClk", 2);
			}

			switch(subMenu.TrackPopupMenu(m_Parent)) {
				case 1 : C_AlienEditor::Instance().ClipBoard().PushResource(m_Parent.GetNodePath(*pNode).c_str()); break;
				case 2 : C_AlienEditor::Instance().MainWindow().EditResource(pNode->pParent->node.Node("path").Attribute("path").AsString(), resourceType, itemInfo.string);break;
			}

			break;
		}
	}
}

/*virtual*/ bool C_TVResourcesCallback::OnItemRenamed(T_ControlID, const char* oldString, const char* newString, void* param, T_TreeItem)
{
	C_ResourcePanel::S_Node* pNode((C_ResourcePanel::S_Node*)param);
	if(pNode->NodeType() == C_ResourcePanel::E_NT_ROOT)
	{
		if(m_Parent.m_PackagesManager.IsPackageSource(newString))
			if(C_MessageBox::Show(&m_Parent, mb_Style_YesNo | mb_Style_IconWarning, "Rename package source", "Overwrite ?") == mb_ReturnCode_No)
				return false;
		m_Parent.m_PackagesManager.DeletePackageSource(oldString);
		m_Parent.m_PackagesManager.SavePackageSource(newString, m_Parent.m_PackageSrc);
		m_Parent.ReloadPackages();
		m_Parent.LoadPackage(newString);
	}
	else if(pNode->NodeType() == C_ResourcePanel::E_NT_DIR)
	{
		if(pNode->pParent->DirExist(newString)) // prevent to edit same dir name
			return false;

		m_Parent.m_PackageSrc.SetAttributeString(pNode->node,"name", newString);
	}
	else if(pNode->NodeType() == C_ResourcePanel::E_NT_SRC)
	{
		m_Parent.m_PackageSrc.SetAttributeString(pNode->node,"name", newString);
	}

	return true;
}

// *****************************************************************************************************

/*virtual*/ void C_TVResourcesCallback::OnFilesDropped(T_ControlID, const S_ItemInfo& itemInfo, const T_DroppedFiles& droppedFiles)
{
	C_ResourcePanel::S_Node* pNode((C_ResourcePanel::S_Node*)itemInfo.param);
	if(pNode->NodeType() != C_ResourcePanel::E_NT_SRC)
	{
		for(size_t i = 0; i < droppedFiles.size(); i++)
			m_Parent.AddSrc(*pNode, droppedFiles[i].c_str());
	}
}

/*virtual*/ void C_TVResourcesCallback::OnItemSelected(T_ControlID, const S_ItemInfo& itemInfo)
{
	m_Parent.m_TVResources.SortChildItems(itemInfo.item);
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_ResourcePanel::NewPackage()
{
	std::string packageName(m_CBPackage.GetText());
	if(m_PackagesManager.IsPackageSource(packageName.c_str()))
		if(C_MessageBox::Show(this, mb_Style_YesNo | mb_Style_IconWarning, "New package source", "Overwrite ?") == mb_ReturnCode_No)
			return;

	m_PackageSrc.Destroy();
	m_PackagesManager.SavePackageSource(packageName.c_str(), m_PackageSrc);
	ReloadPackages();
	LoadPackage(packageName.c_str());
}

/*virtual*/ void C_ResourcePanel::OnSave()
{
	m_PackagesManager.SavePackageSource(m_sPackageName.c_str(), m_PackageSrc);

	C_BasePanel::OnSave();
}

/*virtual*/ void C_ResourcePanel::OnLoad()
{
	LoadPackage(m_sPackageName.c_str());

	C_BasePanel::OnLoad();
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_ResourcePanel::GetNodePath(S_Node& node, std::string& path) const
{
	if(node.pParent) GetNodePath(*node.pParent, path);
	else
	{
		path += node.node.Attribute("name").AsString();
		path += "/";
		return;
	}
	if(node.nodeType == E_NT_DIR)
	{
		path += node.node.Attribute("name").AsString();
		path += "/";
	}
	else if(node.nodeType == E_NT_SRC)
	{
		path += node.node.Attribute("name").AsString();
		path += ".";
		path += node.node.Attribute("type").AsString();
	}
	else if(node.nodeType == E_NT_ELEMENT)
	{
		std::string str;
		m_TVResources.GetItemInfo(node.treeItem, str, 0);
		path += "/";
		path += str;
	}
}

std::string C_ResourcePanel::GetNodePath(S_Node& node) const
{
	std::string path;
	GetNodePath(node, path);
	return m_sPackageName + path;
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_ResourcePanel::RemoveSubDir(S_Node* node)
{
	m_TVResources.RemoveItem(node->treeItem);
	m_PackageSrc.DelNode(node->pParent->node, &node->node);
	delete node;
}

C_ResourcePanel::S_Node* C_ResourcePanel::AddSubDir(C_ResourcePanel::S_Node& node, const char* dirName)
{
	C_ResourcePanel::S_Node* pNewNode = new C_ResourcePanel::S_Node(&node, E_NT_DIR, m_PackageSrc.AddNode(node.node, "dir"));
	m_PackageSrc.SetAttributeString(pNewNode->node, "name", dirName);
	pNewNode->treeItem = m_TVResources.InsertItem(dirName, pNewNode, node.treeItem, insertSorted);
	m_TVResources.BeginEditItem(pNewNode->treeItem);
	return pNewNode;
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_ResourcePanel::RemoveSrc(S_Node* node)
{
	m_TVResources.RemoveItem(node->treeItem);
	m_PackageSrc.DelNode(node->pParent->node, &node->node);
	delete node;
}

void C_ResourcePanel::AddSrc(S_Node& node, const char* srcFile)
{
	// accept files from edit/ mount point only !
	std::string spath(srcFile);
	if(!ae::base::C_FileSys::Inst().MakePathRelativeTo("/edit", spath))
		return;

	std::string resourceName(ae::base::GetFileName(spath.c_str(), false));

	// find registered importer/s
	ae::resource::C_Imports::T_Importers importers;
	std::string ext(ae::base::GetFileExtension(spath.c_str()));
	ae::resource::C_ResourceSys::Inst().Imports().FindImporters(ext.c_str(), importers);

	// In case that file can belongs to more resource types, add each of them as separate resource
	for(size_t i = 0; i < importers.size(); i++)
	{
		// check if we do not have same files
		C_ResourcePanel::S_Node* pNewNode = node.SrcExist(resourceName.c_str(), importers[i].rscType);

		if(!pNewNode) { // new resource
			pNewNode = new C_ResourcePanel::S_Node(&node, E_NT_SRC, m_PackageSrc.AddNode(node.node, "src"));
			m_PackageSrc.SetAttributeString(pNewNode->node, "name", resourceName.c_str());
			m_PackageSrc.SetAttributeString(pNewNode->node, "type", ae::resource::ResourceType2String(importers[i].rscType).c_str());
			pNewNode->treeItem = m_TVResources.InsertItem(ae::base::GetFileName(srcFile, false).c_str(), pNewNode, node.treeItem, insertSorted);
			m_PackageSrc.AddNode(pNewNode->node,"path");
		}

		m_PackageSrc.SetAttributeString(pNewNode->node.Node("path"), "path", spath.c_str());

		LoadElements(pNewNode);
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

class C_PackageEnumerator : public ae::resource::C_PackagesManager::C_PackageSourcesEnumerator
{
	C_ResourcePanel& m_ResourcePanel;
public:
	C_PackageEnumerator(C_ResourcePanel& resourcePanel) : m_ResourcePanel(resourcePanel) {
		m_ResourcePanel.m_CBPackage.Reset();
	}

	virtual void OnPackage(const char* packageSrcName) {
		m_ResourcePanel.m_CBPackage.AddString(packageSrcName);
	}
};

void C_ResourcePanel::ReloadPackages()
{
	C_PackageEnumerator enumerator(*this);
	m_PackagesManager.EnumeratePackageSources(enumerator);
}

// *****************************************************************************************************
// *****************************************************************************************************

class C_ElementsCollector : public ae::resource::I_ResourceElementsEnumerator
{
public:
	typedef std::deque<std::string> T_Elements;
	T_Elements m_Elements;

	virtual void OnElement(const char* elementName, ae::resource::T_ResourceType elementType)
	{
		std::string elementNameWithType(elementName);
		elementNameWithType += ".";
		elementNameWithType += ae::resource::ResourceType2String(elementType);
		m_Elements.push_back(elementNameWithType);
	}
};

void C_ResourcePanel::LoadElements(S_Node* node)
{
	ae::resource::T_ResourceType resourceType(ae::resource::String2ResourceType(node->node.Attribute("type").AsString()));
	if(!
		(resourceType == ae::resource::resourceTypeMesh ||
		resourceType == ae::resource::resourceTypeCollision ||
		resourceType == ae::resource::resourceTypePrefabs  ||
		resourceType == ae::resource::resourceTypeScene)
		) return;

	std::string resourceFile = ae::resource::C_ResourceSys::Inst().Packages().SaveTmpResource(m_sPackageName.c_str(), m_PackageSrc, node->node);
// *****************************************************************************************************
	C_ElementsCollector collector;
	ae::resource::C_ResourcesManager& resources(ae::resource::C_ResourceSys::Inst().Resources());

	ae::resource::S_Resource* pResource = resources.LoadResource(resourceFile.c_str());
	resources.EnumerateResourceElements(pResource, collector);
	resources.UnloadResource(pResource);
// *****************************************************************************************************
	// delete all old elements
	for(size_t i = 0; i < node->childs.size(); i++)
		m_TVResources.RemoveItem(node->childs[i]->treeItem);
	node->DestroyChilds();
// *****************************************************************************************************
	// insert elements to tree and to our S_Node
	auto it(collector.m_Elements.begin());
	for(; it != collector.m_Elements.end(); ++it)
	{
		S_Node* pNode = new S_Node(node, E_NT_ELEMENT, ae::base::g_DummyXMLNode);
		pNode->treeItem = m_TVResources.InsertItem((*it).c_str(), pNode, node->treeItem, insertSorted);
	}
}

void C_ResourcePanel::LoadNode(S_Node* node)
{
	ae::base::S_XMLNode* pXMLNode = node->node.child;
	while(pXMLNode)
	{
		if(pXMLNode->Is("dir"))
		{
			S_Node* pNode = new S_Node(node, E_NT_DIR, *pXMLNode);
			pNode->treeItem = m_TVResources.InsertItem(pXMLNode->Attribute("name").AsString(), pNode, node->treeItem, insertSorted);
			LoadNode(pNode);
		}
		else if(pXMLNode->Is("src"))
		{
			S_Node* pNode = new S_Node(node, E_NT_SRC, *pXMLNode);
			pNode->treeItem = m_TVResources.InsertItem(pXMLNode->Attribute("name").AsString(), pNode, node->treeItem, insertSorted);

			LoadElements(pNode);
		}
		pXMLNode = pXMLNode->next;
	}
}

void C_ResourcePanel::LoadPackage(const char* name)
{
	ae::editor::C_AlienEditor::Instance().MainWindow().EditingPackage(name);
	m_sPackageName = name;
	m_PackagesManager.LoadPackageSource(name, m_PackageSrc);
	if(m_pNode)	{
		m_TVResources.RemoveItem(m_pNode->treeItem);
		delete m_pNode;
		m_pNode = 0;
	}

	m_CBPackage.SetText(name);
	m_TVResources.RemoveAllInfoTips();
	m_pNode = new S_Node(0, E_NT_ROOT, m_PackageSrc.Root());
	m_pNode->treeItem = m_TVResources.InsertItem(name, m_pNode, 0);

	LoadNode(m_pNode);
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_ResourcePanel::DeletePackage(const char* name)
{
	if(C_MessageBox::Show(this, mb_Style_YesNo | mb_Style_IconWarning, "Delete package source", "Delete ?") == mb_ReturnCode_No)
		return;

	if(m_pNode)	{
		m_TVResources.RemoveItem(m_pNode->treeItem);
		delete m_pNode;
		m_pNode = 0;
	}
	m_PackagesManager.DeletePackageSource(name);
	ReloadPackages();
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_ResourcePanel::AddNewScene(S_Node* node)
{
	std::string sceneFile("/edit/scenes/");
	sceneFile += m_sPackageName;
	sceneFile += ".scene";

	ae::base::C_XML_Editable<> scene;
	scene.Destroy();

	std::ostream* pOstream(ae::base::C_FileSys::Inst().CreateOutputStream(sceneFile.c_str(), true));
	ae::base::C_XML_Text_Writer xml_save_writer(scene, *pOstream);
	delete pOstream;

	S_Node* pNewNode = new C_ResourcePanel::S_Node(node, E_NT_SRC, m_PackageSrc.AddNode(node->node, "src"));
	m_PackageSrc.SetAttributeString(pNewNode->node, "name", m_sPackageName.c_str());
	m_PackageSrc.SetAttributeString(pNewNode->node, "type", ae::resource::ResourceType2String(ae::resource::resourceTypeScene).c_str());
	pNewNode->treeItem = m_TVResources.InsertItem(ae::base::GetFileName(sceneFile.c_str(), false).c_str(), pNewNode, node->treeItem, insertSorted);
	m_PackageSrc.AddNode(pNewNode->node,"path");
	m_PackageSrc.SetAttributeString(pNewNode->node.Node("path"), "path", sceneFile.c_str());
}

void C_ResourcePanel::AddNewScenePrefabs(S_Node* node)
{
	std::string sceneFile("/edit/scenes/");
	sceneFile += m_sPackageName;
	sceneFile += ".prefabs";

	ae::base::C_XML_Editable<> scene;
	scene.Destroy();

	std::ostream* pOstream(ae::base::C_FileSys::Inst().CreateOutputStream(sceneFile.c_str(), true));
	ae::base::C_XML_Text_Writer xml_save_writer(scene, *pOstream);
	delete pOstream;

	S_Node* pNewNode = new C_ResourcePanel::S_Node(node, E_NT_SRC, m_PackageSrc.AddNode(node->node, "src"));
	m_PackageSrc.SetAttributeString(pNewNode->node, "name", m_sPackageName.c_str());
	m_PackageSrc.SetAttributeString(pNewNode->node, "type", ae::resource::ResourceType2String(ae::resource::resourceTypePrefabs).c_str());
	pNewNode->treeItem = m_TVResources.InsertItem(ae::base::GetFileName(sceneFile.c_str(), false).c_str(), pNewNode, node->treeItem, insertSorted);
	m_PackageSrc.AddNode(pNewNode->node,"path");
	m_PackageSrc.SetAttributeString(pNewNode->node.Node("path"), "path", sceneFile.c_str());
}

// *****************************************************************************************************
// *****************************************************************************************************

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

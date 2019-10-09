#include "stdafx.h"

#if defined(AE_EDITOR)

#include "prefabs_panel.h"

#include "../../utils/tree_nodes_deleter.h"
#include "../../utils/node_names.h"

#include "../../../alien_editor.h"
#include "../../../windows_sys/widgets/message_box.h"
#include "../../../windows_sys/base/menu.h"

#include "../../../../engine/resource_sys/imports/imports.h"
#include "../../../../engine/resource_sys/imports/importers/import_utils.h"
#include "../../../../engine/resource_sys/resource_sys.h"

#include "../../../../engine/base_sys/xml/xml_text_writer.h"
#include "../../../../engine/base_sys/xml/xml_text_reader.h"
#include "../../../../engine/base_sys/streams/i_input_stream.h"
#include "../../../../engine/base_sys/file_system/file_system.h"

#include "../../../../engine/scene_sys/scene_sys.h"

namespace ae { namespace editor {

// *****************************************************************************************************
// *****************************************************************************************************
namespace ScenePrefabsPanel {

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_ScenePrefabsTreeSelectedItems : public ae::windows_sys::I_TreeViewSelectionEnumerator
	{
		typedef std::deque<ScenePrefabsPanel::T_Node*> T_ElementNodes;
		T_ElementNodes elementNodes;
		ae::base::C_XML_Editable<>& source;
		ae::math::E_TransformationElement element;
		float elementValue;
		void UpdateInstanceTransformations(ScenePrefabsPanel::T_Node* pElementNode);
	public:
		C_ScenePrefabsTreeSelectedItems(ae::base::C_XML_Editable<>& _source, ae::math::E_TransformationElement _element, float value) : source(_source), element(_element), elementValue(value) { }
		~C_ScenePrefabsTreeSelectedItems();
		virtual void OnSelectedItem(const C_TreeViewCallback::S_ItemInfo& itemInfo);
	};

	C_ScenePrefabsTreeSelectedItems::~C_ScenePrefabsTreeSelectedItems()
	{
		T_ElementNodes::iterator i(elementNodes.begin());
		for(; i != elementNodes.end(); ++i)
			UpdateInstanceTransformations(*i);
	}

	/*virtual*/ void C_ScenePrefabsTreeSelectedItems::OnSelectedItem(const C_TreeViewCallback::S_ItemInfo& itemInfo)
	{
		ScenePrefabsPanel::T_Node* pNode((ScenePrefabsPanel::T_Node*)itemInfo.param);
		if(pNode->type != ScenePrefabsPanel::E_Instance) return;
		if(pNode->pParent && pNode->pParent->type == ScenePrefabsPanel::E_Resource)
		{
			if(std::find(elementNodes.begin(), elementNodes.end(), pNode->pParent) == elementNodes.end())
				elementNodes.push_back(pNode->pParent);

			ae::resource::SetTransformationElementToXML(source,pNode->xmlNode,element, elementValue);
		}
	}

	void C_ScenePrefabsTreeSelectedItems::UpdateInstanceTransformations(ScenePrefabsPanel::T_Node* pElementNode)
	{
		static const ae::u32 maxTransformations = 1024;
		static ae::math::float4x4 pTransformations[maxTransformations];

		if(pElementNode && pElementNode->type == ScenePrefabsPanel::E_Resource)
		{
			if(pElementNode->pParent && pElementNode->pParent->pParent && pElementNode->pParent->pParent->type == ScenePrefabsPanel::E_Prefab)
			{
				ae::u32 numInstancies = pElementNode->NumChilds();
				std::string geometryName(pElementNode->xmlNode.Attribute("name").AsString());
				std::string prefabName(pElementNode->pParent->pParent->xmlNode.Attribute("name").AsString());

				ScenePrefabsPanel::T_Node::T_Nodes::iterator it(pElementNode->childs.begin()); 
				AE_ASSERT(numInstancies <= maxTransformations && "pTransformations has less memory than recquired !");
				for(ae::u32 i = 0; i < numInstancies; i++)
				{
					ae::resource::GetMatrixFromXMLTransformation((*it)->xmlNode, pTransformations[i]);
					++it;
				}

				ae::scene::C_SceneSys::Inst().Scene().UpdateGeometryInstanceTransformations(prefabName.c_str(), geometryName.c_str(), pTransformations, numInstancies);
			}
		}
	}

// *****************************************************************************************************
// *****************************************************************************************************
	class C_DuplicateInstances : public ae::windows_sys::I_TreeViewSelectionEnumerator
	{
		typedef ae::base::C_XML_Editable<> T_XMLSource;
		C_TreeView& treeView;
		T_XMLSource& xmlSource;
		ae::math::float3 delta_pos, delta_rot, delta_scale;
		ae::u32 numDuplicates;
	public:
		C_DuplicateInstances(	C_TreeView& treeView, 
			T_XMLSource& xmlSource, 
			ae::math::float3& delta_pos, 
			ae::math::float3& delta_rot, 
			ae::math::float3& delta_scale, 
			ae::u32 numDuplicates) :
		treeView(treeView), 
			xmlSource(xmlSource),
			delta_pos(delta_pos), 
			delta_rot(delta_rot), 
			delta_scale(delta_scale), 
			numDuplicates(numDuplicates) {

		}

		virtual void OnSelectedItem( const C_TreeViewCallback::S_ItemInfo& itemInfo ) 
		{
			ScenePrefabsPanel::T_Node* pNode((ScenePrefabsPanel::T_Node*)itemInfo.param);
			if(pNode->type == ScenePrefabsPanel::E_Instance)
			{
				ae::math::float3 pos, rot, scale;
				ae::resource::GetPosRotScaleFromXMLTransformation(pNode->xmlNode, pos, rot, scale);

				for(ae::u32 i = 0;i < numDuplicates; i++)
				{
					pos += delta_pos;
					rot += delta_rot;
					scale *= delta_scale;

					ScenePrefabsPanel::T_Node* pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Instance, xmlSource.AddNode(pNode->pParent->xmlNode,"instance"));
					xmlSource.SetAttributeString(pNewNode->xmlNode, "name", GetUniqueName(pNode->pParent->xmlNode, pNode->xmlNode.Attribute("name").AsString(),"_").c_str());
					ae::resource::SetPosRotScaleToXML(xmlSource, pNewNode->xmlNode, pos, rot, scale);
					pNewNode->treeItem = treeView.InsertItem(pNewNode->xmlNode.Attribute("name").AsString(), pNewNode, pNode->pParent->treeItem, insertSorted);
				}
			}
		}
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	/*virtual*/ void C_TransformationCB::OnTransformationChanged(ae::math::E_TransformationElement element, float value)
	{
		C_ScenePrefabsTreeSelectedItems selectedItems(parent.xmlSource, element, value);
		parent.treePrefabs.EnumSelectedItems(selectedItems);
	}
} // namespace ScenePrefabsPanel {

// *****************************************************************************************************
// *****************************************************************************************************
C_ScenePrefabsPanel::C_ScenePrefabsPanel(C_Widget* pParent) : 
	C_BasePanel(pParent),
	transformationCB(*this),
	treePrefabsCallback(*this),
	layoutVBox(300),
	layoutHBox(20),
	gbPrefabs(this, " Prefabs "),
	textPrefab(0, this,1),
	treePrefabs(&treePrefabsCallback, this, (ae::windows_sys::T_ControlID)300),
	btnSavePrefabs(this, "Save", (ae::windows_sys::T_ControlID)101),
	btnLoadPrefabs(this, "Load", (ae::windows_sys::T_ControlID)102),
	properties(this),
	propertyTransformation(this, &transformationCB),
	selectedInstancePos(0,0,0), selectedInstanceRot(0,0,0), selectedInstanceScale(1,1,1),
	pPrefabsNode(0)
{
	textPrefab.EnableWindow(false);

	layoutHBox.AddLayoutItem(&textPrefab,100,400);
	layoutHBox.AddLayoutItem(&btnLoadPrefabs,30);
	layoutHBox.AddLayoutItem(&btnSavePrefabs,30);

	gbPrefabs.SetLayout(&layoutHBox);

	layoutVBox.AddLayoutItem(&gbPrefabs, 48, 200, 0);
	layoutVBox.AddLayoutItem(&properties, 120, 200, 0);
	layoutVBox.AddLayoutItem(&treePrefabs, 200, 200, 600);
	properties.AddPanel(&propertyTransformation);
	propertyTransformation.Clear();

	SetLayout(&layoutVBox);
}

/*virtual*/ C_ScenePrefabsPanel::~C_ScenePrefabsPanel()
{
	delete pPrefabsNode;
}

// *****************************************************************************************************
// *****************************************************************************************************
namespace ScenePrefabsPanel {

	/*virtual*/ void C_TreePrefabsCallback::OnMouseRClick(T_ControlID, const S_ItemInfo& itemInfo)
	{
		C_Menu menu;
		C_Menu& subMenu = menu.AppendSubMenu("");
		ScenePrefabsPanel::T_Node* pNode((ScenePrefabsPanel::T_Node*)itemInfo.param);

		switch(pNode->type) {
	// *****************************************************************************************************
	// *****************************************************************************************************
		case ScenePrefabsPanel::E_Root:
				subMenu.AppendMenu("Add prefab", 1);
				switch(subMenu.TrackPopupMenu(parent)) {
				case 1: // Add instance
					ScenePrefabsPanel::T_Node* pNewPrefabNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Prefab, parent.xmlSource.AddNode(pNode->xmlNode, "prefab"));
					ScenePrefabsPanel::T_Node* pNewPrefabCollisionNode = new ScenePrefabsPanel::T_Node(pNewPrefabNode, ScenePrefabsPanel::E_Collision, parent.xmlSource.AddNode(pNewPrefabNode->xmlNode, "collision"));
					ScenePrefabsPanel::T_Node* pNewPrefabLODNode = new ScenePrefabsPanel::T_Node(pNewPrefabNode, ScenePrefabsPanel::E_LOD, parent.xmlSource.AddNode(pNewPrefabNode->xmlNode, "lod"));

					parent.xmlSource.SetAttributeString(pNewPrefabNode->xmlNode, "name", "newPrefab");
					parent.xmlSource.SetAttributeString(pNewPrefabLODNode->xmlNode, "name", "lod");

					pNewPrefabNode->treeItem = parent.treePrefabs.InsertItem(pNewPrefabNode->xmlNode.Attribute("name").AsString(), pNewPrefabNode, pNode->treeItem, insertSorted);
					pNewPrefabCollisionNode->treeItem = parent.treePrefabs.InsertItem("collision", pNewPrefabCollisionNode, pNewPrefabNode->treeItem, insertSorted);
					pNewPrefabLODNode->treeItem = parent.treePrefabs.InsertItem(pNewPrefabLODNode->xmlNode.Attribute("name").AsString(), pNewPrefabLODNode, pNewPrefabNode->treeItem, insertSorted);

					parent.treePrefabs.BeginEditItem(pNewPrefabNode->treeItem);

					break;
				}
			break;
	// *****************************************************************************************************
	// *****************************************************************************************************
		case ScenePrefabsPanel::E_Collision:
			{
				// In case that collision geometry element is pushed in clipboard we can insert it to collision
				const std::string& pushedResouce = ae::editor::C_AlienEditor::Instance().ClipBoard().PushedResource();
				std::string packageName, resourceName, elementName;
				ae::resource::T_ResourceType resourceType, elementType;
				if(ae::resource::C_ResourceSys::Inst().Resources().ExtractInfoFromResourcePath(pushedResouce.c_str(), packageName, resourceName, resourceType, elementName, elementType)) {
					if(elementType == ae::resource::elementTypeCGeo) {
						C_Menu& insertSubMenu = subMenu.AppendSubMenu("Insert");
						insertSubMenu.AppendMenu(pushedResouce.c_str(), 1);
					}
				}
				switch(subMenu.TrackPopupMenu(parent))
				{
	// *****************************************************************************************************
				case 1: // Insert collision geometry
					{
						ScenePrefabsPanel::T_Node* pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Resource, parent.xmlSource.AddNode(pNode->xmlNode, "rsc"));
						pNewNode->treeItem = parent.treePrefabs.InsertItem(ae::base::GetFileName(elementName.c_str(), false).c_str(), pNewNode, pNode->treeItem, insertSorted);
						parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "name", ae::base::GetFileName(elementName.c_str(), false).c_str());
						parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "type", ae::resource::ResourceType2String(elementType).c_str());
						parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "path", pushedResouce.c_str());
						break;
					}
	// *****************************************************************************************************
				}
				break;
			}
		case ScenePrefabsPanel::E_Prefab:
			{
				subMenu.AppendMenu("Add lod", 1);
				subMenu.AppendMenu("Del prefab\tDel", 2);
				subMenu.AppendMenu("Rename prefab", 3);


				switch(subMenu.TrackPopupMenu(parent)) {
	// *****************************************************************************************************
				case 1: // Add LOD
					{
						ScenePrefabsPanel::T_Node* pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_LOD, parent.xmlSource.AddNode(pNode->xmlNode, "lod"));
						pNewNode->treeItem = parent.treePrefabs.InsertItem("lod", pNewNode, pNode->treeItem, insertSorted);
						break;
					}
	// *****************************************************************************************************
				case 2: // Del prefab
					{
						C_TreeNodesDeleter<ScenePrefabsPanel::T_Node> nodesDelete(parent.treePrefabs, parent.xmlSource, ScenePrefabsPanel::E_Prefab);
						parent.treePrefabs.EnumSelectedItems(nodesDelete);
						nodesDelete.DeleteNodes();
						parent.MarkDirty();
						break;
					}
	// *****************************************************************************************************
				case 3: // Rename instance
					{
						parent.treePrefabs.BeginEditItem(pNode->treeItem);
						break;
					}
				}
				break;
			}
	// *****************************************************************************************************
	// *****************************************************************************************************
		case ScenePrefabsPanel::E_LOD:
			{
				subMenu.AppendMenu("Del LOD\tDel", 1);
				subMenu.AppendMenu("Rename LOD\tDel", 20);

				// In case that geometry element is pushed in clipboard we can insert it to LOD
				const std::string& pushedResouce = ae::editor::C_AlienEditor::Instance().ClipBoard().PushedResource();
				std::string packageName, resourceName, elementName;
				ae::resource::T_ResourceType resourceType, elementType;
				if(ae::resource::C_ResourceSys::Inst().Resources().ExtractInfoFromResourcePath(pushedResouce.c_str(), packageName, resourceName, resourceType, elementName, elementType)) {
					if(elementType == ae::resource::elementTypeMGeo) {
						C_Menu& insertSubMenu = subMenu.AppendSubMenu("Insert");
						insertSubMenu.AppendMenu(pushedResouce.c_str(), 2);
					}
				}

				switch(subMenu.TrackPopupMenu(parent)) {
	// *****************************************************************************************************
				case 1: // Del LOD
					{
						C_TreeNodesDeleter<ScenePrefabsPanel::T_Node> nodesDelete(parent.treePrefabs, parent.xmlSource, ScenePrefabsPanel::E_LOD);
						parent.treePrefabs.EnumSelectedItems(nodesDelete);
						nodesDelete.DeleteNodes();
						parent.MarkDirty();
						break;
					}
	// *****************************************************************************************************
				case 2: // Insert resource from clipboard
					{
						ScenePrefabsPanel::T_Node* pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Resource, parent.xmlSource.AddNode(pNode->xmlNode, "rsc"));
						pNewNode->treeItem = parent.treePrefabs.InsertItem(ae::base::GetFileName(elementName.c_str(), false).c_str(), pNewNode, pNode->treeItem, insertSorted);
						parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "name", ae::base::GetFileName(elementName.c_str(), false).c_str());
						parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "type", ae::resource::ResourceType2String(elementType).c_str());
						parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "path", pushedResouce.c_str());
						break;
					}
	// *****************************************************************************************************
				case 20: // Rename LOD
					{
						parent.treePrefabs.BeginEditItem(pNode->treeItem);
						break;
					}
				}
				break;
			}
	// *****************************************************************************************************
	// *****************************************************************************************************
		case ScenePrefabsPanel::E_Resource:
			{
				subMenu.AppendMenu("Del node\tDel", 1);
				subMenu.AppendMenu("Add instance", 2);
				subMenu.AppendMenu("Rename", 3);
				subMenu.AppendMenu("Cast shadow", 4);

				// *****************************************************************************************************
				const std::string& pushedResouce = ae::editor::C_AlienEditor::Instance().ClipBoard().PushedResource();
				std::string packageName, resourceName, elementName;
				ae::resource::T_ResourceType resourceType, elementType;
				if(ae::resource::C_ResourceSys::Inst().Resources().ExtractInfoFromResourcePath(pushedResouce.c_str(), packageName, resourceName, resourceType, elementName, elementType)) {
					if((elementType == ae::resource::elementTypeMGeo && std::string("mgeo") == pNode->xmlNode.Attribute("type").AsString()) ||
						(elementType == ae::resource::elementTypeCGeo && std::string("cgeo") == pNode->xmlNode.Attribute("type").AsString())
						)
					{
						C_Menu& insertSubMenu = subMenu.AppendSubMenu("Replace");
						insertSubMenu.AppendMenu(pushedResouce.c_str(), 5);
					}
				}
				// *****************************************************************************************************

				switch(subMenu.TrackPopupMenu(parent)) {
	// *****************************************************************************************************
				case 1:{ // Del
						C_TreeNodesDeleter<ScenePrefabsPanel::T_Node> nodesDelete(parent.treePrefabs, parent.xmlSource, ScenePrefabsPanel::E_Resource);
						parent.treePrefabs.EnumSelectedItems(nodesDelete);
						nodesDelete.DeleteNodes();
						parent.MarkDirty();
						break;
					}
	// *****************************************************************************************************
				case 2: // Add instance
					{
						ScenePrefabsPanel::T_Node* pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Instance, parent.xmlSource.AddNode(pNode->xmlNode, "instance"));
						pNewNode->treeItem = parent.treePrefabs.InsertItem("instance", pNewNode, pNode->treeItem, insertSorted);
						parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "name", "instance");
						ae::resource::SetPosRotScaleToXML(parent.xmlSource, pNewNode->xmlNode, parent.selectedInstancePos, parent.selectedInstanceRot, parent.selectedInstanceScale);

						parent.MarkDirty();
						break;
					}
   // *****************************************************************************************************
				case 3: // Rename Resource
					parent.treePrefabs.BeginEditItem(pNode->treeItem);
					break;
	// *****************************************************************************************************
				case 4: // Cast shadow
					if(!pNode->xmlNode.Node("shadows").IsValid())
						parent.xmlSource.AddNode(pNode->xmlNode,"shadows");
					parent.xmlSource.SetAttributeUInt(pNode->xmlNode.Node("shadows"), "cast", 1);
					parent.MarkDirty();
					break;
	// *****************************************************************************************************
				case 5 : // Replace
					parent.xmlSource.SetAttributeString(pNode->xmlNode, "path", pushedResouce.c_str());
					parent.MarkDirty();
					break;
   // *****************************************************************************************************
				}
				break;
			}
	// *****************************************************************************************************
	// *****************************************************************************************************
		case ScenePrefabsPanel::E_Instance:
			{
				subMenu.AppendMenu("Del\tDel", 1);
				subMenu.AppendMenu("Rename", 2);
				subMenu.AppendMenu("Duplicate ", 3);
				switch(subMenu.TrackPopupMenu(parent)) {
	// *****************************************************************************************************
				case 1 : // Del instance transformations
					{
						C_TreeNodesDeleter<ScenePrefabsPanel::T_Node> nodesDelete(parent.treePrefabs, parent.xmlSource, ScenePrefabsPanel::E_Instance);
						parent.treePrefabs.EnumSelectedItems(nodesDelete);
						nodesDelete.DeleteNodes();
						parent.MarkDirty();
						break;
					}
	// *****************************************************************************************************
				case 2: // Rename instance transformation
					{
						parent.treePrefabs.BeginEditItem(pNode->treeItem);
						break;
					}
	// *****************************************************************************************************
				case 3: // Duplicate instance transformation
					{
						C_DuplicationsDialog dlg(&parent);
						dlg.SetConfig(parent.duplicationDlgConfig);
						if(dlg.DoModal() == C_WDialog::returnOK)
						{
					
							ae::math::float3 delta_pos, delta_rot, delta_scale;
							dlg.GetTransformation(delta_pos, delta_rot, delta_scale);
							C_DuplicateInstances duplicateInstances(parent.treePrefabs, parent.xmlSource, delta_pos, delta_rot, delta_scale, dlg.GetNumDuplications());
							parent.treePrefabs.EnumSelectedItems(duplicateInstances);
							parent.MarkDirty();
						}

						dlg.GetConfig(parent.duplicationDlgConfig);

						break;
					}
				}
			}
	// *****************************************************************************************************
	// *****************************************************************************************************
		}
	}

	/*virtual*/ bool C_TreePrefabsCallback::OnItemRenamed(T_ControlID, const char* oldString, const char* newString, void* param, T_TreeItem)
	{
		ScenePrefabsPanel::T_Node* pNode((ScenePrefabsPanel::T_Node*)param);
		if(!IsNodeNameUnique(pNode->pParent->xmlNode, newString))
			return false;
		parent.xmlSource.SetAttributeString(pNode->xmlNode, "name", newString);
		return true;
	}

	/*virtual*/ bool C_TreePrefabsCallback::OnItemRenaming(T_ControlID, const S_ItemInfo& itemInfo) {
		ScenePrefabsPanel::T_Node* pNode((ScenePrefabsPanel::T_Node*)itemInfo.param);
		return IsNodeRenameable(pNode->xmlNode);
	}

	/*virtual*/ void C_TreePrefabsCallback::OnFilesDropped(T_ControlID, const S_ItemInfo&, const T_DroppedFiles& droppedFiles)
	{
	}

	/*virtual*/ void C_TreePrefabsCallback::OnItemSelected(T_ControlID, const S_ItemInfo& itemInfo)
	{
		ScenePrefabsPanel::T_Node* pNode((ScenePrefabsPanel::T_Node*)itemInfo.param);
		if(pNode->type == ScenePrefabsPanel::E_Instance)
		{
			parent.propertyTransformation.Set(&parent.xmlSource, &pNode->xmlNode);
			parent.propertyTransformation.GetPos(parent.selectedInstancePos);
			parent.propertyTransformation.GetRot(parent.selectedInstanceRot);
			parent.propertyTransformation.GetScale(parent.selectedInstanceScale);
			parent.propertyTransformation.Enable(true);
		}
		else
		{
			parent.propertyTransformation.Clear();
			parent.propertyTransformation.Enable(false);
		}
	}

} // namespace ScenePrefabsPanel {

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_ScenePrefabsPanel::OnSave()
{
	std::ostream* pOstream(ae::base::C_FileSys::Inst().CreateOutputStream(textPrefab.GetText().c_str()));
	ae::base::C_XML_Text_Writer xml_save_writer(xmlSource, *pOstream);
	delete pOstream;

	C_BasePanel::OnSave();
}

/*virtual*/ void C_ScenePrefabsPanel::OnLoad()
{
	selectedInstancePos = ae::math::float3(0,0,0);
	selectedInstanceRot = ae::math::float3(0,0,0);
	selectedInstanceScale = ae::math::float3(1,1,1);

	LoadPrefabs(textPrefab.GetText().c_str());

	C_BasePanel::OnLoad();
}

// *****************************************************************************************************
/*virtual*/ void C_ScenePrefabsPanel::LoadCfg(const ae::base::C_XML<>& xml)
{
	duplicationDlgConfig.LoadCfg(xml, xml.Root().Node("SceneInstances_Panel"));
}

/*virtual*/ void C_ScenePrefabsPanel::SaveCfg(ae::base::C_XML_Editable<>& xml)
{
	ae::base::S_XMLNode& sceneInstancesPanelNode = xml.AddNode(xml.Root(), "SceneInstances_Panel");
	duplicationDlgConfig.SaveCfg(xml, sceneInstancesPanelNode );
}

// *****************************************************************************************************

/*virtual*/ void C_ScenePrefabsPanel::OnCommand(T_ControlID commandID)
{
	if(btnSavePrefabs.GetControlID() == commandID) OnSave();
	else if(btnLoadPrefabs.GetControlID() == commandID) OnLoad();
}

/*virtual*/ void C_ScenePrefabsPanel::Edit(const char* resourceName, const char* elementName)
{
	if(textPrefab.GetText() != resourceName)
		LoadPrefabs(resourceName);
}

void C_ScenePrefabsPanel::LoadPrefabs(const char* fileName)
{
	// destroying old tree and it's nodes
	if(pPrefabsNode)	{
		treePrefabs.RemoveItem(pPrefabsNode->treeItem);
		treePrefabs.RemoveAllInfoTips();
		delete pPrefabsNode;
		pPrefabsNode = 0;
	}
	// loading data to xml and insert nodes to tree
	ae::base::I_InputStream* pInputStream(ae::base::C_FileSys::Inst().CreateInputStream(fileName));
	if(pInputStream)
	{
		textPrefab.SetText(fileName);
		ae::base::C_XML_Text_Reader<> xmlTextReader(*pInputStream, xmlSource);
		delete pInputStream;
	}

	pPrefabsNode = new ScenePrefabsPanel::T_Node(0, ScenePrefabsPanel::E_Root, xmlSource.Root());
	pPrefabsNode->treeItem = treePrefabs.InsertItem("prefabs", pPrefabsNode, 0);
	LoadNode(pPrefabsNode);
}

void C_ScenePrefabsPanel::LoadNode(ScenePrefabsPanel::T_Node* pNode)
{
	if(!pNode) return;
	ae::base::S_XMLNode* xmlNode(pNode->xmlNode.child);
	while(xmlNode)
	{
		ScenePrefabsPanel::T_Node* pNewNode = 0;
		if(xmlNode->Is("prefab"))
		{
			pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Prefab, *xmlNode);
			pNewNode->treeItem = treePrefabs.InsertItem(xmlNode->Attribute("name").AsString(), pNewNode, pNode->treeItem, insertSorted);
		}
		else if(xmlNode->Is("lod"))
		{
			pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_LOD, *xmlNode);
			pNewNode->treeItem = treePrefabs.InsertItem(xmlNode->Attribute("name").AsString(), pNewNode, pNode->treeItem, insertSorted);
		}
		else if(xmlNode->Is("collision"))
		{
			pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Collision, *xmlNode);
			pNewNode->treeItem = treePrefabs.InsertItem("collision", pNewNode, pNode->treeItem, 0);
		}
		else if(xmlNode->Is("rsc"))
		{
			pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Resource, *xmlNode);
			pNewNode->treeItem = treePrefabs.InsertItem(xmlNode->Attribute("name").AsString(), pNewNode, pNode->treeItem, insertSorted);
		}
		else if(xmlNode->Is("instance"))
		{
			pNewNode = new ScenePrefabsPanel::T_Node(pNode, ScenePrefabsPanel::E_Instance, *xmlNode);
			pNewNode->treeItem = treePrefabs.InsertItem(xmlNode->Attribute("name").AsString(), pNewNode, pNode->treeItem, insertSorted);
		}
		
		LoadNode(pNewNode);

		xmlNode = xmlNode->next;
	}
}


} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

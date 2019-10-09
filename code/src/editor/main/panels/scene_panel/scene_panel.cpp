#include "stdafx.h"

#if defined(AE_EDITOR)

#include "scene_panel.h"

#include "../../utils/tree_nodes_deleter.h"
#include "../../utils/node_names.h"

#include "../../../alien_editor.h"

#include "../../../windows_sys/widgets/message_box.h"
#include "../../../windows_sys/base/menu.h"

#include "../../../../engine/resource_sys/imports/importers/import_utils.h"
#include "../../../../engine/resource_sys/imports/imports.h"
#include "../../../../engine/resource_sys/resource_sys.h"

#include "../../../../engine/base_sys/xml/xml_text_writer.h"
#include "../../../../engine/base_sys/xml/xml_text_reader.h"
#include "../../../../engine/base_sys/streams/i_input_stream.h"
#include "../../../../engine/base_sys/file_system/file_system.h"

#include "../../../../engine/scene_sys/scene_sys.h"

#include "../../../../engine/physic_sys/physic_sys.h"
#include "../../../../engine/physic_sys/scene/scene.h"
#include "../../../../engine/physic_sys/scene/collision_params.h"

#include "../../../../engine/render_sys/render_scene/render_camera.h"
#include "../../../../engine/render_sys/render_scene/render_scene.h"
#include "../../../../engine/render_sys/render_sys.h"

namespace ae { namespace editor {

namespace ScenePanel {

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_ReplacePrefab : public ae::windows_sys::I_TreeViewSelectionEnumerator
	{
		typedef ae::base::C_XML_Editable<> T_XMLSource;
		C_TreeView& treeView;
		T_XMLSource& xmlSource;
		const char* prefabResource;
	public:
		C_ReplacePrefab(C_TreeView& treeView, T_XMLSource& xmlSource, const char* prefabResource) : 
		  treeView(treeView), xmlSource(xmlSource), prefabResource(prefabResource) {}

		virtual void OnSelectedItem(const C_TreeViewCallback::S_ItemInfo& itemInfo)
		{
			ScenePanel::T_Node* pNode((ScenePanel::T_Node*)itemInfo.param);
			if(pNode->type == ScenePanel::E_SceneNode)
			{
				xmlSource.SetAttributeString(pNode->xmlNode, "path", prefabResource);
			}
		}
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_DuplicateSceneNode : public ae::windows_sys::I_TreeViewSelectionEnumerator
	{
		typedef ae::base::C_XML_Editable<> T_XMLSource;
		C_TreeView& treeView;
		T_XMLSource& xmlSource;
		ae::math::float3 delta_pos, delta_rot, delta_scale;
		ae::u32 numDuplicates;
	public:
		C_DuplicateSceneNode(	C_TreeView& treeView, 
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
			ScenePanel::T_Node* pNode((ScenePanel::T_Node*)itemInfo.param);
			if(pNode->type == ScenePanel::E_SceneNode)
			{
				ae::math::float3 pos, rot, scale;
				ae::resource::GetPosRotScaleFromXMLTransformation(pNode->xmlNode, pos, rot, scale);

				for(ae::u32 i = 0;i < numDuplicates; i++)
				{
					pos += delta_pos;
					rot += delta_rot;
					scale *= delta_scale;

					ScenePanel::T_Node* pNewNode = new ScenePanel::T_Node(pNode, ScenePanel::E_SceneNode, xmlSource.AddNode(pNode->pParent->xmlNode,"node"));
					xmlSource.SetAttributeString(pNewNode->xmlNode, "name", GetUniqueName(pNode->pParent->xmlNode, pNode->xmlNode.Attribute("name").AsString(),"_").c_str());
					xmlSource.SetAttributeString(pNewNode->xmlNode, "type", pNode->xmlNode.Attribute("type").AsString());
					xmlSource.SetAttributeString(pNewNode->xmlNode, "path", pNode->xmlNode.Attribute("path").AsString());
					ae::resource::SetPosRotScaleToXML(xmlSource, pNewNode->xmlNode, pos, rot, scale);
					pNewNode->treeItem = treeView.InsertItem(pNewNode->xmlNode.Attribute("name").AsString(), pNewNode, pNode->pParent->treeItem, insertSorted);
				}
			}
		}
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_SceneTranformationChange : public ae::windows_sys::I_TreeViewSelectionEnumerator
	{
		ae::base::C_XML_Editable<>& source;
		ae::math::E_TransformationElement element;
		float elementValue;
	public:
		C_SceneTranformationChange(ae::base::C_XML_Editable<>& _source, ae::math::E_TransformationElement _element, float value) : source(_source), element(_element), elementValue(value) { }
		virtual void OnSelectedItem(const C_TreeViewCallback::S_ItemInfo& itemInfo)
		{
			ScenePanel::T_Node* pNode((ScenePanel::T_Node*)itemInfo.param);
			if(pNode->type != ScenePanel::E_SceneNode) return;
			ae::resource::SetTransformationElementToXML(source,pNode->xmlNode,element, elementValue);

			ae::math::float3 pos, rot, scale;
			ae::resource::GetPosRotScaleFromXMLTransformation(pNode->xmlNode,pos, rot, scale);
			ae::math::float4x4 matrix;
			rot.x = ::ae::math::Deg2Radians(rot.x);
			rot.y = ::ae::math::Deg2Radians(rot.y);
			rot.z = ::ae::math::Deg2Radians(rot.z);
			matrix.TranslateRotateScale(pos, rot, scale);

			ae::scene::C_SceneSys::Inst().Scene().UpdateNodeTransformation(pNode->xmlNode.Attribute("name").AsString(), matrix);
		}
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	/*virtual*/ void C_SceneGizmoNotifier::OnTransformationChange(const ae::math::float3& pos,
		const ae::math::float3& rot,
		const ae::math::float3& scale)
	{
		parent.propertyTransformation.SetPos(pos);
		parent.propertyTransformation.SetRot(rot);
		parent.propertyTransformation.SetScale(scale);
	}

	/*virtual*/ void C_ScenePanelPropTransformationCB::OnPropertyChanged()
	{
		ae::math::float3 pos, rot, scale;
		panel.propertyTransformation.GetPos(pos);
		panel.propertyTransformation.GetRot(rot);
		panel.propertyTransformation.GetScale(scale);
		C_AlienEditor::Instance().Gizmo().SetPos(pos);
		C_AlienEditor::Instance().Gizmo().SetRot(rot);
		C_AlienEditor::Instance().Gizmo().SetScale(scale);
	}

	/*virtual*/ void C_ScenePanelPropTransformationCB::OnTransformationChanged(ae::math::E_TransformationElement element, float value)
	{
		C_SceneTranformationChange changeTransformations(panel.xmlSource, element, value);
		panel.treeScene.EnumSelectedItems(changeTransformations);
	}

	// *****************************************************************************************************
	// *****************************************************************************************************

	/*virtual*/ void C_TVSceneCallback::OnMouseRClick(T_ControlID, const S_ItemInfo& itemInfo)
	{
		C_Menu menu;
		C_Menu& subMenu = menu.AppendSubMenu("");
		ScenePanel::T_Node* pNode((ScenePanel::T_Node*)itemInfo.param);
		switch(pNode->type) {
			// *****************************************************************************************************
		case ScenePanel::E_Root:
			{
				const std::string& pushedResouce = ae::editor::C_AlienEditor::Instance().ClipBoard().PushedResource();
				std::string packageName, resourceName, elementName;
				ae::resource::T_ResourceType resourceType, elementType;
				if(ae::resource::C_ResourceSys::Inst().Resources().ExtractInfoFromResourcePath(pushedResouce.c_str(), packageName, resourceName, resourceType, elementName, elementType)) {
					if(elementType == ae::resource::resourceTypePrefab) {
						C_Menu& subMenu0 = subMenu.AppendSubMenu("Insert to zero pos");
						subMenu0.AppendMenu(pushedResouce.c_str(), 1);
						C_Menu& subMenu1 = subMenu.AppendSubMenu("Insert to cursor pos");
						subMenu1.AppendMenu(pushedResouce.c_str(), 2);
					}
				}

				auto cmdID(subMenu.TrackPopupMenu(parent));
				if(cmdID == 1 || cmdID == 2)
				{
					ScenePanel::T_Node* pNewNode = new ScenePanel::T_Node(pNode, ScenePanel::E_SceneNode, parent.xmlSource.AddNode(pNode->xmlNode, "node"));
					pNewNode->treeItem = parent.treeScene.InsertItem(ae::base::GetFileName(elementName.c_str(), false).c_str(), pNewNode, pNode->treeItem, insertSorted);
					parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "name", ae::base::GetFileName(elementName.c_str(), false).c_str());
					parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "type", ae::resource::ResourceType2String(elementType).c_str());
					parent.xmlSource.SetAttributeString(pNewNode->xmlNode, "path", pushedResouce.c_str());

					if(cmdID == 2)
					{
						const ae::render::C_Camera& camera(ae::render::C_RenderSys::Instance().GetScene().GetCamera());
						ae::physic::S_CollisionResult collResult;
						if(ae::physic::C_PhysicSys::Instance().Scene().LineCollision(camera.GetPos() + camera.GetDir() * 1.0f, camera.GetPos() + camera.GetDir() * 100.0f, collResult, 0))
							ae::resource::SetPosToXML(parent.xmlSource,pNewNode->xmlNode,collResult.collisionContact);
					}
				}

				parent.MarkDirty();
				break;
			}
			// *****************************************************************************************************
		case ScenePanel::E_SceneNode:
			{
				subMenu.AppendMenu("Del node\tDel", 1);
				subMenu.AppendMenu("Rename node\tDel", 2);
				subMenu.AppendMenu("Duplicate ", 3);

				const std::string& pushedResouce = ae::editor::C_AlienEditor::Instance().ClipBoard().PushedResource();
				std::string packageName, resourceName, elementName;
				ae::resource::T_ResourceType resourceType, elementType;
				if(ae::resource::C_ResourceSys::Inst().Resources().ExtractInfoFromResourcePath(pushedResouce.c_str(), packageName, resourceName, resourceType, elementName, elementType) && elementType == ae::resource::resourceTypePrefab) {
					C_Menu& subMenu0 = subMenu.AppendSubMenu("Replace with prefab");
					subMenu0.AppendMenu(pushedResouce.c_str(), 4);
				}
				
				switch(subMenu.TrackPopupMenu(parent)) {
				case 1: // Del node
					{
						C_TreeNodesDeleter<ScenePanel::T_Node> nodesDelete(parent.treeScene, parent.xmlSource, ScenePanel::E_SceneNode);
						parent.treeScene.EnumSelectedItems(nodesDelete);
						nodesDelete.DeleteNodes();
						parent.MarkDirty();
						break;		
					}
				case 2: // Rename node
					{
						parent.treeScene.BeginEditItem(pNode->treeItem);
						break;
					}
				case 3: // Duplicate
					{
						C_DuplicationsDialog dlg(&parent);
						dlg.SetConfig(parent.duplicationDlgConfig);
						if(dlg.DoModal() == C_WDialog::returnOK)
						{
							ae::math::float3 delta_pos, delta_rot, delta_scale;
							dlg.GetTransformation(delta_pos, delta_rot, delta_scale);
							C_DuplicateSceneNode duplicateSceneNode(parent.treeScene, parent.xmlSource, delta_pos, delta_rot, delta_scale, dlg.GetNumDuplications());
							parent.treeScene.EnumSelectedItems(duplicateSceneNode);
							parent.MarkDirty();
						}
						dlg.GetConfig(parent.duplicationDlgConfig);
						break;
					}
				case 4: // replace scene nodes with prefab
					{
						C_ReplacePrefab replacePrefab(parent.treeScene, parent.xmlSource, pushedResouce.c_str());
						parent.treeScene.EnumSelectedItems(replacePrefab);
						parent.MarkDirty();
						break;
					}
				}
				break;
			}
		}
	}

	/*virtual*/ bool C_TVSceneCallback::OnItemRenamed(T_ControlID, const char* oldString, const char* newString, void* param, T_TreeItem)
	{
		ScenePanel::T_Node* pNode((ScenePanel::T_Node*)param);
		if(!IsNodeNameUnique(pNode->pParent->xmlNode, newString))
			return false;
		parent.xmlSource.SetAttributeString(pNode->xmlNode, "name", newString);
		return true;
	}

	/*virtual*/ bool C_TVSceneCallback::OnItemRenaming(T_ControlID, const S_ItemInfo& itemInfo) {
		ScenePanel::T_Node* pNode((ScenePanel::T_Node*)itemInfo.param);
		return IsNodeRenameable(pNode->xmlNode);
	}

	/*virtual*/ void C_TVSceneCallback::OnFilesDropped(T_ControlID, const S_ItemInfo&, const T_DroppedFiles& droppedFiles)
	{
	}

	/*virtual*/ void C_TVSceneCallback::OnItemSelected(T_ControlID, const S_ItemInfo& itemInfo)
	{
		ScenePanel::T_Node* pNode((ScenePanel::T_Node*)itemInfo.param);
		if(pNode->type == ScenePanel::E_SceneNode)
		{
			parent.propertyTransformation.Set(&parent.xmlSource, &pNode->xmlNode);
			parent.propertyTransformation.Enable(true);
			parent.propTransformationPropCB.OnPropertyChanged();
			C_AlienEditor::Instance().Gizmo().Show(true);
		}
		else
		{
			parent.propertyTransformation.Clear();
			parent.propertyTransformation.Enable(false);
			C_AlienEditor::Instance().Gizmo().Show(false);
		}
	}

} // namespace ScenePanel {

// *****************************************************************************************************
// *****************************************************************************************************

C_ScenePanel::C_ScenePanel(C_Widget* pParent) : 
	C_BasePanel(pParent),
	treeSceneCallback(*this),
	gizmoNotifier(*this),
	propTransformationPropCB(*this),
	properties(this),
	propertyTransformation(this, &propTransformationPropCB),
	layoutVBox(300),
	layoutHBox(20),
	gbScene(this, " Scene "),
	m_TextScene(0, this, 1),
	treeScene(&treeSceneCallback, this, (ae::windows_sys::T_ControlID)300),
	btnSave(this, "Save", (ae::windows_sys::T_ControlID)101),
	btnLoad(this, "Load", (ae::windows_sys::T_ControlID)102),
	pScenesNode(0)
{
	m_TextScene.EnableWindow(false);

	layoutHBox.AddLayoutItem(&m_TextScene,100,400);
	layoutHBox.AddLayoutItem(&btnLoad,30);
	layoutHBox.AddLayoutItem(&btnSave,30);
	gbScene.SetLayout(&layoutHBox);

	layoutVBox.AddLayoutItem(&gbScene, 48, 200, 0);
	layoutVBox.AddLayoutItem(&properties, 120, 200, 0);
	layoutVBox.AddLayoutItem(&treeScene, 200, 200, 600);

	properties.AddPanel(&propertyTransformation);
	propertyTransformation.Clear();

	SetLayout(&layoutVBox);
}

/*virtual*/ C_ScenePanel::~C_ScenePanel()
{
	delete pScenesNode;
}

// *****************************************************************************************************
// *****************************************************************************************************

/*virtual*/ void C_ScenePanel::OnCommand(T_ControlID commandID)
{
	if(btnSave.GetControlID() == commandID) OnSave();
	else if(btnLoad.GetControlID() == commandID) OnLoad();
}

/*virtual*/ void C_ScenePanel::Edit(const char* resourceName, const char* elementName)
{
	if(m_TextScene.GetText() != resourceName)
		LoadScene(resourceName);
}
// *****************************************************************************************************

/*virtual*/ void C_ScenePanel::OnSave()
{
	std::ostream* pOstream(ae::base::C_FileSys::Inst().CreateOutputStream(m_TextScene.GetText().c_str()));
	ae::base::C_XML_Text_Writer xml_save_writer(xmlSource, *pOstream);
	delete pOstream;

	C_BasePanel::OnSave();
}

/*virtual*/ void C_ScenePanel::OnLoad()
{
	LoadScene(m_TextScene.GetText().c_str());
	C_AlienEditor::Instance().Gizmo().Show(false);

	C_BasePanel::OnLoad();
}
// *****************************************************************************************************

/*virtual*/ void C_ScenePanel::LoadCfg(const ae::base::C_XML<>& xml)
{
	duplicationDlgConfig.LoadCfg(xml, xml.Root().Node("Scene_Panel"));
}
/*virtual*/ void C_ScenePanel::SaveCfg(ae::base::C_XML_Editable<>& xml)
{
	ae::base::S_XMLNode& scenePanelNode = xml.AddNode(xml.Root(), "Scene_Panel");
	duplicationDlgConfig.SaveCfg(xml, scenePanelNode );
}

// *****************************************************************************************************

void C_ScenePanel::LoadScene(const char* fileName)
{
	// destroying old tree and it's nodes
	if(pScenesNode)	{
		treeScene.RemoveItem(pScenesNode->treeItem);
		treeScene.RemoveAllInfoTips();
		delete pScenesNode;
		pScenesNode = 0;
	}
	// loading data to xml and insert nodes to tree
	ae::base::I_InputStream* pInputStream(ae::base::C_FileSys::Inst().CreateInputStream(fileName));
	if(pInputStream)
	{
		m_TextScene.SetText(fileName);
		ae::base::C_XML_Text_Reader<> xmlTextReader(*pInputStream, xmlSource);
		delete pInputStream;
	}
	pScenesNode = new ScenePanel::T_Node(0, ScenePanel::E_Root, xmlSource.Root());
	pScenesNode->treeItem = treeScene.InsertItem("scene", pScenesNode, 0);
	LoadNode(pScenesNode);
}

void C_ScenePanel::LoadNode(ScenePanel::T_Node* pNode)
{
	ae::base::S_XMLNode* xmlNode(pNode->xmlNode.child);
	while(xmlNode)
	{
		if(xmlNode->Is("node"))	{
			ScenePanel::T_Node* pNewNode = new ScenePanel::T_Node(pNode, ScenePanel::E_SceneNode, *xmlNode);
			pNewNode->treeItem = treeScene.InsertItem(xmlNode->Attribute("name").AsString(), pNewNode, pNode->treeItem, insertSorted);
		}

		xmlNode = xmlNode->next;
	}
}
// *****************************************************************************************************

/*virtual*/ void C_ScenePanel::Activate()
{
	C_AlienEditor::Instance().Gizmo().RegisterNotifier(&gizmoNotifier);
}

/*virtual*/void C_ScenePanel::Deactivate()
{
	C_AlienEditor::Instance().Gizmo().UnregisterNotifier(&gizmoNotifier);
	C_AlienEditor::Instance().Gizmo().Show(false);
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

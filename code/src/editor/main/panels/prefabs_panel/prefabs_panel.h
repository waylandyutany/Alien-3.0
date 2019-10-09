#pragma once
#if !defined(AE_PREFABS_PANEL_H) && defined(AE_EDITOR)
#define AE_PREFABS_PANEL_H

#include "../base_panel.h"
#include "../../utils/tree_node_t.h"
#include "../../properties/prop_container.h"
#include "../../properties/prop_panel_transformation.h"
#include "../../dialogs/duplication_dialog.h"

namespace ae { namespace editor {
// *****************************************************************************************************
	class C_ScenePrefabsPanel;

	namespace ScenePrefabsPanel {

		enum E_Type
		{
			E_Root,
			E_Prefab,
			E_LOD,
			E_Instance,
			E_Resource,
			E_Collision
		};

		typedef S_TreeNode<E_Type> T_Node;

// *****************************************************************************************************
// *****************************************************************************************************
		class C_TreePrefabsCallback : public ae::windows_sys::C_TreeViewCallback
		{
			C_ScenePrefabsPanel& parent;
			virtual void OnMouseRClick(T_ControlID, const S_ItemInfo&);
			virtual bool OnItemRenamed(T_ControlID, const char* oldString, const char* newString, void* param, T_TreeItem);
			virtual bool OnItemRenaming(T_ControlID, const S_ItemInfo&);
			virtual void OnFilesDropped(T_ControlID, const S_ItemInfo&, const T_DroppedFiles& droppedFiles);
			virtual void OnItemSelected(T_ControlID, const S_ItemInfo&);
		public:
			C_TreePrefabsCallback(C_ScenePrefabsPanel& parent) : parent(parent) {}
		};

// *****************************************************************************************************
// *****************************************************************************************************
		class C_TransformationCB : public I_PropPanelTransformationCB
		{
			C_ScenePrefabsPanel& parent;
		public:
			C_TransformationCB(C_ScenePrefabsPanel& parent) : parent(parent) {}
			virtual void OnTransformationChanged(ae::math::E_TransformationElement, float value);
		};

	} // namespace ScenePrefabsPanel {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_ScenePrefabsPanel : public C_BasePanel
	{
		C_ScenePrefabsPanel();
		NOCOPY_CLASS(C_ScenePrefabsPanel);
// *****************************************************************************************************
		friend class ScenePrefabsPanel::C_TreePrefabsCallback;
		friend class ScenePrefabsPanel::C_TransformationCB;
// *****************************************************************************************************
		ScenePrefabsPanel::C_TransformationCB		transformationCB;
		ScenePrefabsPanel::C_TreePrefabsCallback	treePrefabsCallback;
		C_EditBox				textPrefab;
		C_GroupBox				gbPrefabs;
		C_TreeView				treePrefabs;
		C_PushButton			btnSavePrefabs;
		C_PushButton			btnLoadPrefabs;
		C_VBoxLayout			layoutVBox;
		C_HBoxLayout			layoutHBox;
		C_PropertyContainer		properties;
		C_PropPanelTransformation propertyTransformation;
		C_DuplicationDialogConfig duplicationDlgConfig;
// *****************************************************************************************************
		ae::base::C_XML_Editable<> xmlSource;
		ae::math::float3 selectedInstancePos, selectedInstanceRot, selectedInstanceScale;
	private:
		ScenePrefabsPanel::T_Node *pPrefabsNode;
// *****************************************************************************************************
		void LoadPrefabs(const char* fileName);
		void LoadNode(ScenePrefabsPanel::T_Node* pNode);
	public:
// *****************************************************************************************************
		C_ScenePrefabsPanel(C_Widget*);
		~C_ScenePrefabsPanel();
// *****************************************************************************************************
		virtual void Edit(const char* resourceName, const char* elementName);
		virtual void OnCommand(T_ControlID);
		virtual void OnSave();
		virtual void OnLoad();
		virtual void LoadCfg(const ae::base::C_XML<>& xml);
		virtual void SaveCfg(ae::base::C_XML_Editable<>& xml);
// ****************************************************************************************************
	};

// *****************************************************************************************************
// *****************************************************************************************************

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_PREFABS_PANEL_H) && defined(AE_EDITOR)

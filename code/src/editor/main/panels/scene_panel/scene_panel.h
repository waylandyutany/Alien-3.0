#pragma once
#if !defined(AE_SCENE_PANEL_H) && defined(AE_EDITOR)
#define AE_SCENE_PANEL_H

#include "../base_panel.h"
#include "../../utils/tree_node_t.h"
#include "../../properties/prop_container.h"
#include "../../properties/prop_panel_transformation.h"
#include "../../dialogs/duplication_dialog.h"
#include "../../gizmo.h"

namespace ae { namespace editor {
	class C_ScenePanel;

	namespace ScenePanel {

		enum E_Type
		{
			E_Root,
			E_SceneNode,
		};
		typedef S_TreeNode<E_Type> T_Node;

		// *****************************************************************************************************
		// *****************************************************************************************************
		class C_TVSceneCallback : public ae::windows_sys::C_TreeViewCallback
		{
			C_ScenePanel& parent;
			virtual void OnMouseRClick(T_ControlID, const S_ItemInfo&);
			virtual bool OnItemRenamed(T_ControlID, const char* oldString, const char* newString, void* param, T_TreeItem);
			virtual bool OnItemRenaming(T_ControlID, const S_ItemInfo& itemInfo);
			virtual void OnFilesDropped(T_ControlID, const S_ItemInfo&, const T_DroppedFiles& droppedFiles);
			virtual void OnItemSelected(T_ControlID, const S_ItemInfo&);
		public:
			C_TVSceneCallback(C_ScenePanel& parent) : parent(parent) {}
		};

		// *****************************************************************************************************
		// *****************************************************************************************************
		class C_SceneGizmoNotifier : public I_GizmoNotifier
		{
			C_ScenePanel& parent;
			virtual void OnTransformationChange(const ae::math::float3& pos,
				const ae::math::float3& rot,
				const ae::math::float3& scale);
		public:
			C_SceneGizmoNotifier(C_ScenePanel& parent) : parent(parent) {}
		};

		// *****************************************************************************************************
		// *****************************************************************************************************
		class C_ScenePanelPropTransformationCB : public I_PropPanelTransformationCB
		{
			C_ScenePanel& panel;
		public:
			virtual void OnPropertyChanged();
			virtual void OnTransformationChanged(ae::math::E_TransformationElement, float value);
			C_ScenePanelPropTransformationCB(C_ScenePanel& _panel) : panel(_panel) {}
		};

	} // namespace ScenePanel {

	class C_ScenePanel : public C_BasePanel
	{
		C_ScenePanel();
		NOCOPY_CLASS(C_ScenePanel);
		friend class ScenePanel::C_TVSceneCallback;
		friend class ScenePanel::C_SceneGizmoNotifier;
		friend class ScenePanel::C_ScenePanelPropTransformationCB;
// *****************************************************************************************************
		ScenePanel::C_TVSceneCallback		treeSceneCallback;
		ScenePanel::C_SceneGizmoNotifier	gizmoNotifier;
		ScenePanel::C_ScenePanelPropTransformationCB propTransformationPropCB;
		C_EditBox				m_TextScene;
		C_GroupBox				gbScene;
		C_TreeView				treeScene;
		C_PushButton			btnSave;
		C_PushButton			btnLoad;
		C_VBoxLayout			layoutVBox;
		C_HBoxLayout			layoutHBox;
		C_PropertyContainer		properties;
		C_PropPanelTransformation propertyTransformation;
		C_DuplicationDialogConfig duplicationDlgConfig;
// *****************************************************************************************************
		ae::base::C_XML_Editable<> xmlSource;
		ScenePanel::T_Node* pScenesNode;
// *****************************************************************************************************
		void LoadScene(const char* fileName);
		void LoadNode(ScenePanel::T_Node* pNode);
public:
// *****************************************************************************************************
		C_ScenePanel(C_Widget*);
		~C_ScenePanel();
// *****************************************************************************************************
		virtual void Edit(const char* resourceName, const char* elementName);
		virtual void OnCommand(T_ControlID);
		virtual void OnSave();
		virtual void OnLoad();
		virtual void Activate();
		virtual void Deactivate();
		virtual void LoadCfg(const ae::base::C_XML<>& xml);
		virtual void SaveCfg(ae::base::C_XML_Editable<>& xml);
// *****************************************************************************************************
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_SCENE_PANEL_H) && defined(AE_EDITOR)

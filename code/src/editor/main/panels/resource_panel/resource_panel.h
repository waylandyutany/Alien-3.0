#pragma once
#if !defined(AE_RESOURCE_PANEL_H) && defined(AE_EDITOR)
#define AE_RESOURCE_PANEL_H

#include "../base_panel.h"
#include "../../../../engine/resource_sys/resource_sys.h"

namespace ae { namespace editor {

	class C_ResourcePanel;
// *****************************************************************************************************
// *****************************************************************************************************
	class C_CBPackageCallback : public ae::windows_sys::C_ComboBoxCallback
	{
		C_ResourcePanel& m_Parent;
		virtual void OnStringSelected(ae::windows_sys::T_ControlID controID, const char* string);
	public:
		C_CBPackageCallback(C_ResourcePanel& parent) : m_Parent(parent) {}
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_TVResourcesCallback : public ae::windows_sys::C_TreeViewCallback
	{
		C_ResourcePanel& m_Parent;
		virtual void OnMouseRClick(T_ControlID, const S_ItemInfo&);
		virtual void OnMouseLDBClick(T_ControlID, const S_ItemInfo&);
		virtual bool OnItemRenamed(T_ControlID, const char* oldString, const char* newString, void* param, T_TreeItem);
		virtual void OnFilesDropped(T_ControlID, const S_ItemInfo&, const T_DroppedFiles& droppedFiles);
		virtual void OnItemSelected(T_ControlID, const S_ItemInfo&);
	public:
		C_TVResourcesCallback(C_ResourcePanel& parent) : m_Parent(parent) {}
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_ResourcePanel : public C_BasePanel
	{
		C_ResourcePanel();
		NOCOPY_CLASS(C_ResourcePanel);
// *****************************************************************************************************
		C_CBPackageCallback		m_CBPackageCallback;
		C_TVResourcesCallback	m_TVResourcesCallback;
		C_DropDownComboBox		m_CBPackage;
		C_GroupBox				m_GBPackage;
		C_TreeView				m_TVResources;
		C_PushButton			m_BTNewPackage;
		C_PushButton			m_BTSavePackage;
		C_PushButton			m_BTLoadPackage;
		C_VBoxLayout			m_VBoxLayout;
		C_HBoxLayout			m_HBoxLayout;
// *****************************************************************************************************
		std::string m_sPackageName;
		ae::base::C_XML_Editable<> m_PackageSrc;
		ae::resource::C_PackagesManager& m_PackagesManager;
// *****************************************************************************************************
		void NewPackage();
		void ReloadPackages();
		void LoadPackage(const char* name);
		void DeletePackage(const char* name);
// *****************************************************************************************************
		friend class C_PackageEnumerator;
		friend class C_CBPackageCallback;
		friend class C_TVResourcesCallback;
// *****************************************************************************************************
		enum E_NodeType
		{
			E_NT_ROOT,
			E_NT_DIR,
			E_NT_SRC,
			E_NT_ELEMENT
		};

// *****************************************************************************************************
		struct S_Node
		{
			typedef std::deque<S_Node*> T_Nodes;
			S_Node* pParent;
			T_Nodes childs;
			E_NodeType nodeType;
			ae::base::S_XMLNode& node;
			T_TreeItem treeItem;
// *****************************************************************************************************
			S_Node(S_Node* parent, E_NodeType nt, ae::base::S_XMLNode& _node) : pParent(parent), nodeType(nt), node(_node), treeItem(0)	{
				if(pParent) pParent->childs.push_back(this);
			}
			void DestroyChilds()
			{
				T_Nodes tmpChilds(childs);
				childs.clear();
				ae::Purge(tmpChilds);
			}

			~S_Node() {
				DestroyChilds();
				if(pParent && !pParent->childs.empty())
					ae::RemoveElement(pParent->childs, this);
			}
// *****************************************************************************************************
			E_NodeType NodeType() const { return nodeType; }
// *****************************************************************************************************
			S_Node* SrcExist(const char* name, ae::resource::T_ResourceType);
			S_Node* DirExist(const char* name);
		};

		S_Node* m_pNode;
// *****************************************************************************************************
		S_Node* AddSubDir(S_Node& node, const char* dirName);
		void RemoveSubDir(S_Node* node);
		void LoadNode(S_Node* node);
		void LoadElements(S_Node* node);
// *****************************************************************************************************
		void AddSrc(S_Node& node, const char* srcFile);
		void RemoveSrc(S_Node* node);
// *****************************************************************************************************
		void AddNewScene(S_Node* node);
		void AddNewScenePrefabs(S_Node* node);
// *****************************************************************************************************
		// return full path from tree with type extension and element if any
		void GetNodePath(S_Node&, std::string& path) const;
		std::string GetNodePath(S_Node&) const;
// *****************************************************************************************************
	public:
		C_ResourcePanel(C_Widget*);
		~C_ResourcePanel();
// *****************************************************************************************************
		virtual void LoadCfg(const ae::base::C_XML<>& xml);
		virtual void SaveCfg(ae::base::C_XML_Editable<>& xml);
		virtual void OnSave();
		virtual void OnLoad();
		virtual void OnCommand(T_ControlID);
// *****************************************************************************************************
	};

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_RESOURCE_PANEL_H) && defined(AE_EDITOR)

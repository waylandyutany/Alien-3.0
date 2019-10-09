#pragma once
#if !defined(AE_TREE_NODES_DELETER_H) && defined(AE_EDITOR)
#define AE_TREE_NODES_DELETER_H

#include "../../windows_sys/widgets/tree_view.h"

namespace ae { namespace editor {
	template<typename T_Node>
	class C_TreeNodesDeleter : public ae::windows_sys::I_TreeViewSelectionEnumerator
	{
		typedef ae::base::C_XML_Editable<> T_XMLSource;
		C_TreeView& treeView;
		T_XMLSource& xmlSource;
		typedef typename T_Node::T_NodeType T_NodeType;
		typedef std::deque<T_Node*> T_Nodes;
		T_NodeType typeToDelete;
		T_Nodes nodesToDelete;
	public:
		C_TreeNodesDeleter(C_TreeView& treeView, T_XMLSource& xmlSource, T_NodeType typeToDelete);
		~C_TreeNodesDeleter();

		void DeleteNodes();

		virtual void OnSelectedItem(const C_TreeViewCallback::S_ItemInfo& itemInfo) {
			// first we store all valid selected nodes
			// then we must call DeleteNodes() to delete them
			// it's because other not previously items may be selected when we delete currently selected item
			T_Node* pNode((T_Node*)itemInfo.param);
			if(pNode->type == typeToDelete)
				nodesToDelete.push_back(pNode);
		}
	};


} } //namespace ae { namespace editor {

#include "tree_nodes_deleter.inl"

#endif // #if !defined(AE_TREE_NODES_DELETER_H) && defined(AE_EDITOR)

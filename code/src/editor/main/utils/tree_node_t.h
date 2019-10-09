#pragma once
#if !defined(AE_TREE_NODE_T_H) && defined(AE_EDITOR)
#define AE_TREE_NODE_T_H

#include "../../windows_sys/widgets/tree_view.h"

namespace ae { namespace base {
	struct S_XMLNode;
} } // namespace ae { namespace base {

namespace ae { namespace editor {

	template<typename T_Enum>
	struct S_TreeNode
	{
		S_TreeNode(S_TreeNode* parent, T_Enum t, ae::base::S_XMLNode& node);
		virtual ~S_TreeNode();
		void DestroyChilds();
		ae::u32 NumChilds() const;

		typedef T_Enum T_NodeType;
		typedef std::deque<S_TreeNode*> T_Nodes;

		S_TreeNode* pParent;
		T_Nodes childs;
		T_NodeType type;
		ae::windows_sys::T_TreeItem treeItem;
		ae::base::S_XMLNode& xmlNode;
	};

} } //namespace ae { namespace editor {

#include "tree_node_t.inl"

#endif // #if !defined(AE_TREE_NODE_T_H) && defined(AE_EDITOR)

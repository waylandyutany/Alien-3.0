namespace ae { namespace editor {

template<typename T_Enum>
inline S_TreeNode<T_Enum>::S_TreeNode(S_TreeNode* parent, T_Enum t, ae::base::S_XMLNode& node) :
	pParent(parent), type(t), xmlNode(node)
{
	if(pParent) pParent->childs.push_back(this);
}

template<typename T_Enum>
inline S_TreeNode<T_Enum>::~S_TreeNode()
{
	DestroyChilds();
	if(pParent && !pParent->childs.empty())
		ae::RemoveElement(pParent->childs, this);
}

template<typename T_Enum>
inline void S_TreeNode<T_Enum>::DestroyChilds()
{
	T_Nodes tmpChilds(childs);
	childs.clear();
	ae::Purge(tmpChilds);
}

template<typename T_Enum>
inline ae::u32 S_TreeNode<T_Enum>::NumChilds() const {
	return (ae::u32)childs.size();
}

} } //namespace ae { namespace editor {

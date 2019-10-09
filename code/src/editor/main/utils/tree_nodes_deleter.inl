namespace ae { namespace editor {

template<typename T_Node>
inline C_TreeNodesDeleter<T_Node>::C_TreeNodesDeleter(C_TreeView& treeView, T_XMLSource& xmlSource, T_NodeType typeToDelete) : 
	treeView(treeView), xmlSource(xmlSource), typeToDelete(typeToDelete) {
	}

template<typename T_Node>
inline C_TreeNodesDeleter<T_Node>::~C_TreeNodesDeleter() {
	AE_ASSERT(nodesToDelete.empty() && "You must call DeleteNodes() to delete slected items !");
}

template<typename T_Node>
inline void C_TreeNodesDeleter<T_Node>::DeleteNodes()
{
	T_Nodes::iterator i = nodesToDelete.begin();
	for(; i != nodesToDelete.end(); ++i)
	{
		treeView.RemoveItem((*i)->treeItem);
		xmlSource.DelNode((*i)->pParent->xmlNode, &(*i)->xmlNode);
		delete (*i);
	}
	nodesToDelete.clear();
}

} } //namespace ae { namespace editor {

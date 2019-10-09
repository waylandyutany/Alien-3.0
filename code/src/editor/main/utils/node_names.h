#pragma once
#if !defined(AE_NODE_NAMES_H) && defined(AE_EDITOR)
#define AE_NODE_NAMES_H

namespace ae { namespace base {
	struct S_XMLNode;
} } // namespace ae { namespace base {

namespace ae { namespace editor {

	// return true if node has "name attribute" which means that holds a name, otherwise false is returned
	bool IsNodeRenameable(ae::base::S_XMLNode& node);
	// return true if there is no child node with name attribute same as name, it's case insensitive
	bool IsNodeNameUnique(ae::base::S_XMLNode& parentNode, const char* name);
	// if given name already exists function return unique which has unique number at the end
	std::string GetUniqueName(ae::base::S_XMLNode& parentNode, const char* name, const char* postfix);

} } //namespace ae { namespace editor {

#endif // #if !defined(AE_TREE_NODE_T_H) && defined(AE_EDITOR)

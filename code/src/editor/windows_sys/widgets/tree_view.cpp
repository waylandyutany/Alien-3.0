#include "stdafx.h"

#if defined(AE_EDITOR)

#include "tree_view.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************

C_TreeView::C_TreeView(C_TreeViewCallback* pCallback, C_Widget* pParent, T_ControlID id) :
	C_Widget(pParent, windowType_TreeView, 
		windowStyle_EditLabels	|
		windowStyle_Border		|
		windowStyle_Visible		|
		windowStyle_TabStop		|
		windowStyle_Child		|
		windowStyle_HasLines	|
		windowStyle_HasButtons	|
		windowStyle_ShowSelAlways|
		windowStyle_LinesAsRoot	,
		id),
	control(pCallback, *this)
{
}

void C_TreeView::EnumSelectedItems(I_TreeViewSelectionEnumerator& enumerator)
{
	std::string str;
	C_TreeViewCallback::S_ItemInfo itemInfo;
	for(itemInfo.item = control.GetFirstSelectedItem(); itemInfo.item != 0; itemInfo.item = control.GetNextSelectedItem(itemInfo.item))
	{
		control.GetItemInfo(itemInfo.item, str, &itemInfo.param);
		itemInfo.string = str.c_str();
		enumerator.OnSelectedItem(itemInfo);
	}
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)

#pragma once
#if !defined(AE_TREE_VIEW_CTRL_H) && defined(AE_EDITOR)
#define AE_TREE_VIEW_CTRL_H

#if (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#include "../win32/controls/tree_view_ctrl_win32.h"
#elif
	#error No window platform implemented !
#endif

#include "../window.h"

namespace ae { namespace windows_sys {

	class C_TreeViewCtrl
	{
		C_TreeViewCtrl();
		NOCOPY_CLASS(C_TreeViewCtrl);
		T_TreeViewCtrl_Impl m_Impl;
	public:
		C_TreeViewCtrl(C_TreeViewCallback*, C_Window& control);
// *****************************************************************************************************
		T_TreeItem InsertItem(const char* string, void* param, T_TreeItem parent, T_TreeItem insertAfter = 0);
		void RemoveItem(T_TreeItem item);
		void SortChildItems(T_TreeItem item);

		void BeginEditItem(T_TreeItem item);
		void SelectItem(T_TreeItem item);
		void EnsureVisibleItem(T_TreeItem item);

		bool GetItemInfo(T_TreeItem item, std::string& str, void** pParam) const;
		void SetItemText(T_TreeItem item, const char*);
// *****************************************************************************************************
		void AddInfoTip(T_TreeItem item, const TCHAR* infoTip);
		void RemoveInfoTip(T_TreeItem item);
		void RemoveAllInfoTips();
// *****************************************************************************************************
		T_TreeItem GetFirstSelectedItem();
		T_TreeItem GetNextSelectedItem(T_TreeItem);
		T_TreeItem GetPreviousSelectedItem(T_TreeItem);
// *****************************************************************************************************
	};

} } // namespace ae { namespace windows_sys {

#include "tree_view_ctrl.inl"
#endif // #if !defined(AE_TREE_VIEW_CTRL_H) && defined(AE_EDITOR)

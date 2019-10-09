#pragma once
#if !defined(AE_TREE_VIEW_CTRL_WIN32_H) && defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))
#define AE_TREE_VIEW_CTRL_WIN32_H

#include "base_ctrl_win32.h"
#include "../../win32/window_win32.h"

namespace ae { namespace windows_sys {
	typedef HTREEITEM T_TreeItem;
	static const T_TreeItem insertFirst		= TVI_FIRST;
	static const T_TreeItem insertLast		= TVI_LAST;
	static const T_TreeItem insertSorted	= TVI_SORT;

	class C_TreeViewCallback;
} } // namespace ae { namespace windows_sys {

namespace ae { namespace windows_sys {  namespace win32 {

	class C_TreeViewCtrl_Win32 : public C_BaseCtrl_Win32
	{
		C_TreeViewCtrl_Win32();
		NOCOPY_CLASS(C_TreeViewCtrl_Win32);
// *****************************************************************************************************
		C_TreeViewCallback* pCallback;
		std::string editedString;
		T_TreeItem draggedItem, firstSelectedItem;
		bool bDraggedRMB : 1;
		bool bMultiSelect : 1;
		typedef std::map<T_TreeItem, std::basic_string<TCHAR> > T_InfoTips;
		typedef std::deque<T_TreeItem> T_Items;
		T_InfoTips infoTips;
		T_Items selectedItems;
// *****************************************************************************************************
		T_TreeItem MouseHitTest();
		T_TreeItem MouseHitTest(const POINT&);
		T_TreeItem GetSelectedItem();
		T_TreeItem GetRootItem();
		T_TreeItem GetNextItem(T_TreeItem);
		T_TreeItem GetPreviousItem(T_TreeItem);
		T_TreeItem GetNextVisibleItem(T_TreeItem);
		T_TreeItem GetPreviousVisibleItem(T_TreeItem);
		void SetItemState(T_TreeItem item, UINT state, UINT stateMask);
		UINT GetItemState(T_TreeItem item, UINT stateMask);

		void ClearSelection();
		bool SelectItems(T_TreeItem hItemFrom, T_TreeItem hItemTo);
		void RemoveSelectedItem(T_TreeItem);
		void StoreSelectedItem(T_TreeItem);
public:
		C_TreeViewCtrl_Win32(C_TreeViewCallback*, C_Window_Win32& control);
		~C_TreeViewCtrl_Win32();
// *****************************************************************************************************
		T_TreeItem InsertItem(const char* string, void* param, T_TreeItem parent, T_TreeItem insertAfter = 0);
		void RemoveItem(T_TreeItem item);
		void SortChildItems(T_TreeItem item);

		void BeginEditItem(T_TreeItem item);
		void SelectItem(T_TreeItem item);
		void EnsureVisibleItem(T_TreeItem item);

		bool GetItemInfo(T_TreeItem item, std::string& str, void** pParam) const;
		void SetItemText(T_TreeItem item, const char*);

		T_TreeItem GetFirstSelectedItem();
		T_TreeItem GetNextSelectedItem(T_TreeItem);
		T_TreeItem GetPreviousSelectedItem(T_TreeItem);
// *****************************************************************************************************
		void AddInfoTip(T_TreeItem item, const TCHAR* infoTip);
		void RemoveInfoTip(T_TreeItem item);
		void RemoveAllInfoTips();
// *****************************************************************************************************
		virtual S_WPReturn WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result);
	};

} } } // namespace ae { namespace windows_sys {  namespace win32 {

namespace ae { namespace windows_sys {
	typedef ae::windows_sys::win32::C_TreeViewCtrl_Win32 T_TreeViewCtrl_Impl;
} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_TREE_VIEW_CTRL_WIN32_H) && defined(AE_EDITOR)

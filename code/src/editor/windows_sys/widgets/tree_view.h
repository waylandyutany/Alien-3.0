#pragma once
#if !defined(AE_TREE_VIEW_H) && defined(AE_EDITOR)
#define AE_TREE_VIEW_H

#include "widget.h"

#include "../base/controls/tree_view_ctrl.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
	class C_TreeViewCallback : public C_BaseWidgetCallback
	{
	public:
		struct S_ItemInfo
		{
			S_ItemInfo() : string(0), param(0), item(0) {}
			S_ItemInfo(const char* _string, void* _param, T_TreeItem _item) : string(_string), param(_param), item(_item) {}
			bool IsValidItem() const { return item ? true : false; }
			const char* string;
			void* param;
			T_TreeItem item;
		};

		virtual void OnItemSelected(T_ControlID, const S_ItemInfo&) {};

		// return true if you want to accept renamed item otherwise return false to keep old item name
		virtual bool OnItemRenamed(T_ControlID, const char* oldString, const char* newString, void* param, T_TreeItem) { return true; };
		virtual bool OnItemRenaming(T_ControlID, const S_ItemInfo&) { return true; }
		virtual void OnMouseRClick(T_ControlID, const S_ItemInfo&) {};
		virtual void OnMouseLClick(T_ControlID, const S_ItemInfo&) {};
		virtual void OnMouseRDBClick(T_ControlID, const S_ItemInfo&) {};
		virtual void OnMouseLDBClick(T_ControlID, const S_ItemInfo&) {};

		typedef std::vector<std::string> T_DroppedFiles;
		virtual void OnFilesDropped(T_ControlID, const S_ItemInfo&, const T_DroppedFiles& droppedFiles) {};

		virtual void OnItemDroppedLMB(T_ControlID, const S_ItemInfo& draggedItem, const S_ItemInfo& droppedItem) {};
		virtual void OnItemDroppedRMB(T_ControlID, const S_ItemInfo& draggedItem, const S_ItemInfo& droppedItem) {};

		virtual void OnItemDeleted(T_ControlID, const S_ItemInfo&) {};
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class I_TreeViewSelectionEnumerator
	{
	public:
		virtual ~I_TreeViewSelectionEnumerator() {};
		virtual void OnSelectedItem(const C_TreeViewCallback::S_ItemInfo&) = 0;
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_TreeView : public C_Widget
	{
		C_TreeView();
		NOCOPY_CLASS(C_TreeView);
		C_TreeViewCtrl control;
	public:
		C_TreeView(C_TreeViewCallback*, C_Widget*, T_ControlID);

		T_TreeItem InsertItem(const char* string, void* param, T_TreeItem parent, T_TreeItem insertAfter = 0);
		void SortChildItems(T_TreeItem item);
		void RemoveItem(T_TreeItem item);
		void BeginEditItem(T_TreeItem item);
		void SelectItem(T_TreeItem item);
		void EnsureVisibleItem(T_TreeItem item);
		void AddInfoTip(T_TreeItem item, const TCHAR* infoTip);
		void RemoveInfoTip(T_TreeItem item);
		void RemoveAllInfoTips();
		bool GetItemInfo(T_TreeItem item, std::string& str, void** pParam) const;
		void SetItemText(T_TreeItem item, const char*);
		void EnumSelectedItems(I_TreeViewSelectionEnumerator&);
	};

} } // namespace ae { namespace windows_sys {

#include "tree_view.inl"

#endif // #if !defined(AE_TREE_VIEW_H) && defined(AE_EDITOR)

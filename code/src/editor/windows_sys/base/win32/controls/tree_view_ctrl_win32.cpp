#include "stdafx.h"

#if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

#include "tree_view_ctrl_win32.h"
#include "../../../widgets/tree_view.h"

namespace ae { namespace windows_sys {  namespace win32 {

#ifdef UNICODE
	void String2String(std::string& str, const wchar_t* cstr)
	{
		if(!cstr) return;
		char buffer[::ae::maxStringSize];
		memset(buffer,0,::ae::maxStringSize);
		WideCharToMultiByte(CP_ACP,0,cstr, (int)_tcslen(cstr), buffer, ::ae::maxStringSize,0,0);
		str = buffer;
	}
#else
	void String2String(std::string& str, const char* cstr)
	{
		if(!cstr) return;
		str = cstr;
	}
#endif

C_TreeViewCtrl_Win32::C_TreeViewCtrl_Win32(C_TreeViewCallback* pCallback, C_Window_Win32& control) :
	pCallback(pCallback), 
	C_BaseCtrl_Win32(control),
	draggedItem(0), firstSelectedItem(0),
	bDraggedRMB(false),
	bMultiSelect(true)
{
}

C_TreeViewCtrl_Win32::~C_TreeViewCtrl_Win32()
{
}

// *****************************************************************************************************
// *****************************************************************************************************
T_TreeItem C_TreeViewCtrl_Win32::InsertItem(const char* string, void* param, T_TreeItem parent, T_TreeItem insertAfter)
{
	TV_INSERTSTRUCTA tvinsert;
	memset(&tvinsert, 0, sizeof(tvinsert));
	tvinsert.hParent = parent;
	tvinsert.hInsertAfter = insertAfter;
	tvinsert.item.lParam = (LPARAM)param;
	tvinsert.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvinsert.item.pszText = (LPSTR)string;
	return (T_TreeItem)::SendMessage(m_Control, TVM_INSERTITEMA, 0, (LPARAM)&tvinsert);
}

void C_TreeViewCtrl_Win32::SortChildItems(T_TreeItem item)
{
	::SendMessage(m_Control, TVM_SORTCHILDREN, 0, (LPARAM)item);
}

void C_TreeViewCtrl_Win32::RemoveItem(T_TreeItem item)
{
	::SendMessage(m_Control, TVM_DELETEITEM, 0, (LPARAM)item);

	// removing info tips
	infoTips.erase(item);
	// removing item from list of selected items
	RemoveSelectedItem(item);
}

void C_TreeViewCtrl_Win32::BeginEditItem(T_TreeItem item)
{
	::SendMessage(m_Control, TVM_EDITLABEL, 0, (LPARAM)item);
}

void C_TreeViewCtrl_Win32::SelectItem(T_TreeItem item)
{
	::SendMessage(m_Control, TVM_SELECTITEM, TVGN_CARET, (LPARAM)item);
}

void C_TreeViewCtrl_Win32::EnsureVisibleItem(T_TreeItem item)
{
	::SendMessage(m_Control, TVM_ENSUREVISIBLE, 0, (LPARAM)item);
}

void C_TreeViewCtrl_Win32::SetItemText(T_TreeItem item, const char* pszText)
{
	TVITEMA tvitem;
	tvitem.hItem = item;
	tvitem.pszText = (LPSTR)pszText;
	tvitem.mask = TVIF_TEXT;
	::SendMessageA(m_Control, TVM_SETITEMA, 0, (LPARAM)&tvitem);
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_TreeViewCtrl_Win32::AddInfoTip(T_TreeItem item, const TCHAR* infoTip)
{
	if(infoTips.empty())
		::SetWindowLong(m_Control, GWL_STYLE, ::GetWindowLong(m_Control, GWL_STYLE) | TVS_INFOTIP);

	infoTips[item] = infoTip;
}

void C_TreeViewCtrl_Win32::RemoveInfoTip(T_TreeItem item)
{
	infoTips.erase(item);

	if(infoTips.empty())
		::SetWindowLong(m_Control, GWL_STYLE, ::GetWindowLong(m_Control, GWL_STYLE) & ~TVS_INFOTIP);
}

void C_TreeViewCtrl_Win32::RemoveAllInfoTips() {
	infoTips.clear();
}

T_TreeItem C_TreeViewCtrl_Win32::GetSelectedItem() {
	return (T_TreeItem)::SendMessage(m_Control, TVM_GETNEXTITEM, TVGN_CARET, 0);
}

T_TreeItem C_TreeViewCtrl_Win32::GetRootItem() {
	return (T_TreeItem)::SendMessage(m_Control, TVM_GETNEXTITEM, TVGN_ROOT, 0);
}

T_TreeItem C_TreeViewCtrl_Win32::GetNextItem(T_TreeItem item) {
	return (T_TreeItem)::SendMessage(m_Control, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)item);
}

T_TreeItem C_TreeViewCtrl_Win32::GetPreviousItem(T_TreeItem item) {
	return (T_TreeItem)::SendMessage(m_Control, TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM)item);
}

T_TreeItem C_TreeViewCtrl_Win32::GetNextVisibleItem(T_TreeItem item) {
	return (T_TreeItem)::SendMessage(m_Control, TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM)item);
}

T_TreeItem C_TreeViewCtrl_Win32::GetPreviousVisibleItem(T_TreeItem item) {
	return (T_TreeItem)::SendMessage(m_Control, TVM_GETNEXTITEM, TVGN_PREVIOUSVISIBLE, (LPARAM)item);
}

// *****************************************************************************************************
T_TreeItem C_TreeViewCtrl_Win32::GetFirstSelectedItem() {
	if(!selectedItems.empty())
		return *selectedItems.begin();
	return 0;
}

T_TreeItem C_TreeViewCtrl_Win32::GetNextSelectedItem(T_TreeItem item) {
	T_Items::iterator i(std::find(selectedItems.begin(), selectedItems.end(), item));
	if(i == selectedItems.end())
	{	// if we are unable to find previous selected item we return first one selected if any
		// this is prevention for premature GetNextSelectedItem returning 0 when item has been deleted
		if(!selectedItems.empty())
			return *selectedItems.begin();
		return 0;
	}
	if(++i == selectedItems.end()) return 0;
	return *i;
}

T_TreeItem C_TreeViewCtrl_Win32::GetPreviousSelectedItem(T_TreeItem item) {
	T_Items::iterator i(std::find(selectedItems.begin(), selectedItems.end(), item));
	if(i == selectedItems.end()) return 0;
	if(--i == selectedItems.end()) return 0;
	return *i;
}

// *****************************************************************************************************
void C_TreeViewCtrl_Win32::RemoveSelectedItem(T_TreeItem item)
{
	T_Items::iterator i(std::find(selectedItems.begin(), selectedItems.end(), item));
	if( i != selectedItems.end())
		selectedItems.erase(i);
}

void C_TreeViewCtrl_Win32::StoreSelectedItem(T_TreeItem item)
{
	if(std::find(selectedItems.begin(), selectedItems.end(), item) == selectedItems.end())
		selectedItems.push_back(item);
}

// *****************************************************************************************************
void C_TreeViewCtrl_Win32::SetItemState(T_TreeItem item, UINT state, UINT stateMask) {
	TVITEM tvitem;
	tvitem.hItem = item;
	tvitem.mask = TVIF_STATE;
	tvitem.state = state;
	tvitem.stateMask = stateMask;
	::SendMessage(m_Control, TVM_SETITEM, 0, (LPARAM)&tvitem);

	// every selected items is remembered in selectedItems list
	if(stateMask & TVIS_SELECTED) {
		if(state & TVIS_SELECTED) { // store selected item in the list
			StoreSelectedItem(item);
		}
		else { // removing selected item
			RemoveSelectedItem(item);
		}
	}
}

UINT C_TreeViewCtrl_Win32::GetItemState(T_TreeItem item, UINT stateMask) {
	TVITEM tvitem;
	tvitem.hItem = item;
	tvitem.mask = TVIF_STATE;
	tvitem.state = 0;
	tvitem.stateMask = stateMask;
	::SendMessage(m_Control, TVM_GETITEM, 0, (LPARAM)&tvitem);
	return tvitem.state;
}

void C_TreeViewCtrl_Win32::ClearSelection()
{
	T_Items items(selectedItems);
	T_Items::iterator i = items.begin();
	for(;i != items.end(); ++i)
		SetItemState(*i, 0, TVIS_SELECTED);

//	selectedItems.clear();
	AE_ASSERT(selectedItems.empty() && "After ClearSelection call, selectedItems must be empty !");
}

// SelectItems	- Selects items from hItemFrom to hItemTo. Does not
//		- select child item if parent is collapsed. Removes
//		- selection from all other items
// hItemFrom	- item to start selecting from
// hItemTo	- item to end selection at.
bool C_TreeViewCtrl_Win32::SelectItems(T_TreeItem hItemFrom, T_TreeItem hItemTo)
{
	T_TreeItem hItem = GetRootItem();

	// Clear selection upto the first item
	while ( hItem && hItem!=hItemFrom && hItem!=hItemTo ) {
		hItem = GetNextVisibleItem( hItem );
		SetItemState( hItem, 0, TVIS_SELECTED );
	}

	if ( !hItem ) return false;	// Item is not visible
	SelectItem( hItemTo );
	// Rearrange hItemFrom and hItemTo so that hItemFirst is at top
	if( hItem == hItemTo ) {
		hItemTo = hItemFrom;
		hItemFrom = hItem;
	}

	// Go through remaining visible items
	bool bSelect = true;
	while ( hItem ) {
		// Select or remove selection depending on whether item
		// is still within the range.
		SetItemState( hItem, bSelect ? TVIS_SELECTED : 0, TVIS_SELECTED );
		// Do we need to start removing items from selection
		if(hItem == hItemTo) bSelect = FALSE;
		hItem = GetNextVisibleItem( hItem );
	}

	return true;
}
// *****************************************************************************************************
/*virtual*/ S_WPReturn C_TreeViewCtrl_Win32::WndProc(UINT umsg, WPARAM wparam, LPARAM lparam, S_WPReturn result)
{
	void* tmpParam;
	std::string tmpStr;
	C_TreeViewCallback::S_ItemInfo tmpItemInfo;
// *****************************************************************************************************
// Multi selection support
// *****************************************************************************************************
	if(bMultiSelect && umsg == WM_LBUTTONDOWN)
	{
		POINT pt;
		pt.x = LOWORD(lparam); pt.y = HIWORD(lparam);

		if(wparam & MK_CONTROL)	{
			T_TreeItem hItem(MouseHitTest(pt));
			if(hItem) {
				// Toggle selection state
				UINT uNewSelState = GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED ? 0 : TVIS_SELECTED;
				// Get old selected (focus) item and state
				HTREEITEM hItemOld = GetSelectedItem();
				UINT uOldSelState  = hItemOld ? GetItemState(hItemOld, TVIS_SELECTED) : 0;
				// editing prevention
				if(GetSelectedItem() == hItem) SelectItem(NULL);
				// call default wnd proc and let it do it's job
				result = S_WPReturn(m_Control.CallDefaultWndProc(umsg, wparam, lparam), true);
				// Set proper selection (highlight) state for new item
				SetItemState(hItem, uNewSelState,  TVIS_SELECTED);
				// Restore state of old selected item
				if (hItemOld && hItemOld != hItem) SetItemState(hItemOld, uOldSelState, TVIS_SELECTED);
				// clear firstSelectedItem
				firstSelectedItem = 0;
				return result;
			}
		}
		else if(wparam & MK_SHIFT) {
			T_TreeItem hItem(MouseHitTest(pt));
			// Initialize the reference item if this is the first shift selection
			if(!firstSelectedItem)	firstSelectedItem = GetSelectedItem();
			// Select new item
			if(GetSelectedItem() == hItem) SelectItem(NULL); // to prevent edit
			// call default wnd proc and let it do it's job
			result = S_WPReturn(m_Control.CallDefaultWndProc(umsg, wparam, lparam), true);
			// select items in range
			if(firstSelectedItem) SelectItems( firstSelectedItem, hItem );
			return result;
		}
		else {
			ClearSelection();
			firstSelectedItem = 0;
		}
	}
// *****************************************************************************************************
	if(!pCallback) return result;
// *****************************************************************************************************
	if(draggedItem && (umsg == WM_LBUTTONUP || umsg == WM_RBUTTONUP))
	{
		HTREEITEM droppedItem(MouseHitTest());
		if(droppedItem)
		{
			std::string draggedString, droppedString;
			GetItemInfo(draggedItem, draggedString, &tmpParam);
			C_TreeViewCallback::S_ItemInfo draggedItemInfo(draggedString.c_str(), tmpParam, draggedItem);

			GetItemInfo(droppedItem, droppedString, &tmpParam);
			C_TreeViewCallback::S_ItemInfo droppedItemInfo(droppedString.c_str(), tmpParam, droppedItem);

			if(!bDraggedRMB)
				pCallback->OnItemDroppedLMB(m_Control.GetControlID(), draggedItemInfo, droppedItemInfo);
			else 
				pCallback->OnItemDroppedRMB(m_Control.GetControlID(), draggedItemInfo, droppedItemInfo);

			SelectItem(droppedItem);
		}
		draggedItem = 0;
	}
// *****************************************************************************************************
/*
	else if(m_DraggedItem && umsg == WM_MOUSEMOVE)
	{
		HTREEITEM hitTarget(MouseHitTest());
		if(hitTarget)
			::SendMessage(m_Control, TVM_SELECTITEM, TVGN_DROPHILITE,(LPARAM)hitTarget);
	}
*/
// *****************************************************************************************************
	else if(umsg == WM_DROPFILES)
	{
		C_TreeViewCallback::T_DroppedFiles droppedFiles;
		HDROP hDrop((HDROP)wparam);
		UINT numFiles = DragQueryFileA(hDrop, 0xFFFFFFFF, 0, 0);
		for(UINT i = 0; i < numFiles; i++)
		{
			char buffer[::ae::maxFilePathSize];
			DragQueryFileA(hDrop, i, buffer, ::ae::maxFilePathSize);
			droppedFiles.push_back(std::string(buffer));
		}

		HTREEITEM hitItem(MouseHitTest());
		GetItemInfo(hitItem, tmpStr, &tmpParam);
		tmpItemInfo = C_TreeViewCallback::S_ItemInfo(tmpStr.c_str(), tmpParam, hitItem);

		pCallback->OnFilesDropped(m_Control.GetControlID(), tmpItemInfo, droppedFiles);
	}
// *****************************************************************************************************
	else if(umsg == WM_NOTIFY)
	{
		LPNMHDR nmhdr((LPNMHDR)lparam);
// *****************************************************************************************************
		if(nmhdr->code == TVN_SELCHANGED)
		{
			LPNMTREEVIEWA treeView((LPNMTREEVIEWA)lparam);
			if(treeView->itemNew.hItem)
			{
				StoreSelectedItem(treeView->itemNew.hItem);
				GetItemInfo(treeView->itemNew.hItem, tmpStr, &tmpParam);
				pCallback->OnItemSelected(m_Control.GetControlID(), C_TreeViewCallback::S_ItemInfo(tmpStr.c_str(), tmpParam, treeView->itemNew.hItem));
			}
		}
// *****************************************************************************************************
		else if(nmhdr->code == NM_CLICK || nmhdr->code == NM_RCLICK || nmhdr->code == NM_RDBLCLK || nmhdr->code == NM_DBLCLK)
		{
			HTREEITEM hitItem(MouseHitTest());
			if(hitItem)
			{
				GetItemInfo(hitItem, tmpStr, &tmpParam);
				tmpItemInfo = C_TreeViewCallback::S_ItemInfo(tmpStr.c_str(), tmpParam, hitItem);

				if(nmhdr->code == NM_CLICK)
					pCallback->OnMouseLClick(m_Control.GetControlID(), tmpItemInfo);
				if(nmhdr->code == NM_RCLICK)
				{
					pCallback->OnMouseRClick(m_Control.GetControlID(), tmpItemInfo);
					if(hitItem)	SelectItem(hitItem);
				}
				if(nmhdr->code == NM_RDBLCLK)
					pCallback->OnMouseRDBClick(m_Control.GetControlID(), tmpItemInfo);
				if(nmhdr->code == NM_DBLCLK)
					pCallback->OnMouseLDBClick(m_Control.GetControlID(), tmpItemInfo);
			}
		}
// *****************************************************************************************************
		else if(nmhdr->code == TVN_BEGINLABELEDIT)
		{
			LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)lparam;

			GetItemInfo(ptvdi->item.hItem, tmpStr, &tmpParam);
			tmpItemInfo = C_TreeViewCallback::S_ItemInfo(tmpStr.c_str(), tmpParam, ptvdi->item.hItem);

			if(pCallback->OnItemRenaming(m_Control.GetControlID(), tmpItemInfo))
			{
				String2String(editedString,ptvdi->item.pszText);
				return false;
			}
			return S_WPReturn(1, true);
		}
// *****************************************************************************************************
		else if(nmhdr->code == TVN_ENDLABELEDIT)
		{
			LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)lparam;
			if(ptvdi->item.pszText)
			{
				std::string str;
				String2String(str,ptvdi->item.pszText);
				if(pCallback->OnItemRenamed(m_Control.GetControlID(), editedString.c_str(), str.c_str(), (void*)ptvdi->item.lParam, ptvdi->item.hItem))
				{
					SetItemText(ptvdi->item.hItem, str.c_str());
					return true;
				}
				return false;
			}
		}
// *****************************************************************************************************
		else if(nmhdr->code == TVN_BEGINDRAG || nmhdr->code == TVN_BEGINRDRAG)
		{
			bDraggedRMB = nmhdr->code == TVN_BEGINDRAG ? false : true;
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lparam;
			draggedItem = pnmtv->itemNew.hItem;
		}
// *****************************************************************************************************
		else if(nmhdr->code == TVN_DELETEITEM)
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lparam;
			tmpItemInfo = C_TreeViewCallback::S_ItemInfo(tmpStr.c_str(), (void*)pnmtv->itemOld.lParam, pnmtv->itemOld.hItem);
			pCallback->OnItemDeleted(m_Control.GetControlID(), tmpItemInfo);
		}
// *****************************************************************************************************
		else if(nmhdr->code == TVN_GETINFOTIP)
		{
		    LPNMTVGETINFOTIP lpGetInfoTip = (LPNMTVGETINFOTIP)lparam;
			T_InfoTips::iterator i(infoTips.find(lpGetInfoTip->hItem));
			if(i != infoTips.end())
			{
				lpGetInfoTip->pszText = (TCHAR*)i->second.c_str();
				lpGetInfoTip->cchTextMax = (int)i->second.size();
			}
		}
// *****************************************************************************************************
	}
// *****************************************************************************************************
// *****************************************************************************************************
	return result;
}

T_TreeItem C_TreeViewCtrl_Win32::MouseHitTest(const POINT& pt)
{
	TVHITTESTINFO hitTestInfo;
	hitTestInfo.flags = 0;
	hitTestInfo.pt = pt;
	hitTestInfo.hItem = 0;
	return (T_TreeItem)::SendMessage(m_Control, TVM_HITTEST, 0, (LPARAM)&hitTestInfo);
}

T_TreeItem C_TreeViewCtrl_Win32::MouseHitTest()
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(m_Control, &pt);
	return MouseHitTest(pt);
}

bool C_TreeViewCtrl_Win32::GetItemInfo(T_TreeItem item, std::string& str, void** pParam) const
{
	char buffer[::ae::maxStringSize];
	TVITEMEXA tvItem;
	memset(&tvItem, 0, sizeof(item));
	tvItem.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_PARAM;
	tvItem.pszText = buffer;
	tvItem.cchTextMax = ::ae::maxStringSize;
	tvItem.hItem = item;
	if(::SendMessageA(m_Control, TVM_GETITEMA, 0, (LPARAM)&tvItem))
	{
		str = tvItem.pszText ? tvItem.pszText : "";
		if(pParam) *pParam = (void*)tvItem.lParam;
		return true;
	}
	str = "";
	if(pParam) *pParam = 0;
	return false;
}

} } } // namespace ae { namespace windows_sys {  namespace win32 {

#endif // #if defined(AE_EDITOR) && (defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64))

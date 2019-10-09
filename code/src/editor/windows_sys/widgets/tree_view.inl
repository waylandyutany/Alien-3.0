namespace ae { namespace windows_sys {

	inline void C_TreeView::SetItemText(T_TreeItem item, const char* text) {
		control.SetItemText(item, text);
	}

	inline T_TreeItem C_TreeView::InsertItem(const char* string, void* param, T_TreeItem parent, T_TreeItem insertAfter) {
		return control.InsertItem(string, param, parent, insertAfter);
	}

	inline void C_TreeView::SortChildItems(T_TreeItem item) {
		control.SortChildItems(item);
	}

	inline void C_TreeView::RemoveItem(T_TreeItem item)	{
		control.RemoveItem(item);
	}

	inline void C_TreeView::BeginEditItem(T_TreeItem item) {
		control.BeginEditItem(item);
	}

	inline void C_TreeView::SelectItem(T_TreeItem item) {
		control.SelectItem(item);
	}

	inline void C_TreeView::EnsureVisibleItem(T_TreeItem item) {
		control.EnsureVisibleItem(item);
	}

	inline void C_TreeView::AddInfoTip(T_TreeItem item, const TCHAR* infoTip) {
		control.AddInfoTip(item, infoTip);
	}

	inline void C_TreeView::RemoveInfoTip(T_TreeItem item) {
		control.RemoveInfoTip(item);
	}

	inline void C_TreeView::RemoveAllInfoTips()	{
		control.RemoveAllInfoTips();
	}

	inline bool C_TreeView::GetItemInfo(T_TreeItem item, std::string& str, void** pParam) const {
		return control.GetItemInfo(item, str, pParam);
	}

} } // namespace ae { namespace windows_sys {

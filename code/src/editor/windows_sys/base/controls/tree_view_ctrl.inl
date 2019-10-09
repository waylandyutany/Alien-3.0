namespace ae { namespace windows_sys {

	inline void C_TreeViewCtrl::SetItemText(T_TreeItem item, const char* text) {
		m_Impl.SetItemText(item, text);
	}

	inline bool C_TreeViewCtrl::GetItemInfo(T_TreeItem item, std::string& str, void** pParam) const	{
		return m_Impl.GetItemInfo(item, str, pParam);
	}

	inline T_TreeItem C_TreeViewCtrl::InsertItem(const char* string, void* param, T_TreeItem parent, T_TreeItem insertAfter) {
		return m_Impl.InsertItem(string, param, parent, insertAfter);
	}

	inline void C_TreeViewCtrl::SortChildItems(T_TreeItem item)	{
		m_Impl.SortChildItems(item);
	}

	inline void C_TreeViewCtrl::RemoveItem(T_TreeItem item)	{
		m_Impl.RemoveItem(item);
	}

	inline void C_TreeViewCtrl::BeginEditItem(T_TreeItem item) {
		m_Impl.BeginEditItem(item);
	}

	inline void C_TreeViewCtrl::SelectItem(T_TreeItem item) {
		m_Impl.SelectItem(item);
	}

	inline void C_TreeViewCtrl::EnsureVisibleItem(T_TreeItem item) {
		m_Impl.EnsureVisibleItem(item);
	}

	// *****************************************************************************************************
	inline void C_TreeViewCtrl::AddInfoTip(T_TreeItem item, const TCHAR* infoTip) {
		m_Impl.AddInfoTip(item, infoTip);
	}

	inline void C_TreeViewCtrl::RemoveInfoTip(T_TreeItem item) {
		m_Impl.RemoveInfoTip(item);
	}

	inline void C_TreeViewCtrl::RemoveAllInfoTips()	{
		m_Impl.RemoveAllInfoTips();
	}

	// *****************************************************************************************************
	inline T_TreeItem C_TreeViewCtrl::GetFirstSelectedItem() {
		return m_Impl.GetFirstSelectedItem();
	}
	inline T_TreeItem C_TreeViewCtrl::GetNextSelectedItem(T_TreeItem item) {
		return m_Impl.GetNextSelectedItem(item);
	}
	inline T_TreeItem C_TreeViewCtrl::GetPreviousSelectedItem(T_TreeItem item) {
		return m_Impl.GetPreviousSelectedItem(item);
	}

} } // namespace ae { namespace windows_sys {

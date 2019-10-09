#pragma once
#if !defined(AE_WINDOW_GRID_LAYOUT_H) && defined(AE_EDITOR)
#define AE_WINDOW_GRID_LAYOUT_H

#include "layout.h"

namespace ae { namespace windows_sys {

	class C_GridLayout : public C_Layout
	{
		int m_Rows; // Horizontal
		int m_Columns; // Vertical
// *****************************************************************************************************
// *****************************************************************************************************
		struct S_CellInfo
		{
			int size;
			int stretch;
			S_CellInfo() : size(0), stretch(0) {}
		};
		S_CellInfo *m_ColumnsInfo, *m_RowsInfo;
// *****************************************************************************************************
// *****************************************************************************************************
		struct S_LayoutItem
		{
			int row, column, rowSpan, columnSpan;
			C_LayoutItem* pLayoutItem;
			S_LayoutItem(C_LayoutItem* _pLayoutItem, int _row, int _column, int _rowSpan, int _columnSpan) : row(_row), column(_column), rowSpan(_rowSpan), columnSpan(_columnSpan), pLayoutItem(_pLayoutItem) {}
		private:
			S_LayoutItem();
		};
		typedef std::deque<S_LayoutItem> T_LayoutItems;
		T_LayoutItems m_LayoutItems;
// *****************************************************************************************************
// *****************************************************************************************************
		void GetItemRect(int row, int column, int rowSpan, int columnSpan, const S_Rect& layoutRect, S_Rect& itemRect) const;
	public:
		C_GridLayout(int numRows, int numColumns);
		~C_GridLayout();
// *****************************************************************************************************
// *****************************************************************************************************
		void AddGridLayoutItem(C_LayoutItem*, int row, int column);
		void AddGridLayoutItem(C_LayoutItem*, int row, int column, int rowSpan, int columnSpan);
// *****************************************************************************************************
// *****************************************************************************************************
		void RemoveGridLayoutItem(C_LayoutItem*);
// *****************************************************************************************************
// *****************************************************************************************************
		void SetColumnMinSize(int column, int minSize);
		void SetColumnStretch(int column, int stretch);
		void SetRowMinSize(int row, int minSize);
		void SetRowStretch(int row, int stretch);
// *****************************************************************************************************
		void SetRowsMinSize(int size);
		void SetColumnsMinSize(int size);
// *****************************************************************************************************
		int GetColumnMinSize(int column) const;
		int GetColumnStretch(int column) const;
		int GetRowMinSize(int row) const;
		int GetRowStretch(int row) const;
// *****************************************************************************************************
// *****************************************************************************************************
		int GetRows() const { return m_Rows; }
		int GetColumns() const { return m_Columns; }
// *****************************************************************************************************
// *****************************************************************************************************
		virtual void SetLayoutRect(const S_Rect& layoutRect);
	};

} } // namespace ae { namespace windows_sys {

#endif // #if !defined(AE_WINDOW_GRID_LAYOUT_H) && defined(AE_EDITOR)

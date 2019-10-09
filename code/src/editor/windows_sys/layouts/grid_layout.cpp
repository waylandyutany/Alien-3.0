#include "stdafx.h"

#if defined(AE_EDITOR)

#include "grid_layout.h"
#include "../widgets/widget.h"

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
C_GridLayout::C_GridLayout(int numRows, int numColumns) :
	m_Rows(numRows), m_Columns(numColumns)
{
	m_ColumnsInfo = new S_CellInfo[numColumns];
	m_RowsInfo = new S_CellInfo[numRows];

	for(int i = 0; i < numRows; i++)
		m_RowsInfo[i].size = 24;

	for(int i = 0; i < numColumns; i++)
		m_ColumnsInfo[i].size = 96;
}

C_GridLayout::~C_GridLayout()
{
	delete [] m_ColumnsInfo;
	delete [] m_RowsInfo;
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_GridLayout::AddGridLayoutItem(C_LayoutItem* pLayoutItem, int row, int column )
{
	if(std::find_if(m_LayoutItems.begin(), m_LayoutItems.end(), [pLayoutItem](const S_LayoutItem& li) { return li.pLayoutItem == pLayoutItem; }) == m_LayoutItems.end())
		m_LayoutItems.push_back(S_LayoutItem(pLayoutItem, row, column, 1, 1));
}

void C_GridLayout::AddGridLayoutItem(C_LayoutItem* pLayoutItem, int row, int column, int rowSpan, int columnSpan)
{
	if(std::find_if(m_LayoutItems.begin(), m_LayoutItems.end(), [pLayoutItem](const S_LayoutItem& li) { return li.pLayoutItem == pLayoutItem; }) == m_LayoutItems.end())
		m_LayoutItems.push_back(S_LayoutItem(pLayoutItem, row, column, rowSpan, columnSpan));
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_GridLayout::RemoveGridLayoutItem(C_LayoutItem* pLayoutItem)
{
	m_LayoutItems.erase( std::remove_if(m_LayoutItems.begin(), m_LayoutItems.end(), [pLayoutItem](const S_LayoutItem& li) { return li.pLayoutItem == pLayoutItem; }));
}

// *****************************************************************************************************
// *****************************************************************************************************
void C_GridLayout::SetRowsMinSize(int size)
{
	for(int i = 0; i < m_Rows; i++)
		m_RowsInfo[i].size = size;
}

void C_GridLayout::SetColumnsMinSize(int size)
{
	for(int i = 0; i < m_Columns; i++)
		m_ColumnsInfo[i].size = size;
}

// *****************************************************************************************************
void C_GridLayout::SetColumnMinSize(int column, int minSize)
{
	AE_ASSERT(0 <= column && column < m_Columns && "Out of columns range !");
	m_ColumnsInfo[column].size = minSize;
}

int C_GridLayout::GetColumnMinSize(int column) const
{
	AE_ASSERT(0 <= column && column < m_Columns && "Out of columns range !");
	return m_ColumnsInfo[column].size;
}

void C_GridLayout::SetColumnStretch(int column, int stretch)
{
	AE_ASSERT(0 <= column && column < m_Columns && "Out of columns range !");
	m_ColumnsInfo[column].stretch = stretch;
}

int C_GridLayout::GetColumnStretch(int column) const
{
	AE_ASSERT(0 <= column && column < m_Columns && "Out of columns range !");
	return m_ColumnsInfo[column].stretch;
}

void C_GridLayout::SetRowMinSize(int row, int minSize)
{
	AE_ASSERT(0 <= row && row < m_Rows && "Out of rows range !");
	m_RowsInfo[row].size = minSize;
}

int C_GridLayout::GetRowMinSize(int row) const
{
	AE_ASSERT(0 <= row && row < m_Rows && "Out of rows range !");
	return m_RowsInfo[row].size;
}

void C_GridLayout::SetRowStretch(int row, int stretch)
{
	AE_ASSERT(0 <= row && row < m_Rows && "Out of rows range !");
	m_RowsInfo[row].stretch = stretch;
}

int C_GridLayout::GetRowStretch(int row) const
{
	AE_ASSERT(0 <= row && row < m_Rows && "Out of rows range !");
	return m_RowsInfo[row].stretch;
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_GridLayout::GetItemRect(int row, int column, int rowSpan, int columnSpan, const S_Rect& layoutRect, S_Rect& itemRect) const
{
	itemRect.left = layoutRect.left;
	for(int i=0; i < column; i++) itemRect.left += m_ColumnsInfo[i].size;
	itemRect.left += column * m_Spacing.width;

	itemRect.top = layoutRect.top;
	for(int i=0; i < row; i++) itemRect.top += m_RowsInfo[i].size;
	itemRect.top += row * m_Spacing.height;

	itemRect.right = itemRect.left;
	for(int i=column; i < column + columnSpan; i++) itemRect.right += m_ColumnsInfo[i].size;
	if(columnSpan > 0) itemRect.right += (columnSpan - 1)* m_Spacing.width;

	itemRect.bottom = itemRect.top;
	for(int i=row; i < row + rowSpan; i++) itemRect.bottom += m_RowsInfo[i].size;
	if(rowSpan > 0) itemRect.bottom += (rowSpan - 1) * m_Spacing.height;
}

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_GridLayout::SetLayoutRect(const S_Rect& rect)
{
	S_Rect layoutRect, itemRect;
	C_Layout::SetLayoutRect(rect);
	GetLayoutRect(layoutRect);

	for(T_LayoutItems::iterator i(m_LayoutItems.begin()); i != m_LayoutItems.end(); ++i)
	{
		GetItemRect((*i).row, (*i).column, (*i).rowSpan, (*i).columnSpan, layoutRect, itemRect);
		(*i).pLayoutItem->SetLayoutRect(itemRect);
	}
}

} } // namespace ae { namespace windows_sys {

#endif // #if defined(AE_EDITOR)

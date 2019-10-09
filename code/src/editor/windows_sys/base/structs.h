#pragma once
#if !defined(AE_STRUCTS_H) && defined(AE_EDITOR)
#define AE_STRUCTS_H

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
struct S_Rect
{
	S_Rect(int _left, int _top, int _right, int _bottom);
	S_Rect(int width, int height);
	S_Rect();

	int Width() const;
	int Height() const;
	void Inflate(int cx, int cy);
	void Move(int mx, int my);

	int left, right, top, bottom;
};

// *****************************************************************************************************
// *****************************************************************************************************
struct S_Point
{
	S_Point();
	S_Point(int _x, int _y);

	int x,y;
};

// *****************************************************************************************************
// *****************************************************************************************************
struct S_Size
{
	S_Size();
	S_Size(int _width, int _height);

	int width, height;
};

} } // namespace ae { namespace windows_sys {

#include "structs.inl"

#endif // #if !defined(AE_STRUCTS_H) && defined(AE_EDITOR)

namespace ae { namespace windows_sys {

// *****************************************************************************************************
// *****************************************************************************************************
inline S_Rect::S_Rect(int _left, int _top, int _right, int _bottom) : left(_left), right(_right), top(_top), bottom(_bottom) { }
inline S_Rect::S_Rect(int width, int height) : left(0), right(width), top(0), bottom(height) { }
inline S_Rect::S_Rect() : left(0), right(0), top(0), bottom(0) { }
inline int S_Rect::Width() const { return right - left; }
inline int S_Rect::Height() const {	return bottom - top; }
inline void S_Rect::Inflate(int cx, int cy) { top += cy; bottom	-= cy; left	+= cx; right -= cx; }
inline void S_Rect::Move(int mx, int my) { left += mx; right += mx; top += my; bottom += my; }

// *****************************************************************************************************
// *****************************************************************************************************
inline S_Point::S_Point() : x(0), y(0) { }
inline S_Point::S_Point(int _x, int _y) : x(_x), y(_y) { }

// *****************************************************************************************************
// *****************************************************************************************************
inline S_Size::S_Size() : height(0), width(0) {}
inline S_Size::S_Size(int _width, int _height) : height(_height), width(_width) {}

} } // namespace ae { namespace windows_sys {


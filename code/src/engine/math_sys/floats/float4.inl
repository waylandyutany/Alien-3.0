namespace ae { namespace math {

	template< typename T >
	template< typename U >
	inline vector4<T>::vector4(const vector3<U>& v3, float fw) : x(v3.x), y(v3.y), z(v3.z), w(fw) {}

} } // namespace ae { namespace math {

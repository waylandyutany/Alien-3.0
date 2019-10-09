#include "../../_sys/templates.h"

namespace ae { namespace scene {

// *****************************************************************************************************
// *****************************************************************************************************

template<typename T_Light>
inline C_Light<T_Light>::C_Light() {
	ae::ZeroStruct(&light);
}

template<typename T_Light>
inline void C_Light<T_Light>::SetColor(const ae::math::float3& color) {
	light.color.x = color.x;
	light.color.y = color.y;
	light.color.z = color.z;
}

template<typename T_Light>
inline void C_Light<T_Light>::GetColor(ae::math::float3& color) const {
	color = light.color;
}

// *****************************************************************************************************
// *****************************************************************************************************

inline void C_PointLight::SetPosition(const ae::math::float3& position) {
	light.posAndRadius.x = position.x;
	light.posAndRadius.y = position.y;
	light.posAndRadius.z = position.z;
}

inline void C_PointLight::GetPosition(ae::math::float3& position) const {
	position = light.posAndRadius;
}

inline void C_PointLight::SetRadius(float radius) {
	light.posAndRadius.w = radius;
}

inline float C_PointLight::GetRadius() const {
	return light.posAndRadius.w;
}

// *****************************************************************************************************
// *****************************************************************************************************
inline void C_DirectionalLight::SetDirection(const ae::math::float3& direction) {
	light.dir = ae::math::float4(direction.normalize(), 1);
}

inline void C_DirectionalLight::GetDirection(ae::math::float3& direction) const {
	direction = light.dir;
}

} } // namespace ae { namespace scene {

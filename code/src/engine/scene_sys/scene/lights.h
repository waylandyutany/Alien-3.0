#pragma once

#ifndef AE_SCENE_SYS_LIGHTS_H
#define AE_SCENE_SYS_LIGHTS_H

#include "../../math_sys/types.h"
#include "../../render_sys/render_scene/render_lights.h"

namespace ae { namespace scene {

	template<typename T_Light>
	class C_Light {
		friend class C_Scene;
	protected:
		C_Light();
		T_Light light;
	public:
		void SetColor(const ae::math::float3& color);
		void GetColor(ae::math::float3& color) const;
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_DirectionalLight : public C_Light<ae::render::S_DirLight> {
		friend class C_Scene;
		C_DirectionalLight() {};
	public:
		void SetDirection(const ae::math::float3& direction);
		void GetDirection(ae::math::float3& direction) const;
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_PointLight : public C_Light<ae::render::S_PointLight> {
		friend class C_Scene;
		C_PointLight() {};
	public:
		void SetPosition(const ae::math::float3& position);
		void GetPosition(ae::math::float3& position) const;
		void SetRadius(float radius);
		float GetRadius() const;
	};

	// *****************************************************************************************************
	// *****************************************************************************************************

} } // namespace ae { namespace scene {

#include "lights.inl"

#endif // #ifdef AE_SCENE_SYS_LIGHTS_H

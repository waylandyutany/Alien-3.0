#pragma once
#ifndef AE_PHYSIC_SYS_DEVICE_H
#define AE_PHYSIC_SYS_DEVICE_H

#ifdef AE_PHYSX
#include "../physx/device/device_physx.h"
#endif // #ifdef AE_PHYSX

#include "../../math_sys/types.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace physic {

	class C_Device
	{
		T_DeviceImpl m_Impl;
		friend class C_PhysicSys;
		void Update(float deltaTime);
	public:
		T_DeviceImpl& Impl() { return m_Impl; }
#ifdef AE_EDITOR
		bool BakeCollisionMesh(	ae::math::float3* verts, ae::u32 numV, ae::u32 vStride,
									ae::u32* ind, ae::u32 numI,
									ae::base::C_IncrementalAllocator& allocator, bool cookAsConvex  );
#endif // #ifdef AE_EDITOR
	};

} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_DEVICE_H

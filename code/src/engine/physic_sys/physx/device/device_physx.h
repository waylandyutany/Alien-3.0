#pragma once
#ifndef AE_PHYSIC_SYS_DEVICE_PHYSX_H
#define AE_PHYSIC_SYS_DEVICE_PHYSX_H

#include "../../../math_sys/types.h"

namespace ae { namespace physic {
	class C_Device;
} } // namespace ae { namespace physic {

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace physic { namespace physx {

	class C_Device
	{
		friend class ae::physic::C_Device;
		// *****************************************************************************************************
		::physx::PxFoundation* m_pFoundation;
		::physx::PxPhysics* m_pPhysics;
		::physx::PxProfileZoneManager* m_pPZManager;
		::physx::PxScene* m_pScene;
		::physx::PxDefaultCpuDispatcher* m_pCpuDispatcher;
//		::physx::pxtask::CudaContextManager* m_pCudeContextManager;
		const float m_fSimulationDeltaTime;
		float m_fSimTimeAccumulator;
		::physx::PxCooking* m_pCooking;
		// *****************************************************************************************************
#ifndef AE_RELEASE
		PVD::PvdConnection* m_pPVDConnection;
		void ConnectPVD();
		void DisconnectPVD();
#endif // #ifndef AE_RELEASE
		// *****************************************************************************************************
		C_Device();
		~C_Device();
		// *****************************************************************************************************
		void Update(float deltaTime);
		// *****************************************************************************************************
#ifdef AE_EDITOR
		Concurrency::critical_section m_CS;
		// This is thread safe, uses critical section to make sure that function doesnt run parallel
		bool BakeCollisionMesh(	ae::math::float3* verts, ae::u32 numV, ae::u32 vStride,
			ae::u32* ind, ae::u32 numI,
			ae::base::C_IncrementalAllocator& allocator, bool cookAsConvex );
#endif // #ifdef AE_EDITOR
		// *****************************************************************************************************
	public:
		::physx::PxCooking& Cooking() { return *m_pCooking; }
		::physx::PxPhysics& Physics() { return *m_pPhysics; }
		::physx::PxScene& Scene() { return *m_pScene; }
	};

} } } // namespace ae { namespace physic { namespace physx {

namespace ae { namespace physic {
	typedef physx::C_Device T_DeviceImpl;
} } // namespace ae { namespace physic {

#endif // #ifdef AE_PHYSIC_SYS_DEVICE_PHYSX_H

#pragma once
#ifndef AE_PHYSIC_SYS_UTILS_H
#define AE_PHYSIC_SYS_UTILS_H

#include "../../base_sys/memory/allocators/incremental_allocator.h"

namespace ae { namespace physic {
	struct S_Capsule;
	struct S_Cylinder;
	struct S_Box;
	struct S_Sphere;
} } // namespace ae { namespace physic {

namespace ae { namespace physic { namespace physx {

// *****************************************************************************************************
// *****************************************************************************************************
class C_InputStream : public ::physx::PxInputStream
{
	char *m_pPos, *m_pEnd;
public:
	C_InputStream(void* buffer, size_t size) : m_pPos((char*)buffer), m_pEnd(m_pPos + size)
	{ }

	virtual PxU32  read (void *dest, PxU32 count)
	{
		AE_ASSERT(m_pPos+count <= m_pEnd && "Reading out of buffer !");
		memcpy(dest, m_pPos, count);
		m_pPos += count;
		return count;
	}
};

// *****************************************************************************************************
// *****************************************************************************************************
class C_OutputStream : public ::physx::PxOutputStream
{
	NOCOPY_CLASS(C_OutputStream);
	ae::base::C_IncrementalAllocator& m_Allocator;
public:
	C_OutputStream(ae::base::C_IncrementalAllocator& allocator) : m_Allocator(allocator) {
	}

	virtual PxU32  write (const void *src, PxU32 count)
	{
		char* pBuffer = m_Allocator.Alloc(count);
		if(!pBuffer) return 0;
		memcpy(pBuffer, src, count);
		return count;
	}
};

// *****************************************************************************************************
// *****************************************************************************************************
void CapsuleToGeometry(const ae::physic::S_Capsule&, const ae::math::float3& pos, PxCapsuleGeometry&, PxTransform&);
void CylinderToGeometry(const ae::physic::S_Cylinder&, const ae::math::float3& pos, PxConvexMeshGeometry&, PxTransform&);
void BoxToGeometry(const ae::physic::S_Box&, const ae::math::float3& pos, PxBoxGeometry&, PxTransform&);
void SphereToGeometry(const ae::physic::S_Sphere&, const ae::math::float3& pos, PxSphereGeometry&, PxTransform&);
// *****************************************************************************************************
// *****************************************************************************************************
template<typename T_Object>
void SafeRelease(T_Object*&);
// *****************************************************************************************************
// *****************************************************************************************************

} } } // namespace ae { namespace physic { namespace physx {

#include "utils_physx.inl"

#endif // #ifdef AE_PHYSIC_SYS_UTILS_H

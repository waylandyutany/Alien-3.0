#include "StdAfx.h"

#include "device.h"
#include "../../debug_sys/output/outputs.h"

namespace ae { namespace physic {

void C_Device::Update(float deltaTime)
{
	m_Impl.Update(deltaTime);
}

#ifdef AE_EDITOR
bool C_Device::BakeCollisionMesh(	ae::math::float3* verts, ae::u32 numV, ae::u32 vStride,
	ae::u32* ind, ae::u32 numI,
	ae::base::C_IncrementalAllocator& allocator, bool cookAsConvex  )
{
	return m_Impl.BakeCollisionMesh(verts,numV,vStride, ind,numI, allocator, cookAsConvex);
}

#endif // #ifdef AE_EDITOR

} } // namespace ae { namespace physic {

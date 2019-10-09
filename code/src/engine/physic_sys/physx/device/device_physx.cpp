#include "StdAfx.h"

#include "device_physx.h"
#include "../utils_physx.h"

#include "../../scene/actor.h"
#include "../utils_physx.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../_sys/templates.h"
#include "../../../math_sys/types_dbg.h"

#ifdef AE_EDITOR
ae::base::C_IncrementalAllocator g_LargeAllocator;
__declspec( thread ) bool g_tlsUseLargeAllocator = false;
#endif // #ifdef EDITOR

namespace ae { namespace physic { namespace physx {

// *****************************************************************************************************
// *****************************************************************************************************
class C_AllocatorCallback : public ::physx::PxAllocatorCallback
{
	virtual void* allocate(size_t size, const char*,const char*,int)
	{
#ifdef AE_EDITOR
		if(g_tlsUseLargeAllocator)
			return g_LargeAllocator.Alloc(size, 16, false);
#endif // #ifdef EDITOR

		return _aligned_malloc(size, 16);
	}

	virtual void deallocate(void* ptr)
	{
#ifdef AE_EDITOR
		if(!g_tlsUseLargeAllocator)
#endif // #ifdef EDITOR
			_aligned_free(ptr);
	}
};

// *****************************************************************************************************
// *****************************************************************************************************

class C_ErrorCallback : public ::physx::PxErrorCallback
{
	virtual void reportError (::physx::PxErrorCode::Enum /*code*/, const char *message, const char *file, int line)
	{
		DBG_ERR << "PhysX '" << file << "'[" << line << "] : '" << message << "' !" << std::endl;
	}
};

// *****************************************************************************************************
// *****************************************************************************************************

class C_SimulationEventCallback : public ::physx::PxSimulationEventCallback
{
	virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count)
	{
	}

	virtual void onWake(PxActor **actors, PxU32 count)
	{
	}

	virtual void onSleep(PxActor **actors, PxU32 count)
	{
	}

	virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs)
	{
//		DBG_OUT << "virtual void onContact" << std::endl;

		for(ae::u32 i = 0; i < nbPairs; i++)
		{
			ae::physic::C_Actor* pActor0(static_cast<ae::physic::C_Actor*>(pairs[i].shapes[0]->getActor().userData));
			ae::physic::C_Actor* pActor1(static_cast<ae::physic::C_Actor*>(pairs[i].shapes[1]->getActor().userData));

			// todo remove 16 !
			PxContactPairPoint cps[16];
			ae::physic::S_Contact contacts[16];
			PxU32 cNum=pairs[i].extractContacts(cps, 16);

			for(PxU32 c = 0; c < cNum; c++)
			{
				memcpy(&contacts[c].normal, &cps[c].normal, sizeof(ae::math::float3));
				memcpy(&contacts[c].position, &cps[c].position, sizeof(ae::math::float3));
			}

			if(pActor0) pActor0->OnContact(pActor1, contacts, cNum);
			if(pActor1) pActor1->OnContact(pActor0, contacts, cNum);

		}

	}

	virtual void onTrigger(PxTriggerPair *pairs, PxU32 count)
	{
	}
};
// *****************************************************************************************************
// *****************************************************************************************************
static PxFilterFlags CustomFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
//	return PxDefaultSimulationFilterShader(attributes0, filterData0, attributes1, filterData1, pairFlags, constantBlock, constantBlockSize);

	pairFlags |= PxPairFlag::eCONTACT_DEFAULT;
	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;

	// CCD enabling
	pairFlags |= PxPairFlag::eRESOLVE_CONTACTS;
	pairFlags |= PxPairFlag::eSWEPT_INTEGRATION_LINEAR;

	return PxFilterFlag::eDEFAULT;
}
// *****************************************************************************************************
// *****************************************************************************************************
static C_ErrorCallback gErrorCallback;
static C_AllocatorCallback gAllocatorCallback;
static C_SimulationEventCallback gSimulationEventCallback;

// *****************************************************************************************************
// *****************************************************************************************************

C_Device::C_Device() :
	m_fSimulationDeltaTime(1.0f / 60.0f),
	m_fSimTimeAccumulator(0),
	m_pFoundation(0),
	m_pPhysics(0),
	m_pPZManager(0),
	m_pScene(0),
	m_pCpuDispatcher(0),
//	m_pCudeContextManager(),
	m_pCooking(0)
#ifndef AE_RELEASE
	,m_pPVDConnection(0)
#endif // #ifndef AE_RELEASE
{
#ifdef AE_EDITOR
	const size_t bakeMeshMemorySize(1024 * 1024 * 32);
	g_LargeAllocator.Create((char*)malloc(bakeMeshMemorySize), bakeMeshMemorySize);
#endif // #ifdef EDITOR

	std::stringstream sPhysXVersion;
	sPhysXVersion << "'PhysX " << PX_PHYSICS_VERSION_MAJOR << "." << PX_PHYSICS_VERSION_MINOR << "." << PX_PHYSICS_VERSION_BUGFIX << "'";

	DBG_OUT << sPhysXVersion.str() << " Physics system initializing..." << std::endl;

	// *****************************************************************************************************
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocatorCallback, gErrorCallback);
	if(!m_pFoundation)
		DBG_ERR << "Unable to create PhysX foundation !" << std::endl;
/*
	m_pPZManager = &PxProfileZoneManager::createProfileZoneManager(m_pFoundation);
	if(!m_pPZManager)
		DBG_ERR << "Unable to create PhysX Profile zone manager !" << std::endl;
*/
	// *****************************************************************************************************
	const bool bRecordMemoryAllocation = false;
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation,
		PxTolerancesScale(), bRecordMemoryAllocation, m_pPZManager);
	if(!m_pPhysics)
		DBG_ERR << "Unable to create PhysX Physics !" << std::endl;

//	if (!PxInitExtensions(*m_pPhysics))
//		DBG_ERR << "Unable to PxInitExtensions !" << std::endl;

	// *****************************************************************************************************
	m_pCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_pFoundation, PxCookingParams());
	if (!m_pCooking )
		DBG_ERR << "Unable to create PhysX Cooking !" << std::endl;

	// *****************************************************************************************************
	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());

	sceneDesc.flags |= PxSceneFlag::eENABLE_SWEPT_INTEGRATION; // enabling CCD, must be enabled inside CustomFilterShader and per created shape !
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	ae::u32 numThreads = 4; // todo get cpu cores
	m_pCpuDispatcher = PxDefaultCpuDispatcherCreate(numThreads);
	if(!m_pCpuDispatcher)
		DBG_ERR << "Unable to create PhysX Default CPU dispatcher !" << std::endl;
	sceneDesc.cpuDispatcher    = m_pCpuDispatcher;
	sceneDesc.filterShader = &CustomFilterShader;
	sceneDesc.simulationEventCallback = &gSimulationEventCallback;
	// *****************************************************************************************************
/*
#if defined(AE_PLATFORM_WIN32) || defined(AE_PLATFORM_WIN64)
	::physx::pxtask::CudaContextManagerDesc cudaContextManagerDesc;
	m_pCudeContextManager = ::physx::pxtask::createCudaContextManager(*m_pFoundation, cudaContextManagerDesc, 0); // 0 - no profiling zone manager @fix this
	if(m_pCudeContextManager)
		sceneDesc.gpuDispatcher = m_pCudeContextManager->getGpuDispatcher();
	if(sceneDesc.gpuDispatcher)
		DBG_OUT << sPhysXVersion.str() << " Enabling GPU acceleration." << std::endl;
#endif
*/
	// *****************************************************************************************************
	m_pScene = m_pPhysics->createScene(sceneDesc);
	if (!m_pScene)
		DBG_ERR << "Unable to create PhysX Scene !" << std::endl;
	// *****************************************************************************************************
	DBG_OUT << sPhysXVersion.str() << " Physics system initialized." << std::endl;

#ifndef AE_RELEASE
		ConnectPVD();
#endif // #ifndef AE_RELEASE
}

C_Device::~C_Device()
{
#ifndef AE_RELEASE
	DisconnectPVD();
#endif // #ifndef AE_RELEASE

	DBG_OUT << "'PhysX' Physics system releasing..." << std::endl;

	SafeRelease<>(m_pScene);
	SafeRelease<>(m_pCpuDispatcher);
//	SafeRelease<>(m_pCudeContextManager);
	SafeRelease<>(m_pCooking);
	SafeRelease<>(m_pPhysics);
	SafeRelease<>(m_pPZManager);
	SafeRelease<>(m_pFoundation);

	DBG_OUT << "'PhysX' Physics system released." << std::endl;

#ifdef AE_EDITOR
	::free(g_LargeAllocator.GetBottomPointer());
#endif // #ifdef EDITOR
}

#ifndef AE_RELEASE
void C_Device::ConnectPVD()
{
	DBG_OUT << "'PhysX' Connecting to PVD..." << std::endl;
	if(!m_pPhysics->getPvdConnectionManager())
		return;

	m_pPVDConnection = PxVisualDebuggerExt::createConnection(m_pPhysics->getPvdConnectionManager(),
		"127.0.0.1",5425, 100, PxVisualDebuggerExt::getAllConnectionFlags());
	if(m_pPVDConnection)
		DBG_OUT << "'PhysX' PVD connected." << std::endl;
	else
		DBG_ERR << "'PhysX' Unable connect to PVD!" << std::endl;
}

void C_Device::DisconnectPVD()
{
	if(m_pPVDConnection)
		DBG_OUT << "'PhysX' Disconnecting from PVD..." << std::endl;
	SafeRelease<>(m_pPVDConnection);
}
#endif // #ifndef AE_RELEASE

void C_Device::Update(float deltaTime)
{
	m_fSimTimeAccumulator += deltaTime;

	// Don't touch, otherwise it may crash
	while(m_fSimTimeAccumulator > m_fSimulationDeltaTime)
	{
		// this should be called at the frame begin
		m_pScene->simulate(m_fSimulationDeltaTime);

		m_fSimTimeAccumulator -= m_fSimulationDeltaTime;

		m_pScene->fetchResults(true);
	}
}

// *****************************************************************************************************
#ifdef AE_EDITOR

bool C_Device::BakeCollisionMesh(	ae::math::float3* verts, ae::u32 numV, ae::u32 vStride,
	ae::u32* ind, ae::u32 numI,
	ae::base::C_IncrementalAllocator& allocator, bool cookAsConvex )
{
	Concurrency::critical_section::scoped_lock slock(m_CS);

	if(cookAsConvex)
	{
		PxConvexMeshDesc convexDesc;
		convexDesc.points.count     = numV;
		convexDesc.points.stride    = sizeof(PxVec3);
		convexDesc.points.data      = verts;
		convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX;

		g_tlsUseLargeAllocator = true;
		g_LargeAllocator.Reset(true);
		C_OutputStream allocOutput(allocator);
		if(!m_pCooking->cookConvexMesh(convexDesc, allocOutput))
		{
			g_tlsUseLargeAllocator = false;
			return false;
		}

		g_tlsUseLargeAllocator = false;
	}
	else
	{
		::physx::PxTriangleMeshDesc meshDesc;
		meshDesc.convexEdgeThreshold = 0.0f;
		meshDesc.points.count           = numV;
		meshDesc.points.stride          = vStride;
		meshDesc.points.data            = verts;

		meshDesc.triangles.count        = numI / 3;
		meshDesc.triangles.stride       = 3*sizeof(ae::u32);
		meshDesc.triangles.data         = ind;

		meshDesc.convexEdgeThreshold = 0.0f;

		g_tlsUseLargeAllocator = true;
		g_LargeAllocator.Reset(true);
		C_OutputStream allocOutput(allocator);
		if(!m_pCooking->cookTriangleMesh(meshDesc, allocOutput))
		{
			g_tlsUseLargeAllocator = false;
			return false;
		}

		g_tlsUseLargeAllocator = false;
	}

	return true;
}
#endif // #ifdef AE_EDITOR

// *****************************************************************************************************

} } } // namespace ae { namespace physic { namespace physx {

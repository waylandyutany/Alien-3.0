#include "StdAfx.h"

#include "physic_sys.h"
#include "../debug_sys/output/outputs.h"

namespace ae { namespace physic {

// *****************************************************************************************************
C_PhysicSys* C_PhysicSys::pInstance = 0;
// *****************************************************************************************************
C_PhysicSys::C_PhysicSys(S_Init& /*sInit*/) :
	m_Resources(m_Device),
	m_Scene(m_Resources,m_Device)
{
}

C_PhysicSys::~C_PhysicSys()
{
}

// *****************************************************************************************************
bool C_PhysicSys::Create(S_Init& sInit)
{
	AE_ASSERT(!pInstance && "C_PhysicSys instance already created !");
	pInstance = new C_PhysicSys(sInit);
	return true;
}

void C_PhysicSys::Release()
{
	AE_ASSERT(pInstance && "Uninitialized C_PhysicSys class instance !");
	delete pInstance;
}

C_PhysicSys& C_PhysicSys::Instance()
{
	AE_ASSERT(pInstance && "Uninitialized C_PhysicSys class instance !");
	return *pInstance;
}
// *****************************************************************************************************
void C_PhysicSys::Update( float deltaTime )
{
	m_Device.Update(deltaTime);
}
// *****************************************************************************************************
} } // namespace ae { namespace physic {

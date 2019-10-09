#include "StdAfx.h"

#include "resource_sys.h"
#include "resources/resources_manager.h"

namespace ae { namespace resource {

C_ResourceSys* C_ResourceSys::m_pInstance = 0;

C_ResourceSys::C_ResourceSys(S_Init&sInit) :
	m_ResourceManager(sInit),
	m_PackagesManager(sInit)
{
}

C_ResourceSys::~C_ResourceSys()
{
}

void C_ResourceSys::Create(S_Init& sInit)
{
	AE_ASSERT(!m_pInstance && "C_ResourceSys instance already created !");
	m_pInstance = new C_ResourceSys(sInit);
}

void C_ResourceSys::Release()
{
	AE_ASSERT(m_pInstance && "Uninitialized C_ResourceSys instance !");
	delete m_pInstance;
}

C_ResourceSys& C_ResourceSys::Inst()
{
	AE_ASSERT(m_pInstance && "Uninitialized C_ResourceSys instance !");
	return *m_pInstance;
}

} } // namespace ae { namespace resource {

#include "StdAfx.h"

#include "entities.h"
#include "entity.h"

#include "../../engine/resource_sys/resources/resources_manager.h"
#include "../../engine/resource_sys/resource_sys.h"

namespace ag { namespace entities {

// *****************************************************************************************************
// *****************************************************************************************************
class C_EntityPackageNotifier : public ae::resource::I_PackageNotify
{
	virtual void OnPackageLoaded(const char* packageName)
	{
		C_Entities::Inst().OnPackageLoaded(packageName);
	}

	virtual void OnPackageUnloading(const char* packageName)
	{
		C_Entities::Inst().OnPackageUnloading(packageName);
	}
};

C_EntityPackageNotifier entityPackageNotifier;

// *****************************************************************************************************
// *****************************************************************************************************

C_Entities::C_Entities()
{
	ae::resource::C_ResourceSys::Inst().Resources().RegisterPackageNotifier(&entityPackageNotifier);
}

C_Entities::~C_Entities()
{
	// @crashfix @todo - fix it
//	ae::resource::C_ResourceSys::Inst().Resources().UnregisterPackageNotifier(&entityPackageNotifier);
}

C_Entities& C_Entities::Inst()
{
	static C_Entities inst;
	return inst;
}

void C_Entities::Update(float deltaTime)
{
	// Deleting dead entities
	T_Entities tmp;
	m_Entities.erase( std::remove_if(m_Entities.begin(), m_Entities.end(), [&tmp](I_Entity* p) -> bool
	{
		if(p->m_bDead)
		{
			tmp.push_back(p);
			return true;
		}
		return false;
	}
	), m_Entities.end() );

	if(!tmp.empty())
	{
		T_Entities::iterator i = tmp.begin();
		for(;i != tmp.end(); ++i)
			delete (*i);
	}

	// updating entities
	T_Entities::iterator i = m_Entities.begin();
	for(;i != m_Entities.end(); ++i)
		(*i)->OnUpdate(deltaTime);
}

void C_Entities::AfterUpdate()
{
	T_Entities::iterator i = m_Entities.begin();
	for(;i != m_Entities.end(); ++i)
		(*i)->OnAfterUpdate();
}

void C_Entities::AddEntity(I_Entity* pEntity)
{
	m_Entities.push_back(pEntity);
}

void C_Entities::RemoveEntity(I_Entity* pEntity)
{
	m_Entities.remove(pEntity);
}

void C_Entities::OnPackageUnloading(const char* packageName)
{
	std::for_each(m_Entities.begin(), m_Entities.end(), [packageName](I_Entity* pEntity) { pEntity->OnPackageUnloading(packageName); });
}

void C_Entities::OnPackageLoaded(const char* packageName)
{
	std::for_each(m_Entities.begin(), m_Entities.end(), [packageName](I_Entity* pEntity) { pEntity->OnPackageLoaded(packageName); });
}

} } // namespace ag { namespace entities {


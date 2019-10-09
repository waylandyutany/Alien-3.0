#include "StdAfx.h"

#include "entity.h"
#include "entities.h"
#include "../../../engine/scene_sys/scene_sys.h"

namespace ag { namespace entities {

I_Entity::I_Entity() : m_bDead(false)
{
	ag::entities::C_Entities::Inst().AddEntity(this);
}

/*virtual*/ I_Entity::~I_Entity()
{
	ag::entities::C_Entities::Inst().RemoveEntity(this);
}

} } // namespace ag { namespace entities {


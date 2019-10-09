#pragma once

#ifndef AG_ENTITIES_ENTITY_H
#define AG_ENTITIES_ENTITY_H

namespace ag { namespace entities {

	class C_Entities;

	class I_Entity
	{
		friend class C_Entities;
		bool m_bDead;
	protected:
		virtual ~I_Entity();
		I_Entity();

		virtual void OnPackageUnloading(const char* packageName) {}
		virtual void OnPackageLoaded(const char* packageName) {}

		virtual void OnUpdate(float deltaTime) {};
		virtual void OnAfterUpdate() {};
		void SetDead() { m_bDead = true; }
	};

} } // namespace ag { namespace entities {

#endif // #ifdef AG_ENTITIES_ENTITY_H

#pragma once
#ifndef AG_ENTITIES_H
#define AG_ENTITIES_H

namespace ag { namespace entities {

	class I_Entity;
	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Entities
	{
		C_Entities();
		~C_Entities();
		NOCOPY_CLASS(C_Entities);

		typedef std::list<I_Entity*> T_Entities;
		T_Entities m_Entities;

	public:
		void OnPackageUnloading(const char* packageName);
		void OnPackageLoaded(const char* packageName);
		void Update(float deltaTime);
		void AfterUpdate();
		void AddEntity(I_Entity*);
		void RemoveEntity(I_Entity*);

		static C_Entities& Inst();
	};
	// *****************************************************************************************************
	// *****************************************************************************************************

} } // namespace ag { namespace entities {

#endif // #ifdef AG_ENTITIES_H

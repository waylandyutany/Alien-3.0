#pragma once

#ifndef AE_RESOURCE_SYS_COLLISION_MESH_EXPORTER_H
#define AE_RESOURCE_SYS_COLLISION_MESH_EXPORTER_H

#include "collada_mesh_importer.h"
#include "mesh_exporter.h"

#include "../../../resource.h"
#include "../../../imports/imports.h"

namespace ae { namespace scene {
	struct S_CollisionResource;
	struct S_CollisionResource;
} } // namespace ae { namespace scene{

namespace ae { namespace resource {

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_CollisionMeshExporter : public I_MeshExporterCallback
	{
		bool m_bImportAsConvex;
		typedef C_Import<ae::resource::S_Resource> T_Import;
		virtual void OnExport(const char* resourceName, T_MeshResource&, T_INDBResource&, T_VTXBResource&, C_ColladaMeshImporter&);
		void ImportPointers(ae::scene::S_CollisionResource*, T_Import& import);
	public:
		C_CollisionMeshExporter(bool bImportAsConvex) : m_bImportAsConvex(bImportAsConvex) {}
	};
	// *****************************************************************************************************
	// *****************************************************************************************************

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_COLLISION_MESH_EXPORTER_H

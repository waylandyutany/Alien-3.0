#pragma once

#ifndef AE_RESOURCE_SYS_COLLADA_MESH_EXPORTER_H
#define AE_RESOURCE_SYS_COLLADA_MESH_EXPORTER_H

#include "collada_mesh_importer.h"

#include "../../../../render_sys/render_device/resources/resource_mesh.h"
#include "../../../../render_sys/render_device/resources/resource_vb.h"
#include "../../../../render_sys/render_device/resources/resource_ib.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace resource {

// *****************************************************************************************************
typedef ae::render::S_MeshResource T_MeshResource;
typedef ae::render::S_IndexBufferResource T_INDBResource;
typedef ae::render::S_VertexBufferResource T_VTXBResource;

class I_MeshExporterCallback
{
public:
	// On export is called after vertex/index buffer and mesh is initialized
	// You can implement your own export from valid mesh resources
	virtual void OnExport(const char* /*resourceName*/, T_MeshResource&, T_INDBResource&, T_VTXBResource&, C_ColladaMeshImporter&) {};
};
// *****************************************************************************************************

class C_Mesh_Exporter
{
	I_MeshExporterCallback* m_pMeshExporterCallback;
	C_ColladaMeshImporter& m_Importer;
	std::string m_sResourceName, m_sResourcePath;
	std::string m_sMeshResourceName,m_sIBResourceName, m_sVBResourceName;
	ae::base::C_FileSys::T_FileNames m_MergeFileNames;
	bool m_bForce32bIB;
	bool m_bForceFloat3VB;

	C_Mesh_Exporter();
	NOCOPY_CLASS(C_Mesh_Exporter);

	typedef std::pair<ae::u32, ae::u32> T_IndexPair;
	typedef std::map<ae::u32, ae::u32> T_IndexMap;

	T_MeshResource* rsrcMesh;

	void ExportVBIB(T_MeshResource*, const char* resourceName);

	typedef C_Import<ae::resource::S_Resource> T_Import;
	void ImportPointers(T_MeshResource*, T_Import&);

	inline ae::u32 MakeIndex(ae::u32 posIndex, ae::u32 uvIndex)
	{	return (static_cast<ae::u16>(posIndex) << 16) | static_cast<ae::u16>(uvIndex);	}

	inline void ConvertPos(ae::math::float3& pos)
	{ pos.z = 0.0f - pos.z; }

	inline void ConvertUV(ae::math::float2& uv)
	{ uv.y = 1.0f - uv.y;}

	template<typename T_Index>
	T_Index ProcessVertex(T_IndexMap& indexMap, C_ColladaMeshImporter::S_Geometry* iGeometry, ae::u32 pIndex, ae::u32 uvIndex, ae::u32& vertexIndex, ae::base::C_IncrementalAllocator&);

	::ae::render::S_MeshResource::S_Geometry::S_Triangles* FindTriangles(T_MeshResource* , const char* geometryName, const char* materialName);
	void Export();
	void ExportMaterialTextures(const C_ColladaMeshImporter::S_Material& iMaterial, ae::render::S_MeshResource::S_Material& eMaterial, T_Import&);
public:
	// If pMeshExporterCallback is given, you must provide your own export code, no resource and tmp textures are created, only tmp resources for mesh,index/vertex buffer are created
	C_Mesh_Exporter(const char* resourceName, C_ColladaMeshImporter&, I_MeshExporterCallback* pMeshExporterCallback, bool bForce32bIB, bool bForceFloat4VB);
	~C_Mesh_Exporter();
};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_COLLADA_MESH_EXPORTER_H

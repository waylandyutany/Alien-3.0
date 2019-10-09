#include "StdAfx.h"

#include "mesh_exporter.h"

#include "../../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../../debug_sys/output/outputs.h"
#include "../../../../math_sys/types_dbg.h"
#include "../../../../_sys/templates.h"

namespace ae { namespace resource {

// *********************************************************************************************************

C_Mesh_Exporter::C_Mesh_Exporter(const char* resourceName, C_ColladaMeshImporter& importer, I_MeshExporterCallback* pMeshExporterCallback, bool bForce32bIB, bool bForceFloat4VB) :
	m_pMeshExporterCallback(pMeshExporterCallback),
	m_sResourceName(resourceName),
	m_Importer(importer),
	rsrcMesh(0),
	m_bForce32bIB(bForce32bIB),
	m_bForceFloat3VB(bForceFloat4VB)
{
	std::string resourceID = ae::base::GetFileName(resourceName, false);
	std::string extension = ae::base::GetFileExtension(resourceName);
	std::string resourcePath = ae::base::ExtractFilePath(resourceName) + "/" + resourceID + "_" + extension + "/" + resourceID + ".";

	m_sResourcePath = ae::base::ExtractFilePath(resourceName) + "/" + resourceID  + "_" + extension +  "/";
	m_sMeshResourceName = resourcePath + ae::resource::ResourceType2String(resourceTypeMesh);
	m_sIBResourceName	= resourcePath + ae::resource::ResourceType2String(resourceTypeIB);
	m_sVBResourceName	= resourcePath + ae::resource::ResourceType2String(resourceTypeVB);

	m_MergeFileNames.push_back(m_sMeshResourceName);
	m_MergeFileNames.push_back(m_sIBResourceName);
	m_MergeFileNames.push_back(m_sVBResourceName);

	Export();
}

C_Mesh_Exporter::~C_Mesh_Exporter()
{
	if(!m_pMeshExporterCallback)
		ae::base::C_FileSys::Inst().MergeFiles(m_MergeFileNames, m_sResourceName.c_str());
}

void C_Mesh_Exporter::ExportMaterialTextures(const C_ColladaMeshImporter::S_Material& iMaterial, ae::render::S_MeshResource::S_Material& eMaterial, T_Import& import)
{
	if(m_pMeshExporterCallback)
		return;

	if(iMaterial.pDiffuseTexture)
	{
		std::string textureFileName = iMaterial.pDiffuseTexture->textureFile->textureFileName;
		std::string textureName = ae::base::GetFileName(textureFileName.c_str(), false);
		eMaterial.diffuseTexture.id = AllocString(import.allocator, textureName.c_str());
		eMaterial.diffuseTexture.idHash = ae::base::CalculateHashI32(eMaterial.diffuseTexture.id);

		import.AddPointer(&eMaterial.diffuseTexture.id);

		if(ae::base::C_FileSys::Inst().MakePathRelativeTo("/edit", textureFileName)) {
			std::string textureResourceFileName = m_sResourcePath + textureName + "." + ResourceType2String(resourceTypeTexture);
			ae::resource::C_ResourceSys::Inst().Imports().ImportFile(textureFileName.c_str(), textureResourceFileName.c_str(), resourceTypeTexture);
			m_MergeFileNames.push_back(textureResourceFileName);
		}
		else {
			DBG_ERR << "Unable to import texture '" << iMaterial.pDiffuseTexture->textureFile->textureFileName<< "' ! Store textures within '/edit' directory !" << std::endl;
		}
	}
}


void C_Mesh_Exporter::Export()
{
	auto import(BeginImport<S_Resource>(m_sMeshResourceName.c_str(), m_sMeshResourceName.c_str(), resourceTypeMesh, 1024 * 1024 * 4));
	auto rsrcMesh = reinterpret_cast<T_MeshResource*>(import.allocator.Alloc(sizeof(T_MeshResource)));

	// mesh materials
	C_ColladaMeshImporter::S_Material* iMaterial = m_Importer.m_pMaterials;
	while(iMaterial)
	{
		ae::render::S_MeshResource::S_Material* eMaterial = ae::CreateStruct<ae::render::S_MeshResource::S_Material>(rsrcMesh->materials, import.allocator);

		eMaterial->hid.id = AllocString(import.allocator, iMaterial->id);
		eMaterial->hid.idHash = ae::base::CalculateHashI32(iMaterial->id);
		eMaterial->diffuse = iMaterial->diffuse;
		
		ExportMaterialTextures(*iMaterial, *eMaterial, import);

		iMaterial = iMaterial->next;
	}

	// mesh geometries
	C_ColladaMeshImporter::S_Geometry* iGeometry = m_Importer.m_pGeometries;
	while(iGeometry)
	{
		ae::render::S_MeshResource::S_Geometry* eGeometry = ae::CreateStruct<ae::render::S_MeshResource::S_Geometry>(rsrcMesh->geometries, import.allocator);
		eGeometry->hid.id = AllocString(import.allocator, iGeometry->id);
		eGeometry->hid.idHash = ae::base::CalculateHashI32(iGeometry->id);

		eGeometry->boundingBox.min = iGeometry->minPosition;
		eGeometry->boundingBox.max = iGeometry->maxPosition;

		// Triangles
		C_ColladaMeshImporter::S_Geometry::S_Triangles* iTriangles = iGeometry->triangles;
		while(iTriangles)
		{
			ae::render::S_MeshResource::S_Geometry::S_Triangles* eTriangles = ae::CreateStruct<ae::render::S_MeshResource::S_Geometry::S_Triangles>(eGeometry->triangles, import.allocator);
			eTriangles->material = ae::FindStructHID<ae::render::S_MeshResource::S_Material>(iTriangles->material->id, rsrcMesh->materials);

			iTriangles = iTriangles->next;
		}

		iGeometry = iGeometry->next;
	}

	ExportVBIB(rsrcMesh, m_sResourceName.c_str());

	ImportPointers(rsrcMesh, import);
	EndImport<S_Resource>(import);
}

void C_Mesh_Exporter::ImportPointers(T_MeshResource* rsrcMesh, T_Import& import)
{
	ae::render::S_MeshResource::S_Material* eMaterial = rsrcMesh->materials;
	import.AddPointer(&rsrcMesh->materials);
	while(eMaterial)
	{
		import.AddPointer(&eMaterial->next);
		import.AddPointer(&eMaterial->hid.id);
		eMaterial = eMaterial->next;
	}
	// *********************************************************************************************************
	ae::render::S_MeshResource::S_Geometry* eGeometry = rsrcMesh->geometries;
	import.AddPointer(&rsrcMesh->geometries);
	while(eGeometry)
	{
		ae::render::S_MeshResource::S_Geometry::S_Triangles* eTriangles = eGeometry->triangles;
		import.AddPointer(&eGeometry->hid.id);
		import.AddPointer(&eGeometry->triangles);
		while(eTriangles)
		{
			import.AddPointer(&eTriangles->material);
			import.AddPointer(&eTriangles->next);
			eTriangles = eTriangles->next;
		}

		import.AddPointer(&eGeometry->next);
		eGeometry = eGeometry->next;
	}
}

ae::render::S_MeshResource::S_Geometry::S_Triangles* C_Mesh_Exporter::FindTriangles(T_MeshResource* rsrcMesh, const char* geometryName, const char* materialName)
{
	ae::render::S_MeshResource::S_Geometry* eGeometry = ae::FindStructHID<ae::render::S_MeshResource::S_Geometry>(geometryName, rsrcMesh->geometries);
	if(eGeometry)
	{
		ae::render::S_MeshResource::S_Geometry::S_Triangles* eTriangles = eGeometry->triangles;
		while(eTriangles)
		{
			if(!_stricmp(eTriangles->material->hid.id, materialName))
				return eTriangles;

			eTriangles = eTriangles->next;
		}
	}
	return 0;
}

template<typename T_Index>
T_Index C_Mesh_Exporter::ProcessVertex(T_IndexMap& indexMap, C_ColladaMeshImporter::S_Geometry* iGeometry, ae::u32 pIndex, ae::u32 uvIndex, ae::u32& vertexIndex, ae::base::C_IncrementalAllocator& allocator)
{
	//@todo for now it's stupid hack
	if(m_bForceFloat3VB)
		uvIndex = 0;

	ae::u32 k = MakeIndex(pIndex, uvIndex);
	T_IndexMap::iterator it = indexMap.find(k);
	ae::u32 i;
	if(it == indexMap.end())
	{
		T_VTXBResource::S_Vertex* newVertex = 0;
		if(m_bForceFloat3VB) {
			newVertex = reinterpret_cast<T_VTXBResource::S_Vertex*>(allocator.Alloc(sizeof(ae::math::float3)));
			newVertex->pos = iGeometry->positions[pIndex];
		}
		else {
			newVertex = reinterpret_cast<T_VTXBResource::S_Vertex*>(allocator.Alloc(sizeof(T_VTXBResource::S_Vertex)));
			newVertex->pos = iGeometry->positions[pIndex];
			newVertex->uv = iGeometry->UVs[uvIndex];
		}
		ConvertPos(newVertex->pos);
		ConvertUV(newVertex->uv);
		i = vertexIndex++;
		indexMap.insert(T_IndexPair(k, i));
	}
	else i = it->second;

	return (T_Index)i;
}

void C_Mesh_Exporter::ExportVBIB(T_MeshResource* rsrcMesh, const char* /*resourceName*/)
{
	ae::u32 totalIndicies = 0;
	bool bUse32BitIB = false;

	// calculate buffer size for index buffer
	C_ColladaMeshImporter::S_Geometry* iGeometry = m_Importer.m_pGeometries;
	while(iGeometry)
	{
		// if number of positions(verticies) is greater then 16bit value we must use 32bit index buffer !!!
		if(iGeometry->numPositions >= (1 << 16))
			bUse32BitIB = true;

		C_ColladaMeshImporter::S_Geometry::S_Triangles* iTriangles = iGeometry->triangles;
		while(iTriangles)
		{
			totalIndicies += iTriangles->numTrianglesIndicies * 3;
			iTriangles = iTriangles->next;
		}
		iGeometry = iGeometry->next;
	}

	if(m_bForce32bIB) bUse32BitIB = true;
	if(bUse32BitIB) DBG_OUT << "Using 32bit index buffer." << std::endl;
// *****************************************************************************************************
// *****************************************************************************************************
	// Creating index buffer resource first since we have it's exact size
	auto importIB(BeginImport<T_INDBResource>(m_sIBResourceName.c_str(), m_sIBResourceName.c_str(), resourceTypeIB, 1024 * 1024 * 4));
	T_INDBResource* rsrcINDB(importIB.pResource);
	rsrcINDB->m_IndexBufferType = bUse32BitIB ? C_IndexBufferResource::E_IBT_32Bit : C_IndexBufferResource::E_IBT_16Bit;
// *****************************************************************************************************
	const ae::u32 indiciesBuffSize = bUse32BitIB ? (totalIndicies * 4) : (totalIndicies * 2);
	ae::u16* indicies16 = bUse32BitIB ? 0 : reinterpret_cast<ae::u16*>(importIB.allocator.Alloc(indiciesBuffSize));
	ae::u32* indicies32 = bUse32BitIB ? reinterpret_cast<ae::u32*>(importIB.allocator.Alloc(indiciesBuffSize)) : 0;
// *****************************************************************************************************
// *****************************************************************************************************
	// Creating vertex buffer resource as last since we are going to grow it until last index processed
	auto importVB(BeginImport<T_VTXBResource>(m_sVBResourceName.c_str(), m_sVBResourceName.c_str(), resourceTypeVB, 1024 * 1024 * 4));
	T_VTXBResource* rsrcVTXB(importVB.pResource);
	rsrcVTXB->vertexFormat = T_VTXBResource::VF_POSITION | T_VTXBResource::VF_UV0;
	rsrcVTXB->vertextSize = sizeof(T_VTXBResource::S_Vertex);
// *********************************************************************************************************
// Iterating each triangles, filling vertex/index buffer and associating with object geometries structures
// *********************************************************************************************************
	T_IndexMap indexMap;
	ae::u32 vbStartIndex = 0, ibStartIndex = 0;
	ae::u32 vbIndex = 0, ibIndex = 0;

	iGeometry = m_Importer.m_pGeometries;
	while(iGeometry)
	{
		C_ColladaMeshImporter::S_Geometry::S_Triangles* iTriangles = iGeometry->triangles;
		while(iTriangles)
		{
			const ae::u32 numIndicies = iTriangles->numTrianglesIndicies;
			if(bUse32BitIB)
			{
				for(ae::u32 i = 0; i < numIndicies; i++)
				{
					indicies32[ibStartIndex + ibIndex++] = ProcessVertex<ae::u32>(indexMap, iGeometry, iTriangles->triangleIndicies[i].pi2, iTriangles->triangleIndicies[i].uvi2, vbIndex, importVB.allocator);
					indicies32[ibStartIndex + ibIndex++] = ProcessVertex<ae::u32>(indexMap, iGeometry, iTriangles->triangleIndicies[i].pi1, iTriangles->triangleIndicies[i].uvi1, vbIndex, importVB.allocator);
					indicies32[ibStartIndex + ibIndex++] = ProcessVertex<ae::u32>(indexMap, iGeometry, iTriangles->triangleIndicies[i].pi0, iTriangles->triangleIndicies[i].uvi0, vbIndex, importVB.allocator);
				}
			}
			else
			{
				for(ae::u32 i = 0; i < numIndicies; i++)
				{
					indicies16[ibStartIndex + ibIndex++] = ProcessVertex<ae::u16>(indexMap, iGeometry, iTriangles->triangleIndicies[i].pi2, iTriangles->triangleIndicies[i].uvi2, vbIndex, importVB.allocator);
					indicies16[ibStartIndex + ibIndex++] = ProcessVertex<ae::u16>(indexMap, iGeometry, iTriangles->triangleIndicies[i].pi1, iTriangles->triangleIndicies[i].uvi1, vbIndex, importVB.allocator);
					indicies16[ibStartIndex + ibIndex++] = ProcessVertex<ae::u16>(indexMap, iGeometry, iTriangles->triangleIndicies[i].pi0, iTriangles->triangleIndicies[i].uvi0, vbIndex, importVB.allocator);
				}
			}

			ae::render::S_MeshResource::S_Geometry::S_Triangles* eTriangles = FindTriangles(rsrcMesh, iGeometry->id, iTriangles->material->id);
			if(eTriangles)
			{
				eTriangles->indexCount = numIndicies * 3;
				eTriangles->indexOffset = ibStartIndex;

				eTriangles->vertexOffset = vbStartIndex;
			}

			ibStartIndex += ibIndex;
			ibIndex = 0;

			iTriangles = iTriangles->next;
		}

		vbStartIndex += vbIndex;
		vbIndex = 0;

		indexMap.clear();

		iGeometry = iGeometry->next;
	}

	rsrcVTXB->numVertices = vbStartIndex;
// *********************************************************************************************************
// *********************************************************************************************************
	if(m_pMeshExporterCallback)
		m_pMeshExporterCallback->OnExport(m_sResourceName.c_str(), *rsrcMesh, *rsrcINDB, *rsrcVTXB, m_Importer);
// *********************************************************************************************************
// *********************************************************************************************************
	EndImport<T_INDBResource>(importIB);
	EndImport<T_VTXBResource>(importVB);
}

} } // namespace ae { namespace resource {

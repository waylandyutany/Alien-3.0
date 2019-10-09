#pragma once

#ifndef AE_RESOURCE_SYS_COLLADA_MESH_IMPORTER_H
#define AE_RESOURCE_SYS_COLLADA_MESH_IMPORTER_H

#include "collada_parser.h"

#include "../../../../math_sys/types.h"

namespace ae { namespace base {
	class C_IncrementalAllocator;
} } // namespace ae { namespace base {

namespace ae { namespace resource {

class C_ColladaMeshImporter
{
public:
	struct S_TextureFile
	{
		const char* id;
		const char* textureFileName;

		S_TextureFile* next;
	};

	struct S_Texture
	{
		S_TextureFile* textureFile;
	};

	struct S_Material
	{
		const char* id;
		math::float4 emission;
		math::float4 ambient;
		math::float4 diffuse;
		math::float4 transparent;
		float transparency;
		S_Texture* pDiffuseTexture;

		S_Material* next;
	};

	struct S_Geometry
	{
		struct S_Triangles
		{
			struct S_TriangleIndicies
			{
				ae::u32 pi0,ni0,uvi0; // position, normal, uv index of first triangle vertex
				ae::u32 pi1,ni1,uvi1; // position, normal, uv index of second triangle vertex
				ae::u32 pi2,ni2,uvi2; // position, normal, uv index of third triangle vertex
			};

			S_Material* material;
			ae::u32 numTrianglesIndicies;
			S_TriangleIndicies* triangleIndicies;

			S_Triangles* next;
		};

		const char* id;

		ae::u32 numPositions;
		ae::u32 numNormals;
		ae::u32 numUVs;

		math::float3* positions;
		math::float3* normals;
		math::float2* UVs;

		math::float3 minPosition, maxPosition;

		S_Triangles* triangles;

		S_Geometry* next;

		void CalculateMinMaxPosition();
		void TransformToZeroCenter(); // Must be called after CalculateMinMaxPosition() !!!
	};

	struct S_Object
	{
		const char* id;
		math::float4x4 worldMatrix;
		S_Geometry* geometry;

		S_Object* next;
	};

	S_TextureFile* m_pTextureFiles;
	S_Material* m_pMaterials;
	S_Geometry* m_pGeometries;
	S_Object* m_pObjects;

	C_ColladaMeshImporter(C_ColladaParser::T_FileInputStream& input, ae::base::C_IncrementalAllocator& allocator);
	~C_ColladaMeshImporter();

	void PrintDebug();

private:
	void Import();
	C_ColladaParser m_Parser;
	ae::base::C_IncrementalAllocator& m_Allocator;

	S_Geometry* m_pLastUsedGeometry;

	void ImportTextureFile();
	void ImportMaterial();
	void ImportEffect();
	void ImportGeometry();
	void ImportGeometryTriangles(S_Geometry*);
	void ImportVisualScene();
	void ImportObject();

	void PrintTextureFilesDebug();
	void PrintMaterialsDebug();
	void PrintGeometriesDebug();
//	void PrintObjectsDebug();
};

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_COLLADA_MESH_IMPORTER_H

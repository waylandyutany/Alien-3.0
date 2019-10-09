#include "StdAfx.h"

#include "collada_mesh_importer.h"

#include "../../../../debug_sys/debug_sys.h"
#include "../../../../math_sys/types_dbg.h"
#include "../../../../base_sys/memory/allocators/incremental_allocator.h"

#include "../../../../_sys/templates.h"

namespace ae { namespace resource {

// 

void C_ColladaMeshImporter::S_Geometry::CalculateMinMaxPosition()
{
	minPosition = ae::math::float3(ae::math::max_float, ae::math::max_float, ae::math::max_float);
	maxPosition = ae::math::float3(-ae::math::max_float, -ae::math::max_float, -ae::math::max_float);

	for(ae::u32 i = 0; i < numPositions; i++) {
		minPosition.x = ae::math::Min<float>(minPosition.x, positions[i].x);
		minPosition.y = ae::math::Min<float>(minPosition.y, positions[i].y);
		minPosition.z = ae::math::Min<float>(minPosition.z, positions[i].z);

		maxPosition.x = ae::math::Max<float>(maxPosition.x, positions[i].x);
		maxPosition.y = ae::math::Max<float>(maxPosition.y, positions[i].y);
		maxPosition.z = ae::math::Max<float>(maxPosition.z, positions[i].z);
	}
}


void C_ColladaMeshImporter::S_Geometry::TransformToZeroCenter() // Must be called after CalculateMinMaxPosition() !!!
{
	ae::math::float3 centerOffset((minPosition + maxPosition) * 0.5f);
	for(ae::u32 i = 0; i < numPositions; i++) {
		positions[i] -= centerOffset;
	}
	minPosition -= centerOffset;
	maxPosition -= centerOffset;
}

// ************************************************************************************
// ************************************************************************************

C_ColladaMeshImporter::C_ColladaMeshImporter(C_ColladaParser::T_FileInputStream& input, ae::base::C_IncrementalAllocator& allocator) :
	m_Parser(input),
	m_Allocator(allocator),
	m_pTextureFiles(0),
	m_pMaterials(0),
	m_pGeometries(0),
	m_pObjects(0)
{
	Import();
}

C_ColladaMeshImporter::~C_ColladaMeshImporter()
{
}

void C_ColladaMeshImporter::PrintDebug()
{
	DBG_OUT << "Collada importer data" << std::endl;

	PrintTextureFilesDebug();
	PrintMaterialsDebug();
	PrintGeometriesDebug();
//	PrintObjectsDebug();
}

void C_ColladaMeshImporter::PrintTextureFilesDebug()
{
	DBG_OUT << "Texture files :" << std::endl;

	S_TextureFile* pTextureFile = m_pTextureFiles;
	while(pTextureFile)
	{
		DBG_OUT << "\t Texture file '" << pTextureFile->id << "' from file '" << pTextureFile->textureFileName << "'." << std::endl;
		pTextureFile = pTextureFile->next;
	}
}

void C_ColladaMeshImporter::PrintMaterialsDebug()
{
	DBG_OUT << "Materials :" << std::endl;
	DBG_OUT << std::setprecision(3);

	S_Material* pMaterial = m_pMaterials;
	while(pMaterial)
	{
		DBG_OUT << "\tMaterial : " << pMaterial->id << std::endl;
		DBG_OUT << "\t\tAmbient      : " << pMaterial->ambient << std::endl;
		DBG_OUT << "\t\tDiffuse      : " << pMaterial->diffuse << std::endl;
		if(pMaterial->pDiffuseTexture)
			DBG_OUT << "\t\t   Texture   : '" << pMaterial->pDiffuseTexture->textureFile->id << "'" << std::endl;

		DBG_OUT << "\t\tEmission     : " << pMaterial->emission << std::endl;
		DBG_OUT << "\t\tTransparent  : " << pMaterial->transparent << std::endl;
		DBG_OUT << "\t\tTransparency : " << pMaterial->transparency << std::endl;

		pMaterial = pMaterial->next;
	}

	DBG_OUT << std::setprecision(0);
}

void C_ColladaMeshImporter::PrintGeometriesDebug()
{
	DBG_OUT << "Geometries :" << std::endl;
	S_Geometry* pGeometry = m_pGeometries;
	while(pGeometry)
	{
		DBG_OUT << "\tGeometry : " << pGeometry->id << std::endl;

		S_Geometry::S_Triangles* pTriangles = pGeometry->triangles;
		while(pTriangles)
		{
			DBG_OUT << "\t\t " << pTriangles->numTrianglesIndicies << " trangles for '" << pTriangles->material->id << "' material" << std::endl;
			for(ae::u32 i = 0; i < pTriangles->numTrianglesIndicies; i++)
			{
				DBG_OUT << "\t\t\t" << ae::dbg::dec_fmt(4) << i << " : ";
				DBG_OUT << "[ " <<	ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].pi0 << " " <<
									ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].ni0 << " " << 
									ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].uvi0 << " ] ";

				DBG_OUT << "[ " <<	ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].pi1 << " " <<
									ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].ni1 << " " << 
									ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].uvi1 << " ] ";

				DBG_OUT << "[ " <<	ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].pi2 << " " <<
									ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].ni2 << " " << 
									ae::dbg::dec_fmt(4) <<pTriangles->triangleIndicies[i].uvi2 << " ]";

				DBG_OUT << std::endl;
			}

			pTriangles = pTriangles->next;
		}

		pGeometry = pGeometry->next;
	}
}
/*
void C_ColladaMeshImporter::PrintObjectsDebug()
{
	DBG_OUT << "Objects :" << std::endl;
	S_Object* pObject = m_pObjects;
	while(pObject)
	{
		DBG_OUT << "\tObject : " << pObject->id << std::endl;
		DBG_OUT << "\t\tGeometry   : " << pObject->geometry->id << std::endl;
		DBG_OUT << "\t\tMatrix     : " <<  std::endl;

		DBG_OUT << std::setprecision(3) << pObject->worldMatrix << std::setprecision(0) <<  std::endl;

		pObject = pObject->next;
	}
}
*/
// ************************************************************************************
// ************************************************************************************

void C_ColladaMeshImporter::Import()
{
	const char* nodeName = m_Parser.NextBeginNode();
	while(nodeName)
	{
		if(!_stricmp(nodeName, "image"))
			ImportTextureFile();
		if(!_stricmp(nodeName, "material"))
			ImportMaterial();
		else if(!_stricmp(nodeName, "effect"))
			ImportEffect();
		else if(!_stricmp(nodeName, "geometry"))
			ImportGeometry();
		else if(!_stricmp(nodeName, "library_visual_scenes"))
			ImportVisualScene();

		nodeName = m_Parser.NextBeginNode();
	}

	// CalculateMinMaxPosition for every geometry and transform positions to local center(0,0,0) 
	S_Geometry* pGeometry = m_pGeometries;
	while(pGeometry)
	{
		pGeometry->CalculateMinMaxPosition();
		pGeometry->TransformToZeroCenter();
		pGeometry = pGeometry->next;
	}
}

void C_ColladaMeshImporter::ImportTextureFile()
{
	m_Parser.GotoAttribute("id");
	S_TextureFile* pNewTextureFile = ae::CreateStruct<S_TextureFile>(m_pTextureFiles, m_Allocator);

	std::string textureID(m_Parser.GetStringValue());
	pNewTextureFile->id = AllocString(m_Allocator, textureID.c_str());

	m_Parser.GotoNode("init_from");
	std::string textureFileName = m_Parser.GetElementString();

	if(textureFileName.substr(0,7) == "file://")
		textureFileName = textureFileName.substr(7);

	pNewTextureFile->textureFileName = AllocString(m_Allocator, textureFileName.c_str());
}

void C_ColladaMeshImporter::ImportMaterial()
{
	m_Parser.GotoAttribute("name");

	S_Material* pNewMaterial = ae::CreateStruct<S_Material>(m_pMaterials, m_Allocator);

	std::string materialID(m_Parser.GetStringValue());
	pNewMaterial->id = AllocString(m_Allocator, materialID.c_str());
}

void C_ColladaMeshImporter::ImportEffect()
{
	std::string nextNode;

	m_Parser.GotoAttribute("name");
	S_Material* pMaterial = ae::FindStruct<S_Material>(m_Parser.GetStringValue(), m_pMaterials);

	// Emission color retrieving
	m_Parser.GotoNode("emission");
	m_Parser.GotoAttribute("sid");
	m_Parser.GetStringValue();
	m_Parser >> pMaterial->emission;

	// Ambient color retrieving
	m_Parser.GotoNode("ambient");
	nextNode = m_Parser.NextNode();
	if(nextNode == "color") {
		m_Parser.GotoAttribute("sid");
		m_Parser.GetStringValue();
		m_Parser >> pMaterial->ambient;
	} else if(nextNode == "texture") {
	}

	// Diffuse color retrieving
	m_Parser.GotoNode("diffuse");
	nextNode = m_Parser.NextNode();
	if(nextNode == "color") {
		m_Parser.GotoAttribute("sid");
		m_Parser.GetStringValue();
		m_Parser >> pMaterial->diffuse;
	} else if(nextNode == "texture") {
		pMaterial->diffuse = ae::math::float4(1,1,1,1);
		pMaterial->pDiffuseTexture = (S_Texture*)m_Allocator.Alloc(sizeof(S_Texture));
		m_Parser.GotoAttribute("texture");
		pMaterial->pDiffuseTexture->textureFile = ae::FindStruct<S_TextureFile>(m_Parser.GetStringValue(), m_pTextureFiles);
	}

	// Transparent color retrieving
	m_Parser.GotoNode("transparent");
	m_Parser.GotoAttribute("sid");
	m_Parser.GetStringValue();
	m_Parser >> pMaterial->transparent;

	// Transparency value retrieving
	m_Parser.GotoNode("transparency");
	m_Parser.GotoAttribute("sid");
	m_Parser.GetStringValue();
	pMaterial->transparency = m_Parser.GetFloatValue();
}

void C_ColladaMeshImporter::ImportGeometry()
{
	S_Geometry* pNewGeometry = ae::CreateStruct<S_Geometry>(m_pGeometries, m_Allocator);

	m_Parser.GotoAttribute("id");

	// removing "-lib" from geometry name
	std::string geometryID(m_Parser.GetStringValue());
	if(geometryID.find_last_of('-') == geometryID.size() - 4)
		geometryID = geometryID.substr(0, geometryID.size() - 4);
	// **********************************
	pNewGeometry->id = AllocString(m_Allocator, geometryID.c_str());

	// Positions 
	m_Parser.GotoNode("source");
	m_Parser.GotoAttribute("id");
	m_Parser.GotoNode("float_array");
	m_Parser.GotoAttribute("id");

	m_Parser.GotoAttribute("count");
	pNewGeometry->numPositions = m_Parser.GetUInt32Value() / 3;
	pNewGeometry->positions = reinterpret_cast<math::float3*>(m_Allocator.Alloc(pNewGeometry->numPositions * sizeof(math::float3),4,false));

	for(ae::u32 i = 0; i < pNewGeometry->numPositions; i++)
		m_Parser >> pNewGeometry->positions[i];

	// Normals
	m_Parser.GotoNode("source");
	m_Parser.GotoAttribute("id");

	m_Parser.GotoNode("float_array");
	m_Parser.GotoAttribute("id");
	m_Parser.GotoAttribute("count");
	pNewGeometry->numNormals = m_Parser.GetUInt32Value() / 3;
	pNewGeometry->normals = reinterpret_cast<math::float3*>(m_Allocator.Alloc(pNewGeometry->numNormals* sizeof(math::float3),4,false));

	for(ae::u32 i = 0; i < pNewGeometry->numNormals; i++)
		m_Parser >> pNewGeometry->normals[i];

	// UVs
	m_Parser.GotoNode("source");
	m_Parser.GotoAttribute("id");

	m_Parser.GotoNode("float_array");
	m_Parser.GotoAttribute("id");
	m_Parser.GotoAttribute("count");
	pNewGeometry->numUVs = m_Parser.GetUInt32Value() / 2;
	pNewGeometry->UVs = reinterpret_cast<math::float2*>(m_Allocator.Alloc(pNewGeometry->numUVs* sizeof(math::float2),4,false));

	for(ae::u32 i = 0; i < pNewGeometry->numUVs; i++)
		m_Parser >> pNewGeometry->UVs[i];

	// Triangles separated by materials
	const char* nodeName = m_Parser.NextBeginNode();
	while(nodeName)
	{
		if(!_stricmp(nodeName, "polygons"))
			ImportGeometryTriangles(pNewGeometry);
		else if(!_stricmp(nodeName, "library_visual_scenes"))
			ImportVisualScene();
		else if(!_stricmp(nodeName, "geometry"))
			ImportGeometry();

		nodeName = m_Parser.NextBeginNode();
	}
}

void C_ColladaMeshImporter::ImportGeometryTriangles(S_Geometry* pGeometry)
{
	S_Geometry::S_Triangles* triangles = ae::CreateStruct<S_Geometry::S_Triangles>(pGeometry->triangles, m_Allocator);

	m_Parser.GotoAttribute("material");
	triangles->material = ae::FindStruct<S_Material>(m_Parser.GetStringValue(), m_pMaterials);

	m_Parser.GotoAttribute("count");
	triangles->numTrianglesIndicies = m_Parser.GetUInt32Value();

	m_Parser.GotoNode("p");

	triangles->triangleIndicies =  reinterpret_cast<S_Geometry::S_Triangles::S_TriangleIndicies*>(m_Allocator.Alloc(triangles->numTrianglesIndicies* sizeof(S_Geometry::S_Triangles::S_TriangleIndicies),4,false));
	for(ae::u32 i = 0; i < triangles->numTrianglesIndicies; i++)
	{
		triangles->triangleIndicies[i].pi0 = m_Parser.GetUInt32Value();
		triangles->triangleIndicies[i].ni0 = m_Parser.GetUInt32Value();
		triangles->triangleIndicies[i].uvi0 = m_Parser.GetUInt32Value();

		triangles->triangleIndicies[i].pi1 = m_Parser.GetUInt32Value();
		triangles->triangleIndicies[i].ni1 = m_Parser.GetUInt32Value();
		triangles->triangleIndicies[i].uvi1 = m_Parser.GetUInt32Value();

		triangles->triangleIndicies[i].pi2 = m_Parser.GetUInt32Value();
		triangles->triangleIndicies[i].ni2 = m_Parser.GetUInt32Value();
		triangles->triangleIndicies[i].uvi2 = m_Parser.GetUInt32Value();
	}
}

void C_ColladaMeshImporter::ImportVisualScene()
{
	const char* nodeName = m_Parser.NextBeginNode();
	while(nodeName)
	{
		if(!_stricmp(nodeName, "node"))
			ImportObject();
		else if(!_stricmp(nodeName, "instance_visual_scene"))
		{
			// entire collada parsing is finished
			return;
		}
		nodeName = m_Parser.NextBeginNode();
	}
}

void C_ColladaMeshImporter::ImportObject()
{
	m_Parser.GotoAttribute("id");

	ae::math::float4x4 worldMatrix; 
	std::string idName;

	idName = m_Parser.GetStringValue();
	m_Parser.GotoNode("matrix");
	m_Parser.GotoAttribute("sid");
	m_Parser.GetStringValue();
	m_Parser >> worldMatrix;

	// Sometimes happend that node is part of another node
	// So when other nested node is detected we recursively import him instead of parent
	// Still not clean solution and needs to be solved in future !!!
	// @todo handle this later
	const char* nodeName = m_Parser.NextBeginNode();
	if(!_stricmp(nodeName, "node"))
	{
		ImportObject();
		return;
	}

	S_Object* pNewObject = ae::CreateStruct<S_Object>(m_pObjects, m_Allocator);
	pNewObject->id = AllocString(m_Allocator, idName.c_str());
	pNewObject->worldMatrix = worldMatrix;

	if(!_stricmp(nodeName, "instance_geometry"))
	{
		m_Parser.GotoAttribute("url");
		const char* geometryId = m_Parser.GetStringValue();
		m_pLastUsedGeometry = ae::FindStruct<S_Geometry>(++geometryId, m_pGeometries);
		pNewObject->geometry = m_pLastUsedGeometry;
	}
	else // if no instance geometry is found, prevous geometry is used
		pNewObject->geometry = m_pLastUsedGeometry;
}

} } // namespace ae { namespace resource {

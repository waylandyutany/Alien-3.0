#include "StdAfx.h"

#include "scene_prefabs.h"

namespace ae { namespace scene {

S_ScenePrefab_MGeometry* FindGeometryNode(S_ScenePrefabBase* pNode, ae::base::T_Hash32 geometryHash, ae::u32 numInstancies)
{
	if(!pNode) return 0;
	S_ScenePrefab_MGeometry* pRet(0);

	if(pNode->type == E_SINT_MGeometry) {
		S_ScenePrefab_MGeometry* pMGeo((S_ScenePrefab_MGeometry*)pNode);
		if(pMGeo->pGeometry->hid.idHash == geometryHash && pMGeo->numInstances == numInstancies)
			return pMGeo;
	}

	pRet = FindGeometryNode(pNode->child, geometryHash, numInstancies);
	if(pRet) return pRet;

	pNode = pNode->next;
	while(pNode)
	{
		pRet = FindGeometryNode(pNode, geometryHash, numInstancies);
		if(pRet) return pRet;
		pNode = pNode->next;
	}

	return 0;
}

S_ScenePrefab_MGeometry* FindGeometryNode(S_ScenePrefabBase* pNode, const char* geometryName, ae::u32 numInstancies)
{
	return FindGeometryNode(pNode, ae::base::CalculateHashI32(geometryName), numInstancies);
}

} } // namespace ae { namespace scene {

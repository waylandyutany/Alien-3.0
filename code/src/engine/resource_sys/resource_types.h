#pragma once

#ifndef AE_RESOURCE_SYS_RESOURCE_TYPES_H
#define AE_RESOURCE_SYS_RESOURCE_TYPES_H

namespace ae { namespace resource {

	typedef ae::u32 T_ResourceType;

	#define MAKE_RESOURCE_TYPE(c0,c1,c2,c3) ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)

	inline char Get_Rsc_Type_C0(T_ResourceType type) { return (type & 0xff); }
	inline char Get_Rsc_Type_C1(T_ResourceType type) { return ((type >> 8 ) & 0xff); }
	inline char Get_Rsc_Type_C2(T_ResourceType type) { return ((type >> 16) & 0xff); }
	inline char Get_Rsc_Type_C3(T_ResourceType type) { return ((type >> 24) & 0xff); }

	inline std::string ResourceType2String(T_ResourceType rscType)
	{
		std::string strType;
		strType += Get_Rsc_Type_C0(rscType);
		strType += Get_Rsc_Type_C1(rscType);
		strType += Get_Rsc_Type_C2(rscType);
		strType += Get_Rsc_Type_C3(rscType);
		return strType;
	}

	inline T_ResourceType String2ResourceType(const char* string)
	{
		if(!string[0] || !string[1] || !string[2] || !string[3]) return 0;
		return MAKE_RESOURCE_TYPE(string[0], string[1], string[2], string[3]);
	};

	static const T_ResourceType resourceTypeSound		= MAKE_RESOURCE_TYPE('s','n','d','r');
	static const T_ResourceType resourceTypeTexture		= MAKE_RESOURCE_TYPE('t','x','t','r');
	static const T_ResourceType resourceTypeEffect		= MAKE_RESOURCE_TYPE('e','f','f','e');
	static const T_ResourceType resourceTypeIB			= MAKE_RESOURCE_TYPE('i','n','d','b');
	static const T_ResourceType resourceTypeVB			= MAKE_RESOURCE_TYPE('v','t','x','b');

	static const T_ResourceType resourceTypeMesh		= MAKE_RESOURCE_TYPE('m','e','s','h'); // Mesh
		static const T_ResourceType elementTypeMGeo		= MAKE_RESOURCE_TYPE('m','g','e','o'); // Mesh geometry

	static const T_ResourceType resourceTypeCollision	= MAKE_RESOURCE_TYPE('c','o','l','l'); // Collision
		static const T_ResourceType elementTypeCGeo		= MAKE_RESOURCE_TYPE('c','g','e','o'); // Collision geometry

	static const T_ResourceType resourceTypeScene		= MAKE_RESOURCE_TYPE('s','c','e','n'); // Scene
		static const T_ResourceType resourceTypeSceneNode	= MAKE_RESOURCE_TYPE('s','c','n','d'); // Scene node

	static const T_ResourceType resourceTypePrefabs		= MAKE_RESOURCE_TYPE('p','f','b','s'); // Prefabs
		static const T_ResourceType resourceTypePrefab		= MAKE_RESOURCE_TYPE('p','r','f','b'); // Prefab

} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_RESOURCE_TYPES_H

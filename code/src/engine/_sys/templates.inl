namespace ae {

	template<typename T_Object>
	void SafeRelease(T_Object*& pObject)
	{
		if(pObject)
			pObject->Release();
		pObject = 0;
	}

	// ************************************************************************************
	template<typename T_Allocator>
	inline const char* AllocString(T_Allocator& allocator, const char* str)
	{
		const size_t ssize = strlen(str);
		char* sbuff = static_cast<char*>(allocator.Alloc(ssize + 1));
		strcpy_s(sbuff, ssize + 1, str);
		sbuff[ssize] = 0;
		return sbuff;
	}
	// ************************************************************************************
	template<typename T_Struct,typename T_Allocator>
	inline T_Struct* CreateStruct(T_Struct*& beginStruct, T_Allocator& allocator)
	{
		T_Struct* pNewStruct(reinterpret_cast<T_Struct*>(allocator.Alloc(sizeof(T_Struct))));
		if(!beginStruct)
			beginStruct = pNewStruct;
		else
		{
			T_Struct* pStruct(beginStruct);
			while(pStruct->next)
				pStruct = (T_Struct*)pStruct->next;

			pStruct->next = pNewStruct;
		}
		return pNewStruct;
	}
	// ************************************************************************************
	template<typename T>
	inline void AppendStruct(T* newStruct, T** structs)
	{
		newStruct->next = *structs;
		*structs = newStruct; 
	}
	// ************************************************************************************
	template<typename T_Struct>
	inline T_Struct* FindStruct(const char* id, T_Struct*& beginStruct)
	{
		T_Struct* pStruct(beginStruct);
		while(pStruct)
		{
			if(!_stricmp(id, pStruct->id))
				return pStruct;

			pStruct = pStruct->next;
		}

		return 0;
	}
	// ************************************************************************************
	template<typename T_Struct>
	inline T_Struct* FindStructHID(const char* id, T_Struct*& beginStruct)
	{
		ae::u32 idHash = ae::base::CalculateHashI32(id);

		T_Struct* pStruct(beginStruct);
		while(pStruct)
		{
			if(idHash == pStruct->hid.idHash)
				return pStruct;

			pStruct = pStruct->next;
		}

		return 0;
	}
	// ************************************************************************************
	template<typename T_Struct>
	inline void ZeroStruct(T_Struct* pStruct)
	{
		memset(pStruct,0,sizeof(T_Struct));
	}

	// ************************************************************************************
	template<typename T_Container>
	inline void Purge(T_Container& container)
	{
		auto it(container.begin());
		auto itEnd(container.end());
		for(;it != itEnd; ++it)
			delete (*it);

		container.erase(container.begin(), itEnd);
	}

	// ************************************************************************************
	template<typename T_Container, typename T_Element>
	inline void RemoveElement(T_Container& container, T_Element element) {
		container.erase(std::remove(container.begin(), container.end(), element));
	}

	// ************************************************************************************
	template<typename T_Array>
	inline size_t ArraySize(const T_Array& a) {
		REFERENCE(a);
		return sizeof(a) / sizeof(*a);
	}

} // namespace ae {


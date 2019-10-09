namespace ae { namespace physic { namespace physx {

	template<typename T_Object>
	inline void SafeRelease(T_Object*& pObject)
	{
		if(pObject)
			pObject->release();
		pObject = 0;
	}

} } } // namespace ae { namespace physic { namespace physx {

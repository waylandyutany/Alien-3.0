namespace ae { namespace resource {

template<typename T_ResourceType>
inline T_ResourceType* C_ResourcesManager::FindResource(const char* resourcePath)
{
	return static_cast<T_ResourceType*>(FindResource(resourcePath));
}

template<typename T_ElementType>
inline T_ElementType* C_ResourcesManager::FindElement(const char* elementPath)
{
	return static_cast<T_ElementType*>(FindElement(elementPath));
}

} } // namespace ae { namespace resource {

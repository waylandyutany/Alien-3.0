namespace ae { namespace resource {

inline void I_ResourceHandler::AssignResource(S_Resource* pBeginResource, S_Resource* pEndResource)
{
	m_pBeginResource = pBeginResource;
	m_pEndResource = pEndResource;
}

inline S_Resource* I_ResourceHandler::GetResource() {
	return m_pBeginResource;
}

inline S_Resource* I_ResourceHandler::GetEndResource() {
	return m_pEndResource;
}

} } // namespace ae { namespace resource {
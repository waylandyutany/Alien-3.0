namespace ae { namespace physic {

	inline I_ContactCallback* C_Actor::SetContactCB(I_ContactCallback* pContactCB)
	{
		I_ContactCallback* pRet(m_pContactCB);
		m_pContactCB = pContactCB;
		return pRet;
	}

} } // namespace ae { namespace physic {

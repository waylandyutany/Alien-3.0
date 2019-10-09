#include "StdAfx.h"

#include "actor.h"

namespace ae { namespace physic {

	void C_Actor::OnContact(C_Actor* pActor, const S_Contact* contacts, ae::u32 numContacts)
	{
		if(m_pContactCB) m_pContactCB->OnContact(pActor,contacts,numContacts);

		//	DBG_OUT << "onContact " << numContacts << std::endl;
	}

} } // namespace ae { namespace physic {

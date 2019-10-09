#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_OBJECT_IMPL_H
#define AE_AUDIO_SYS_AUDIO_OBJECT_IMPL_H

#include "../../base_sys/smart_ptr.h"

namespace ae { namespace audio {

	class C_AudioObjectImpl
	{
		static const ae::u32 cIsDead = 1;
	protected:
		ae::u32 m_Flags;
		C_AudioObjectImpl() : m_Flags(0)
		{}
	public:
		void SetDead();
		bool IsDead() const;
	};

	inline void C_AudioObjectImpl::SetDead() {
		m_Flags |= cIsDead;
	}

	inline bool C_AudioObjectImpl::IsDead() const {
		return m_Flags & cIsDead ? true : false;
	}
} } // namespace ae { namespace audio {

#endif // #ifndef AE_AUDIO_SYS_AUDIO_OBJECT_IMPL_H


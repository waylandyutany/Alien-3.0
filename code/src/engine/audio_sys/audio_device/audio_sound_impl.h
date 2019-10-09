#pragma once

#ifndef AE_AUDIO_SYS_AUDIO_SOUND_IMPL_H
#define AE_AUDIO_SYS_AUDIO_SOUND_IMPL_H

#include "audio_object_impl.h"
#include "sound_render_params.h"
#include "audio_data_info.h"

#include "../../base_sys/smart_ptr.h"
#include "../../base_sys/hash.h"
#include "../../math_sys/types.h"

namespace ae { namespace audio {

	class C_ListenerImpl;
	class C_ContextImpl;

	struct S_ListenerRenderParams;
	struct S_ContextRenderParams;

// *****************************************************************************************************
// *****************************************************************************************************
	class C_SoundImpl : public ae::base::C_RefCounter, public C_AudioObjectImpl
	{
		static const ae::u32 cIsPlaying = 2;
		static const ae::u32 cIsLooped = 4;
		static const ae::u32 cIsSeeked = 8;
// *****************************************************************************************************
		C_SoundImpl();
// *****************************************************************************************************
		void UpdateSamplePosStopped(float deltaTime);
		void UpdateSamplePosLooped(float deltaTime);
		void UpdateSamplePosNoLooped(float deltaTime);
		typedef void (C_SoundImpl::*T_UpdateSamplePos)(float deltaTime);
		T_UpdateSamplePos m_UpdateSamplePosFunc;
		ae::u32 m_SamplePos;
// *****************************************************************************************************
		ae::base::T_Hash32 m_AudioDataID;
		ae::u32 m_ActiveFrame;
		void* m_pRenderLink;
// *****************************************************************************************************
		float m_fVolume;
		float m_fFrequency;
	protected:
		S_SoundRenderParams m_RenderParams;
		S_AudioDataInfo m_AudioDataInfo;
		C_SoundImpl(ae::base::T_Hash32 audioDataID);
	public:
		void Update(float deltaTime);
// *****************************************************************************************************
		ae::base::T_Hash32 GetAudioDataID() const { return m_AudioDataID; }
// *****************************************************************************************************
		void SetRenderLink(void* renderLink) { m_pRenderLink = renderLink; }
		void* GetRenderLink() const { return m_pRenderLink; }
// *****************************************************************************************************
		void SetActiveFrame(ae::u32 activeFrame) { m_ActiveFrame = activeFrame; } 
		ae::u32 GetActiveFrame() const { return m_ActiveFrame; } 
// *****************************************************************************************************
		const S_SoundRenderParams& GetSoundRenderParams() const;
		const S_AudioDataInfo& GetAudioDataInfo() const;
// *****************************************************************************************************
		void Play(bool looped = false);
		void Stop();
		bool IsPlaying() const;
		bool IsLooped() const;
// *****************************************************************************************************
		void SeekSamplePos(ae::u32 samplePos);
		ae::u32 GetSamplePos() const;
		bool IsSeeked() const;
		void ClearSeek();
// *****************************************************************************************************
		void SetVolume(float volume);
		float GetVolume() const;
// *****************************************************************************************************
		void SetFrequency(float frequency);
		float GetFrequency() const;
// *****************************************************************************************************
		void SetLowPassFrequency(float frequency);
		float GetLowPassFrequency() const;
// *****************************************************************************************************
		void SetPosition(const ae::math::float3&);
		const ae::math::float3& GetPosition() const;
// *****************************************************************************************************
		void SetVelocity(const ae::math::float3&);
		const ae::math::float3& GetVelocity() const;
// *****************************************************************************************************
		void SetOrientation(const ae::math::float3&, const ae::math::float3&);
		void GetOrientation(ae::math::float3&, ae::math::float3&) const;
// *****************************************************************************************************
		virtual void UpdateSoundRenderParameters(const S_ContextRenderParams&, const S_ListenerRenderParams&);
	};
// *****************************************************************************************************
// *****************************************************************************************************
	typedef ae::base::C_SmartPtr<C_SoundImpl> T_SoundPtr;
	typedef std::deque<T_SoundPtr> T_Sounds;
// *****************************************************************************************************
// *****************************************************************************************************
	class C_3DSoundImpl : public C_SoundImpl
	{
	public:
		C_3DSoundImpl(ae::base::T_Hash32 audioDataID);
		virtual void UpdateSoundRenderParameters(const S_ContextRenderParams&, const S_ListenerRenderParams&);
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_AmbientSoundImpl : public C_SoundImpl
	{
	public:
		C_AmbientSoundImpl(ae::base::T_Hash32 audioDataID);
		virtual void UpdateSoundRenderParameters(const S_ContextRenderParams&, const S_ListenerRenderParams&);
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_SphereAmbientSoundImpl : public C_SoundImpl
	{
	public:
		C_SphereAmbientSoundImpl(ae::base::T_Hash32 audioDataID);
		virtual void UpdateSoundRenderParameters(const S_ContextRenderParams&, const S_ListenerRenderParams&);
	};
// *****************************************************************************************************
// *****************************************************************************************************
} } // namespace ae { namespace audio {

#include "audio_sound_impl.inl"

#endif // #ifdef AE_AUDIO_SYS_AUDIO_SOUND_IMPL_H

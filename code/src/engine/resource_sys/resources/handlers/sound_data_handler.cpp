#include "StdAfx.h"

#include "sound_data_handler.h"
#include "../resource_handler.h"

#include "../../../debug_sys/output/outputs.h"

#include "../../../audio_sys/audio_device/resources/resource_sound_data.h"
#include "../../../audio_sys/audio_device/audio_resources.h"
#include "../../../audio_sys/audio_codecs/wav/audio_codec_wav.h"

#include "../../../audio_sys/audio_sys.h"

using ae::audio::S_SoundDataResource;

namespace ae { namespace resource {

C_SoundDataResourceHandler::C_SoundDataResourceHandler() :
	I_ResourceHandler()
{
}

void C_SoundDataResourceHandler::Activate()
{
	I_ResourceHandler::Activate();

	S_SoundDataResource* pSoundDataResource = static_cast<S_SoundDataResource*>(GetResource());

	ae::audio::C_AudioSys::Instance().Resources().RegisterSoundDataResource(pSoundDataResource, pSoundDataResource->hid.idHash);

//	PrintDebug();
}

void C_SoundDataResourceHandler::Deactivate()
{
	I_ResourceHandler::Deactivate();

	S_SoundDataResource* pSoundDataResource = static_cast<S_SoundDataResource*>(GetResource());

	ae::audio::C_AudioSys::Instance().Resources().UnregisterSoundDataResource(pSoundDataResource, pSoundDataResource->hid.idHash);
}

void C_SoundDataResourceHandler::PrintDebug()
{
	S_SoundDataResource* pSoundDataResource = static_cast<S_SoundDataResource*>(GetResource());
	I_ResourceHandler::PrintDebug();

	switch(pSoundDataResource->audioDataInfo.format)
	{
	case ae::audio::E_AF_WAV :
		DBG_OUT << "WAV sound data resource." << std::endl;
		break;
	case ae::audio::E_AF_FLAC :
		DBG_OUT << "FLAC sound data resource." << std::endl;
		break;
	case ae::audio::E_AF_OGG  :
		DBG_OUT << "OGG sound data resource." << std::endl;
		break;
	}

	DBG_OUT << "Channels    : " << pSoundDataResource->audioDataInfo.numChannels << std::endl;
	DBG_OUT << "Samples     : " << pSoundDataResource->audioDataInfo.numSamples << std::endl;
	DBG_OUT << "BPS         : " << pSoundDataResource->audioDataInfo.numBitesPerSampel << std::endl;
	DBG_OUT << "Sample rate : " << pSoundDataResource->audioDataInfo.sampleRate << std::endl;
}

} } // namespace ae { namespace resource {

#include "StdAfx.h"

#include "sound_data_importer.h"

#include "../../../_sys/templates.h"

#include "../../../base_sys/file_system/file_system.h"
#include "../../../base_sys/memory/allocators/incremental_allocator.h"
#include "../../../debug_sys/output/outputs.h"

#include "../../../audio_sys/audio_device/resources/resource_sound_data.h"
#include "../../../base_sys/streams/memory_seekable_input_stream.h"

#include "../../../audio_sys/audio_codecs/wav/audio_codec_wav.h"
#include "../../../audio_sys/audio_codecs/flac/audio_codec_flac.h"
#include "../../../audio_sys/audio_codecs/ogg/audio_codec_ogg.h"

using ae::audio::S_SoundDataResource;

namespace ae { namespace resource {

C_SoundDataImporter::C_SoundDataImporter()
{
}

bool C_SoundDataImporter::Import(const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*)
{
	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
	ae::base::C_FileSys::S_MappedFile* pMappedFile = fileSys.OpenMappedFile(fileToImport);
	if(!pMappedFile) return false;
// *****************************************************************************************************
	const size_t resourceSize = sizeof(S_SoundDataResource) + pMappedFile->Size() + 1024;
	auto import(BeginImport<S_SoundDataResource>(resourceToExport, resourceToExport, ae::resource::resourceTypeSound, resourceSize));
	S_SoundDataResource* rscSoundData = import.pResource;
// *****************************************************************************************************
	std::string resourceExt = ae::base::GetFileExtension(fileToImport);
	ae::base::C_MemorySeekableInputStream memoryStream(pMappedFile->Buffer<char*>(), pMappedFile->Size());
#ifdef AE_AUDIO_WAV_CODEC
	if(!_stricmp(resourceExt.c_str(), "wav"))
	{
		ae::audio::C_AudioCodecWAV audioCodecWav(memoryStream);
		audioCodecWav.GetCodecInfo(rscSoundData->audioDataInfo);
	}
#endif // #ifdef AE_AUDIO_WAV_CODEC
#ifdef AE_AUDIO_FLAC_CODEC
	if(!_stricmp(resourceExt.c_str(), "flac"))
	{
		ae::audio::C_AudioCodecFLAC audioCodecFLAC(memoryStream);
		audioCodecFLAC.GetCodecInfo(rscSoundData->audioDataInfo);
	}
#endif // #ifdef AE_AUDIO_FLAC_CODEC
#ifdef AE_AUDIO_OGG_CODEC
	if(!_stricmp(resourceExt.c_str(), "ogg"))
	{
		ae::audio::C_AudioCodecOGG audioCodecOGG(memoryStream);
		audioCodecOGG.GetCodecInfo(rscSoundData->audioDataInfo);
	}
#endif // #ifdef AE_AUDIO_OGG_CODEC

	memcpy(import.allocator.Alloc(pMappedFile->Size()), pMappedFile->Buffer<char*>(), pMappedFile->Size());
	EndImport<S_SoundDataResource>(import);
// *****************************************************************************************************
	fileSys.CloseMappedFile(pMappedFile);
	return true;
}

} } // namespace ae { namespace resource {

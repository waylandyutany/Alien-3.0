#include "StdAfx.h"

#include "effect_importer.h"

#include "../../../render_sys/render_device/resources/resource_effect.h"
#include "../../../render_sys/render_device/dx11/utils_dx11.h"

#include "../../../debug_sys/output/outputs.h"
#include "../../../_sys/templates.h"
#include "../../../_sys/strings.h"

namespace ae { namespace resource {

	namespace shader {

		class C_ShaderInclude : public ID3D10Include
		{
			std::string m_IncludeDir;
		public:
			C_ShaderInclude(const char* includeDir);

			HRESULT __stdcall Open( D3D10_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes );
			HRESULT __stdcall Close( LPCVOID pData );
		};

		C_ShaderInclude::C_ShaderInclude(const char* includeDir) :
			m_IncludeDir(includeDir)
		{
		}

		HRESULT __stdcall C_ShaderInclude::Open( D3D10_INCLUDE_TYPE /*IncludeType*/, LPCSTR pFileName, LPCVOID /*pParentData*/, LPCVOID *ppData, UINT *pBytes )
		{
			ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
			std::string headerFile = m_IncludeDir + "/" + pFileName;

			*pBytes = (UINT)fileSys.GetFileSize(headerFile.c_str());
			*ppData = ::malloc(*pBytes);
			fileSys.ReadFile(headerFile.c_str(), reinterpret_cast<char*>(const_cast<void*>(*ppData)), *pBytes );

//			if(IncludeType == D3D10_INCLUDE_LOCAL)
//			else if(IncludeType == D3D10_INCLUDE_SYSTEM)

			return S_OK;
		}

		HRESULT __stdcall C_ShaderInclude::Close( LPCVOID pData )
		{
			::free(const_cast<void*>(pData));
			return S_OK;
		}

		struct S_CompiledShader
		{
			ID3D10Blob* errorBuffer;
			ID3D10Blob* shaderBuffer;

			S_CompiledShader() : errorBuffer(0), shaderBuffer(0) {}
			~S_CompiledShader() {
				ae::SafeRelease(errorBuffer);
				ae::SafeRelease(shaderBuffer);
			}
		};

	} // namespace shader {

bool C_EffectImporter::Import( const char* fileToImport, const char* resourceToExport, const ae::base::S_XMLNode*)
{
	ae::base::C_FileSys& fileSys(ae::base::C_FileSys::Inst());
	ae::base::C_FileSys::S_MappedFile* pMappedFile = fileSys.OpenMappedFile(fileToImport);
	if(!pMappedFile) return false;
// *****************************************************************************************************
// Compiling all possible shaders to buffers
// *****************************************************************************************************
	size_t compiledShadersSize = 0;
	shader::S_CompiledShader compiledShaders[6];

	bool bErrorReported = false;
	compiledShadersSize += CompileShader(pMappedFile->Buffer<char*>(), pMappedFile->Size(), "vs_5_0", "MainVS", fileToImport, compiledShaders[0], bErrorReported);
	compiledShadersSize += CompileShader(pMappedFile->Buffer<char*>(), pMappedFile->Size(), "gs_5_0", "MainGS", fileToImport, compiledShaders[1], bErrorReported);
	compiledShadersSize += CompileShader(pMappedFile->Buffer<char*>(), pMappedFile->Size(), "ps_5_0", "MainPS", fileToImport, compiledShaders[2], bErrorReported);
	compiledShadersSize += CompileShader(pMappedFile->Buffer<char*>(), pMappedFile->Size(), "cs_5_0", "MainCS", fileToImport, compiledShaders[3], bErrorReported);
	compiledShadersSize += CompileShader(pMappedFile->Buffer<char*>(), pMappedFile->Size(), "ds_5_0", "MainDS", fileToImport, compiledShaders[4], bErrorReported);
	compiledShadersSize += CompileShader(pMappedFile->Buffer<char*>(), pMappedFile->Size(), "hs_5_0", "MainHS", fileToImport, compiledShaders[5], bErrorReported);

	if(compiledShadersSize)
	{
		// *****************************************************************************************************
		const size_t resourceSize = compiledShadersSize + sizeof(ae::render::S_EffectResource) + (1024 * 4);
		auto import(BeginImport<ae::render::S_EffectResource>(resourceToExport, resourceToExport, ae::resource::resourceTypeEffect,resourceSize));
		ae::render::S_EffectResource* rscEffect = import.pResource;
		// *****************************************************************************************************
		for(int i = 0; i < 6; i++)
		{
			if(!compiledShaders[i].shaderBuffer) continue;
			rscEffect->m_ShaderBuffers[i].bufferSize = compiledShaders[i].shaderBuffer->GetBufferSize();
			if(rscEffect->m_ShaderBuffers[i].bufferSize)
			{
				rscEffect->m_ShaderBuffers[i].pBuffer = import.allocator.Alloc(rscEffect->m_ShaderBuffers[i].bufferSize);
				import.AddPointer(&rscEffect->m_ShaderBuffers[i].pBuffer);
				memcpy(rscEffect->m_ShaderBuffers[i].pBuffer, compiledShaders[i].shaderBuffer->GetBufferPointer(), compiledShaders[i].shaderBuffer->GetBufferSize());
			}
		}
		// *****************************************************************************************************
		EndImport<ae::render::S_EffectResource>(import);

		return true;
	}
	fileSys.CloseMappedFile(pMappedFile);
	return false;
}

size_t C_EffectImporter::CompileShader(char* buff, size_t size, const char* profile, const char* funcName, const char* fileName, shader::S_CompiledShader& compiledShader, bool& bErrorReported)
{
	shader::C_ShaderInclude shaderInclude(ae::base::ExtractFilePath(fileName).c_str());
	HRESULT result;
	result = D3DX11CompileFromMemory(buff, size, fileName, NULL, &shaderInclude, funcName, profile, D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &compiledShader.shaderBuffer, &compiledShader.errorBuffer, NULL);
	if(result != S_OK)
	{
		char* pErrStr = (char*)compiledShader.errorBuffer->GetBufferPointer();

		// Ignoring error X3501 missing entry function
		if(ae::SearchStringBuffer(pErrStr, compiledShader.errorBuffer->GetBufferSize(), "X3501") != -1)
			return 0; // this is not error so we return true

		if(!bErrorReported)
		{
			for(size_t i = 0; i < compiledShader.errorBuffer->GetBufferSize(); i++)
				DBG_ERR << pErrStr[i];
			DBG_ERR << std::endl;
			bErrorReported = true;
		}

		return 0;
	}
	return compiledShader.shaderBuffer->GetBufferSize();
}

} } // namespace ae { namespace resource {

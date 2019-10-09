#include "StdAfx.h"

#include "file_system.h"

#include "../../debug_sys/output/outputs.h"

namespace ae { namespace base {

void ValidateFilePath(std::string& pth);

size_t GetFileSize(const char* fileName)
{
	WIN32_FIND_DATAA ffd;
	HANDLE hFind;
	hFind = FindFirstFileA(fileName, &ffd);
	if(hFind == INVALID_HANDLE_VALUE)
		return 0;

	size_t fileSize = 0;
#ifdef AE_32BIT
		fileSize = ffd.nFileSizeLow;
#else
		fileSize = ((size_t)ffd.nFileSizeHigh << 32) | (size_t)ffd.nFileSizeLow;
#endif

	FindClose(hFind);
	return fileSize;
}

bool C_FileSys::GetFileTimes(const char* fileName, ae::u64* timeCreation, ae::u64* timeWriting, ae::u64* timeReading)
{
	std::string filePath(GetFilePath(fileName));
	WIN32_FIND_DATAA ffd;
	HANDLE hFind;
	hFind = FindFirstFileA(filePath.c_str(), &ffd);
	if(hFind == INVALID_HANDLE_VALUE)
		return false;

	if(timeCreation)
		*timeCreation = ((ae::u64)ffd.ftCreationTime.dwHighDateTime << 32) | (ae::u64)ffd.ftCreationTime.dwLowDateTime;
	if(timeWriting)
		*timeWriting = ((ae::u64)ffd.ftLastWriteTime.dwHighDateTime << 32) | (ae::u64)ffd.ftLastWriteTime.dwLowDateTime;
	if(timeReading)
		*timeReading = ((ae::u64)ffd.ftLastAccessTime.dwHighDateTime << 32) | (ae::u64)ffd.ftLastAccessTime.dwLowDateTime;

	FindClose(hFind);
	return true;
}

bool C_FileSys::SetFileTimes(const char* fileName, const ae::u64* timeCreation, const ae::u64* timeWriting, const ae::u64* timeReading)
{
	std::string filePath(GetFilePath(fileName));
	HANDLE hFile = ::CreateFileA(filePath.c_str(), FILE_WRITE_ATTRIBUTES | FILE_READ_ATTRIBUTES, 0, 0, OPEN_EXISTING, 0, 0);
	if(hFile == INVALID_HANDLE_VALUE) return false;

	BOOL bRet = true;
	FILETIME creationTime, lastAccessTime, lastWriteTime;
	bRet = ::GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime);

	if(timeCreation)
	{
		creationTime.dwHighDateTime	=  (*timeCreation >> 32) & 0xffffffff;
		creationTime.dwLowDateTime	= *timeCreation & 0xffffffff;
	}

	if(timeWriting)
	{
		lastWriteTime.dwHighDateTime	=  (*timeWriting >> 32) & 0xffffffff;
		lastWriteTime.dwLowDateTime		= *timeWriting & 0xffffffff;
	}

	if(timeReading)
	{
		lastAccessTime.dwHighDateTime	=  (*timeReading >> 32) & 0xffffffff;
		lastAccessTime.dwLowDateTime	= *timeReading & 0xffffffff;
	}

	bRet = ::SetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime);

	CloseHandle(hFile);

	return bRet ? true : false;
}

void C_FileSys::EnumFiles(const char* path, const char* enumMask, I_FileEnumerator& fe)
{
	std::string dirName(path);
	dirName = dirName.substr(dirName.find_first_of('/',1));
	std::string mountedPath(GetMountedPath(path));
	std::string fullMaskPath = mountedPath + dirName + "/" + enumMask;

	WIN32_FIND_DATAA ffd;
	HANDLE hFind;
	hFind = FindFirstFileA(fullMaskPath.c_str(), &ffd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do {
			if(!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string fileName = ffd.cFileName;
				std::string filePath(path);
				filePath += "/" + fileName;
				fe.OnFile(fileName.c_str(), filePath.c_str());
			}
		} while(FindNextFileA(hFind, &ffd));
	}
	FindClose(hFind);
}

bool PathExist(const char* path)
{
	WIN32_FIND_DATAA ffd;
	HANDLE hFind;
	hFind = FindFirstFileA(path, &ffd);
	if(hFind == INVALID_HANDLE_VALUE)
		return false;
	FindClose(hFind);
	return true;
}

bool FileExist(const char* path)
{
	return PathExist(path);
}

// *****************************************************************************************************

std::string GetCurrentDirectory()
{
	char sDirectory[::ae::maxFilePathSize];
	::GetCurrentDirectoryA(::ae::maxFilePathSize, sDirectory);
	std::string directory(sDirectory);
	ValidateFilePath(directory);
	return directory;
}

void SetCurrentDirectory(const char* directory)
{
	::SetCurrentDirectoryA(directory);
}

void CreateDirectory(const char* directory)
{
	::CreateDirectoryA(directory,0);
}

void DeleteFile(const char* fileName)
{
	::DeleteFileA(fileName);
}

void RenameFile(const char* oldFileName, const char* newFileName)
{
	::MoveFileA(oldFileName, newFileName);
}

// *****************************************************************************************************
C_FileSys::S_MappedFile* C_FileSys::OpenMappedFile(const char* fileName) const 
{
	std::string filePath(GetFilePath(fileName));
	HANDLE hFile = ::CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE) return 0;

	size_t size = ae::base::GetFileSize(filePath.c_str());
	DWORD dwMaximumSizeHigh = 0;
	DWORD dwMaximumSizeLow = size & 0xffffffff;

	HANDLE hMappedFile = ::CreateFileMapping(hFile, 0, PAGE_READONLY, dwMaximumSizeHigh, dwMaximumSizeLow, 0);
	if(hMappedFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return 0;
	}

	void* buffer = ::MapViewOfFile(hMappedFile, FILE_MAP_READ, 0, 0, size);
	if(!buffer)
	{
		CloseHandle(hMappedFile);
		CloseHandle(hFile);
		return 0;
	}

	S_MappedFile* pMappedFile = new C_FileSys::S_MappedFile();
	pMappedFile->size = size;
	pMappedFile->buffer = buffer;
	pMappedFile->handles[0] = (void*)hFile;
	pMappedFile->handles[1] = (void*)hMappedFile;
	pMappedFile->bReadOnly = true;

	return pMappedFile;
}

C_FileSys::S_MappedFile* C_FileSys::CreateMappedFile(const char* fileName, size_t size, bool bAssureFile)
{
	std::string filePath(GetFilePath(fileName));
	if(bAssureFile) AssureFile(filePath.c_str());
	HANDLE hFile = ::CreateFileA(filePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE) return 0;

	DWORD dwMaximumSizeHigh = 0;
	DWORD dwMaximumSizeLow = size & 0xffffffff;

	HANDLE hMappedFile = ::CreateFileMapping(hFile, 0, PAGE_READWRITE, dwMaximumSizeHigh, dwMaximumSizeLow, 0);
	if(hMappedFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hFile);
		return 0;
	}

	void* buffer = ::MapViewOfFile(hMappedFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, size);
	if(!buffer)
	{
		CloseHandle(hMappedFile);
		CloseHandle(hFile);
		return 0;
	}

	S_MappedFile* pMappedFile = new C_FileSys::S_MappedFile();
	pMappedFile->size = size;
	pMappedFile->buffer = buffer;
	pMappedFile->handles[0] = (void*)hFile;
	pMappedFile->handles[1] = (void*)hMappedFile;
	pMappedFile->bReadOnly = false;

	return pMappedFile;
}

void C_FileSys::CloseMappedFile(S_MappedFile* pMappedFile, size_t size)
{
	::UnmapViewOfFile(pMappedFile->buffer);
	::CloseHandle((HANDLE)pMappedFile->handles[1]);
	if(size && !pMappedFile->bReadOnly) { // adjust mapped file size
		LONG sizeLow(size & 0xffffffff);
		LONG sizeHigh = 0;
#ifdef AE_64BIT
		sizeHigh = (size >> 32);
#endif // #ifdef AE_64BIT
		::SetFilePointer((HANDLE)pMappedFile->handles[0], sizeLow, &sizeHigh, FILE_BEGIN);
		::SetEndOfFile((HANDLE)pMappedFile->handles[0]);
	}
	::CloseHandle((HANDLE)pMappedFile->handles[0]);
	delete pMappedFile;
}

} } // namespace ae { namespace base {

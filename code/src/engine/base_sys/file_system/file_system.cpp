#include "StdAfx.h"

#include "file_system.h"

#include "../streams/file_input_stream.h"
#include "../../debug_sys/output/outputs.h"

namespace ae { namespace base {

// *****************************************************************************************************
// *****************************************************************************************************
std::string GetCurrentDirectory();
void SetCurrentDirectory(const char* directory);
void CreateDirectory(const char* directory);
void DeleteFile(const char* fileName);
void RenameFile(const char* oldFileName, const char* newFileName);
bool FileExist(const char* path);
bool PathExist(const char* path);
size_t GetFileSize(const char* fileName);
size_t ReadFile(const char* fileName, char* buff, size_t size);
size_t WriteFile(const char* fileName, char* buff, size_t size);
size_t AppendFile(const char* fileName, char* buff, size_t size);
void ValidateFilePath(std::string& pth);

// *****************************************************************************************************
// *****************************************************************************************************

C_FileSys& C_FileSys::Inst()
{
	static C_FileSys instance;
	return instance;
}

void C_FileSys::Mount(const char* path, const char* mntName)
{
	S_Mount mount;

	mount.path = path;
	mount.fullPath = path;
	mount.name = mntName;

	ValidateFilePath(mount.name);
	ValidateFilePath(mount.path);
	ValidateFilePath(mount.fullPath);

	// Counting how many times we use "../"
	int parentDirCount = 0;
	while(!_strnicmp(mount.fullPath.c_str() + (parentDirCount*3), "../", 3))
		parentDirCount++;

	// replacing ../ with current directory
	if(parentDirCount)
	{
		T_Strings directories;
		SeparateDirectoriesFromPath(GetCurrentDirectory(), directories);
		std::string pth;
		for(size_t i = 0; i < directories.size()-parentDirCount; i++)
			pth += directories[i] + "/";
		mount.fullPath.replace(0, parentDirCount*3, pth);
	}


	m_Mounts.push_back(mount);

	DBG_OUT << "Mounting '" << path << "' as '" << mntName << "'" << std::endl;
}

bool C_FileSys::MakePathRelativeTo(const char* mntName, std::string& path)
{
	ValidateFilePath(path);
	T_Mounts::iterator i = std::find_if(m_Mounts.begin(), m_Mounts.end(), [mntName](S_Mount& s) { return s.name == mntName;} );
	if(i == m_Mounts.end()) return false;


	if(_strnicmp(path.c_str(), i->fullPath.c_str(), i->fullPath.size())) return false;
	std::string spath = path.substr(i->fullPath.size());
	path = mntName + spath;
	return true;
}

// *****************************************************************************************************
// *****************************************************************************************************


std::string C_FileSys::GetMountedPath(const char* path) const
{
	std::string p(path);
	std::string mntName = p.substr(0,p.find_first_of('/',1));

	T_Mounts::const_iterator it = m_Mounts.begin();
	for(; it != m_Mounts.end(); ++it)
	{
		if(it->name == mntName)
			return it->path;
	}

	DBG_ERR << "Unable to find path for '" << path << "' !" << std::endl;
	return "";
}

std::string C_FileSys::GetFilePath(const char* path) const
{
	std::string relativePath(path);
	ValidateFilePath(relativePath);
	relativePath = relativePath.substr(relativePath.find_first_of('/',1));

	std::string mountedPath(GetMountedPath(path));

	return mountedPath + relativePath;
}
// *****************************************************************************************************
// *****************************************************************************************************

size_t C_FileSys::GetFileSize(const char* fileName)
{
	std::string filePath(GetFilePath(fileName));
	return ae::base::GetFileSize(filePath.c_str());
}

size_t C_FileSys::ReadFile(const char* fileName, char* buff, size_t size)
{
	std::string filePath(GetFilePath(fileName));
	return ae::base::ReadFile(filePath.c_str(), buff, size);
}

size_t C_FileSys::WriteFile(const char* fileName, char* buff, size_t size, bool bAssureFile)
{
	std::string filePath(GetFilePath(fileName));
	if(bAssureFile)	AssureFile(filePath.c_str());
	return ae::base::WriteFile(filePath.c_str(), buff, size);
}

size_t C_FileSys::AppendFile(const char* fileName, char* buff, size_t size, bool bAssureFile)
{
	std::string filePath(GetFilePath(fileName));
	if(bAssureFile)	AssureFile(filePath.c_str());
	return ae::base::AppendFile(filePath.c_str(), buff, size);
}

I_InputStream* C_FileSys::CreateInputStream(const char* fileName)
{
	std::string filePath(GetFilePath(fileName));
	if(!ae::base::FileExist(filePath.c_str())) return 0;
	return new C_FileInputStream(filePath.c_str());
}

std::ostream* C_FileSys::CreateOutputStream(const char* fileName, bool bAssureFile)
{
	std::string filePath(GetFilePath(fileName));
	if(bAssureFile)	AssureFile(filePath.c_str());
	return new std::ofstream(filePath.c_str(), std::ios::out);
}

bool C_FileSys::AssureFile(const char* fileName)
{
	std::string path(::ae::base::ExtractFilePath(fileName));
	if(!ae::base::PathExist(path.c_str()))
	{
		T_Strings directories;
		SeparateDirectoriesFromPath(path, directories);
		AssureDirectories(directories);
		return true;
	}
	return true;
}

// *****************************************************************************************************
void C_FileSys::DeleteFile(const char* fileName)
{
	std::string filePath(GetFilePath(fileName));
	ae::base::DeleteFile(filePath.c_str());
}

void C_FileSys::RenameFile(const char* oldFileName, const char* newFileName)
{
	std::string oldFilePath(GetFilePath(oldFileName));
	std::string newFilePath(GetFilePath(newFileName));
	ae::base::RenameFile(oldFilePath.c_str(), newFilePath.c_str());
}

bool C_FileSys::FileExist(const char* path)
{
	std::string filePath(GetFilePath(path));
	return ae::base::FileExist(filePath.c_str());
}

bool C_FileSys::PathExist(const char* path)
{
	std::string filePath(GetFilePath(path));
	return ae::base::PathExist(filePath.c_str());
}


// *****************************************************************************************************
bool C_FileSys::AssureDirectories(const T_Strings& directories)
{
	std::string currentDirectory(GetCurrentDirectory());
	std::string directory(currentDirectory);

	for(size_t i = 0; i < directories.size(); i++)
	{
		directory += "/";
		directory += directories[i];
		if(!::ae::base::PathExist(directory.c_str()))
			::ae::base::CreateDirectory(directory.c_str());
	}

	return true;
}

void C_FileSys::SeparateDirectoriesFromPath(const std::string& path, T_Strings& directories)
{
	size_t startPos = 0;
	while(startPos != std::string::npos)
	{
		size_t p = path.find_first_of('/', startPos);
		if(p == std::string::npos)
		{
			directories.push_back(path.substr(startPos));
			break;
		}

		directories.push_back(path.substr(startPos, p - startPos));
		startPos = p + 1;
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

void C_FileSys::MergeFiles(const T_FileNames& fileNames, const char* mergedFileName)
{
	size_t fileSize = 0;
	for(size_t i = 0; i < fileNames.size();i++)
		fileSize += GetFileSize(fileNames[i].c_str());

	S_MappedFile* mergeFile = CreateMappedFile(mergedFileName, fileSize, true);
	if(!mergeFile) return;
	size_t offset = 0;
	for(size_t i = 0; i < fileNames.size();i++)
	{
		S_MappedFile* pMappedFile = OpenMappedFile(fileNames[i].c_str());
		memcpy(mergeFile->Buffer<char*>() + offset, pMappedFile->Buffer<char*>(), pMappedFile->Size());
		offset += pMappedFile->Size();
		CloseMappedFile(pMappedFile);
	}

	CloseMappedFile(mergeFile);
}

// *****************************************************************************************************
// *****************************************************************************************************

size_t ReadFile(const char* fileName, char* buff, size_t size)
{
	std::FILE *f;
	if(fopen_s(&f, fileName,"rb") != 0)
		return 0;

	size_t readed = fread(buff, 1, size, f);
	fclose(f);

	return readed;
}

size_t WriteFile(const char* fileName, char* buff, size_t size)
{
	std::FILE *f;
	if(fopen_s(&f, fileName,"wb") != 0)
		return 0;

	size_t written = fwrite(buff, 1, size, f);
	fclose(f);

	return written;
}

size_t AppendFile(const char* fileName, char* buff, size_t size)
{
	std::FILE *f;
	if(fopen_s(&f, fileName,"a+b") != 0)
		return 0;

	size_t written = fwrite(buff, 1, size, f);
	fclose(f);

	return written;
}

std::string GetFileExtension(const char* fileName)
{
	std::string s(fileName);
	size_t p = s.find_last_of('.');
	if(p != std::string::npos)
		return &fileName[p+1];

	return "";
}

void ValidateFilePath(std::string& pth)
{
	size_t slash_pos = pth.find_last_of('\\');
	while(slash_pos != std::string::npos) {
		pth.replace(slash_pos,1,"/");
		slash_pos = pth.find_last_of('\\');
	} 
}

std::string GetFileName(const char* fileName, bool bExtension)
{
	std::string s(fileName);
	ValidateFilePath(s);
	std::string sret(s);

	size_t slash_pos = s.find_last_of('/');
	size_t dot_pos = s.find_last_of('.');
	if(slash_pos != std::string::npos)
		sret = s.substr(slash_pos+1);

	if(!bExtension)
	{
		dot_pos = sret.find_last_of('.');
		if(dot_pos != std::string::npos)
			sret = sret.substr(0, dot_pos);
	}

	return sret;
}

std::string ExtractFilePath(const char* fileName)
{
	std::string s(fileName);
	size_t slash_pos = s.find_last_of('/');
	if(slash_pos != std::string::npos)
		return std::string(s.substr(0,slash_pos));
	return "";
}

// *****************************************************************************************************
// *****************************************************************************************************

} } // namespace ae { namespace base {

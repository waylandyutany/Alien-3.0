#pragma once

#ifndef AE_BASE_SYS_FILE_SYSTEM_H
#define AE_BASE_SYS_FILE_SYSTEM_H

namespace ae { namespace base {

class I_InputStream;

// *****************************************************************************************************
class I_FileEnumerator
{
public:
	virtual ~I_FileEnumerator() {}
	virtual void OnFile(const char* fileName, const char* path) = 0;
};

// *****************************************************************************************************
class C_FileSys
{
	C_FileSys() {}
	const C_FileSys& operator = (const C_FileSys&);
	C_FileSys(const C_FileSys&);

	struct S_Mount
	{
		std::string path;
		std::string name;
		std::string fullPath; // '..' is replaced by current directory
	};

	typedef std::vector<S_Mount> T_Mounts;
	T_Mounts m_Mounts;
// *****************************************************************************************************
// *****************************************************************************************************
	std::string GetMountedPath(const char* path) const;
	std::string GetFilePath(const char* path) const;
// *****************************************************************************************************
	typedef std::vector<std::string> T_Strings;
	void SeparateDirectoriesFromPath(const std::string& path, T_Strings& directories);
// *****************************************************************************************************
	bool AssureFile(const char* fileName);
	bool AssureDirectories(const T_Strings& directories);
public:
	static C_FileSys& Inst();
// *****************************************************************************************************
	void Mount(const char* path, const char* mntName);
	void EnumFiles(const char* path, const char* enumMask, I_FileEnumerator&);
	bool MakePathRelativeTo(const char* mntName, std::string& path);

// *****************************************************************************************************
	size_t GetFileSize(const char* fileName);
	size_t ReadFile(const char* fileName, char* buff, size_t size);
	size_t WriteFile(const char* fileName, char* buff, size_t size, bool bAssureFile);
	size_t AppendFile(const char* fileName, char* buff, size_t size, bool bAssureFile);
// *****************************************************************************************************
	void DeleteFile(const char* fileName);
	void RenameFile(const char* oldFileName, const char* newFileName);
	bool FileExist(const char* path);
	bool PathExist(const char* path);
// *****************************************************************************************************
	bool GetFileTimes(const char* fileName, ae::u64* timeCreation, ae::u64* timeWriting, ae::u64* timeReading);
	bool SetFileTimes(const char* fileName, const ae::u64* timeCreation, const ae::u64* timeWriting, const ae::u64* timeReading);
// *****************************************************************************************************
	I_InputStream* CreateInputStream(const char* fileName);
	std::ostream* CreateOutputStream(const char* fileName, bool bAssureFile = false);
// *****************************************************************************************************
	struct S_MappedFile
	{
		size_t Size() const { return size; }
		size_t Size() { return size; }

		template<typename T_Return>
		T_Return Buffer() const { return static_cast<T_Return>(buffer); }
		template<typename T_Return>
		T_Return Buffer() { return static_cast<T_Return>(buffer); }

	private:
		friend class C_FileSys;
		void* handles[2];
		void* buffer;
		size_t size;
		bool bReadOnly;
	};
	S_MappedFile* CreateMappedFile(const char* fileName, size_t size, bool bAssureFile);
	S_MappedFile* OpenMappedFile(const char* fileName) const;
	void CloseMappedFile(S_MappedFile*, size_t size = 0);
// *****************************************************************************************************
	typedef std::deque<std::string> T_FileNames;
	void MergeFiles(const T_FileNames&, const char* mergedFileName);
// *****************************************************************************************************
};

std::string GetFileExtension(const char* fileName);
std::string GetFileName(const char* fileName, bool bExtension = true);
std::string ExtractFilePath(const char* fileName);

} } // namespace ae { namespace base {

#endif // #ifdef AE_BASE_SYS_FILE_SYSTEM_H

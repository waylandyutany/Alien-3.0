namespace ae { namespace resource {

template<typename T_Resource>
inline C_Import<T_Resource> BeginImport(const char* fileToExport, const char* resourceName, T_ResourceType resourceType, size_t memoryUsage)
{
	C_Import<T_Resource> import;

	import.pMappedFile = ae::base::C_FileSys::Inst().CreateMappedFile(fileToExport, memoryUsage, true);
	import.allocator.Create(import.pMappedFile->Buffer<char*>(), import.pMappedFile->Size());
	import.allocator.Reset(true);
	import.pResource = reinterpret_cast<T_Resource*>(import.allocator.Alloc(sizeof(T_Resource)));
	import.pResource->resourceType = resourceType;
	import.pResource->hid.id = ae::AllocString(import.allocator, resourceName);
	import.pResource->hid.idHash = ae::base::CalculateHashI32(resourceName);
	import.pResource->headerSize = import.allocator.GetRelativeUsage(import.pResource);
	import.AddPointer(&import.pResource->hid.id);
	return import;
}

template<typename T_Resource>
inline void EndImport(C_Import<T_Resource>& import)
{
	const size_t pts(import.pointers.size());
	if(pts)
	{
		import.pResource->offsetPointersTable = import.allocator.GetRelativeUsage(import.pResource);
		C_PointersTableExport pte(reinterpret_cast<char*>(import.pResource), import.pResource->offsetPointersTable, import.allocator);
		for(size_t i = 0; i < pts; i++)
			pte.AddPointer(import.pointers[i]);

		pte.MakeOffsetsRelative();
	}

	import.pResource->totalSize = import.allocator.GetRelativeUsage(import.pResource);
	ae::base::C_FileSys::Inst().CloseMappedFile(import.pMappedFile,import.pResource->totalSize);
}

} } // namespace ae { namespace resource {

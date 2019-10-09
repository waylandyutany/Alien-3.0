#include "StdAfx.h"

#include "resource_handler.h"

#include "../../debug_sys/output/outputs.h"
#include "import_pointers_table.h"

namespace ae { namespace resource {

/*virtual*/ void I_ResourceHandler::PrintDebug()
{
	S_Resource* pResource(GetResource());

	DBG_OUT << "Resource info : " << std::endl;
	DBG_OUT << "Type          : '";
	DBG_OUT << Get_Rsc_Type_C0(pResource->resourceType);
	DBG_OUT << Get_Rsc_Type_C1(pResource->resourceType);
	DBG_OUT << Get_Rsc_Type_C2(pResource->resourceType);
	DBG_OUT << Get_Rsc_Type_C3(pResource->resourceType);
	DBG_OUT << "'" << std::endl;

	DBG_OUT << "Header size   : " << ae::dbg::dec_fmt(6) << pResource->headerSize	<< std::endl;
	DBG_OUT << "Total size    : " << ae::dbg::dec_fmt(6) << pResource->totalSize	<< std::endl;
	DBG_OUT << "Data size	  : " <<  ae::dbg::dec_fmt(6) << pResource->GetDataSize();

}

void I_ResourceHandler::ResolvePointers()
{
	S_Resource* pResource(GetResource());
	if(!(pResource->state & resourceResolved))
	{
		if(pResource->offsetPointersTable)
		{
			C_PointersTableImport pti(reinterpret_cast<char*>(pResource), pResource->offsetPointersTable);
			pti.MakePointersAbsolute();
		}
		pResource->state |= resourceResolved;
	}
}

/*virtual*/ void I_ResourceHandler::Activate()
{
	S_Resource* pResource(GetResource());
	// Set state that resource has been activated and pointers resolved
	pResource->state |= resourceActivated;
}

/*virtual*/ void I_ResourceHandler::Deactivate()
{
	S_Resource* pResource(GetResource());
	// Clear state that resource has been activated and pointers resolved
	pResource->state &= ~(resourceActivated | resourceResolved);
}


} } // namespace ae { namespace resource {

#include "StdAfx.h"

#include "input_sys.h"
#include "../render_sys/render_device/render_window.h"
#include "../debug_sys/output/outputs.h"

namespace ae { namespace input {

C_InputSys* C_InputSys::pInstance = 0;

bool C_InputSys::Create(S_Init& init)
{
	AE_ASSERT(!pInstance && "C_InputSys instance already created !");
	pInstance = new C_InputSys(init);
	DBG_OUT << "'" << pInstance->GetProviderName() << "' input system initialized." << std::endl;
	return true;
}

void C_InputSys::Release()
{
	AE_ASSERT(pInstance && "Uninitialized C_InputSys class instance !");
	DBG_OUT << "'" << pInstance->GetProviderName() << "' input system released." << std::endl;
	delete pInstance;
}

C_InputSys& C_InputSys::Inst()
{
	return *pInstance;
}

C_InputSys::C_InputSys(S_Init& init) :
	m_Impl(init.window.GetImpl()->GetHWnd())
{
}

const char* C_InputSys::GetProviderName() const
{
	return m_Impl.GetProviderName();
}

} } // namespace ae { namespace input {

#include "stdafx.h"

#if defined(AE_EDITOR)

#include "profile_panel.h"

#include "../../../../engine/debug_sys/debug_sys.h"

namespace ae { namespace editor {

C_ProfilePanel::C_ProfilePanel(C_Widget* pParent) : 
	C_BasePanel(pParent),
	m_ProfileText(this, "Profile")
{
	m_ProfileText.SetPos(0,0);
	m_ProfileText.SetSize(300,600);
}

/*virtual*/ C_ProfilePanel::~C_ProfilePanel()
{
}

/*virtual*/ void C_ProfilePanel::Activate()
{
	SetTimer(100,50);
}

/*virtual*/ void C_ProfilePanel::Deactivate()
{
	DelTimer(100);
}

/*virtual*/ void C_ProfilePanel::OnTimer(ae::u32 timerID)
{
	std::stringstream ss;

	ae::dbg::C_ProfileBlocksTree::T_BlocksInfo blocksInfo;
	ae::dbg::profilerBlocksTree.GetBlocksInfo(blocksInfo);

	ss << "Profile blocks info" << std::endl;
	const float rcp(1.0f / 1000.0f);
	const size_t s(blocksInfo.size());
	for(size_t i(0); i < s; i++) {
		const float timeMS = blocksInfo[i].blockTime * rcp;
		ss << blocksInfo[i].blockName << " [" << timeMS << "ms]" << std::endl;
	}

	m_ProfileText.SetText(ss.str().c_str());
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

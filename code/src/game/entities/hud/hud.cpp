#include "StdAfx.h"

#include "hud.h"

#include "../player/player.h"
#include "../projectiles/projectile.h"

#include "../../../engine/debug_sys/debug_sys.h"
#include "../../../engine/math_sys/types_dbg.h"

#include "../../../engine/render_sys/render_scene/render_scene.h"
#include "../../../engine/render_sys/render_sys.h"

#include "../../../engine/input_sys/input_sys.h"

namespace ag { namespace entities {

// *****************************************************************************************************
// *****************************************************************************************************
C_HUDText::C_HUDText(ae::s32 fontSizeX, ae::s32 fontSizeY, ae::s32 fontShrinkX) :
	m_FontSizeX(fontSizeX), m_FontSizeY(fontSizeY), m_FontShrinkX(fontShrinkX), m_FontColor(1,1,1,1)
{
}

void C_HUDText::DrawText(ae::render::C_Render2DContext& ctx, E_HAlign hAlign, E_VAlign vAlign, std::stringstream& ss)
{
	ae::u32 screenCX,screenCY;
	ae::render::C_RenderSys::Instance().GetScene().GetScreenDimensions(screenCX, screenCY);

	std::string s;
	std::stringstream tmps(ss.str());
// *****************************************************************************************************
// Calculation of maximal line length and number of lines
	ae::u32 lineWidth = 0;
	ae::u32 numLines = 0;
	while(std::getline(tmps,s))
	{
		lineWidth = ae::math::Max(lineWidth, (ae::u32)s.length());
		numLines++;
	}
	lineWidth++;
// *****************************************************************************************************
	ae::s32 textX = 0, textY = 0;
	const ae::s32 textWidth = (lineWidth * m_FontSizeX) + (lineWidth * m_FontShrinkX);
	const ae::s32 textHeight = numLines * m_FontSizeY;

	if(hAlign == E_HALIGN_CENTER) textX = (screenCX / 2) - (textWidth / 2);
	else if(hAlign == E_HALIGN_RIGHT) textX = screenCX - textWidth;

	if(vAlign == E_VALIGN_CENTER) textX = (screenCY / 2) - (textHeight / 2);
	else if(vAlign == E_VALIGN_BOTTOM) textY = screenCY - textHeight;

	ctx.DrawBox(textX,textY,textWidth,textHeight,ae::math::float4(0.2f,0.2f,0.2f,0.5f));
	while(std::getline(ss,s))
	{
		ctx.DrawText(ae::render::E_2DT_Font0, textX, textY, m_FontSizeX, m_FontSizeY, m_FontShrinkX, s.c_str(), m_FontColor);
		textY += m_FontSizeY;
	}
}
// *****************************************************************************************************
// *****************************************************************************************************
C_HUD* g_pHUD = 0;

C_HUD::C_HUD() : 
	m_HUDText(16,24,-6),
	m_HUDIndex(0),
	m_HelpIndex(0),
	m_ProfileIndex(0),
	m_Seconds(0.0f),
	m_bDrawProjectileTrajectory(false)
{
	m_HUDUpdates[0] = &C_HUD::UpdateHelp;
	m_HUDUpdates[1] = &C_HUD::UpdateProfile;

	m_HelpUpdates[0] = &C_HUD::UpdateHelp0;
	m_HelpUpdates[1] = &C_HUD::UpdateHelp1;

	m_ProfileUpdates[0] = &C_HUD::UpdateProfile0;
	m_ProfileUpdates[1] = &C_HUD::UpdateProfile1;
	m_ProfileUpdates[2] = &C_HUD::UpdateProfile2;
	AE_ASSERT(!g_pHUD && "Must be NULL !");
	g_pHUD = this;
}

C_HUD::~C_HUD()
{
	AE_ASSERT(g_pHUD && "Must not be NULL !");
	g_pHUD = 0;
}

void C_HUD::UpdateHelp(float deltaTime)
{
	(this->*m_HelpUpdates[m_HelpIndex])(deltaTime);

	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();
	if(input.KeyState(ae::input::E_KS_Key_Pressed, ae::input::E_KC_F1))
		m_HelpIndex = (m_HelpIndex + 1) % (sizeof(m_HelpUpdates) / sizeof(*m_HelpUpdates));
}

void C_HUD::UpdateHelp0(float /*deltaTime*/)
{
	ae::render::C_Render2DContext& context2d = ae::render::C_RenderSys::Instance().GetScene().Get2DContext();
	std::stringstream ss;
	ss << "Help (F1)  Profile (F2)" << std::endl;
	m_HUDText.DrawText(context2d, C_HUDText::E_HALIGN_LEFT, C_HUDText::E_VALIGN_TOP, ss);
}

void C_HUD::UpdateHelp1(float /*deltaTime*/)
{
	ae::render::C_Render2DContext& context2d = ae::render::C_RenderSys::Instance().GetScene().Get2DContext();
	std::stringstream ss;
	ss << "Help (F1)  Profile (F2)" << std::endl;
	ss << "Move               (W,S,A,D)" << std::endl;
	ss << "Sprint/Slow        (Shift/Alt)" << std::endl;
	ss << "Jump/Crouch/Ground (Space/C/Ctrl)" << std::endl;
	ss << "Camera FPS/Free    (Tab)" << std::endl;
	ss << "Zoom               (RMB)" << std::endl;
	ss << "Quit               (ESC)" << std::endl;
	ss << "Projectile trajectory(P)" << std::endl;
	m_HUDText.DrawText(context2d, C_HUDText::E_HALIGN_LEFT, C_HUDText::E_VALIGN_TOP, ss);
}

void C_HUD::UpdateProfile(float deltaTime)
{
	(this->*m_ProfileUpdates[m_ProfileIndex])(deltaTime);

	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();
	if(input.KeyState(ae::input::E_KS_Key_Pressed, ae::input::E_KC_F2))
		m_ProfileIndex = (m_ProfileIndex + 1) % (sizeof(m_ProfileUpdates) / sizeof(*m_ProfileUpdates));
}


void C_HUD::UpdateProfile0(float /*deltaTime*/)
{
	ae::u32 screenWidth, screenHeight;
	ae::render::C_Render2DContext& context2d = ae::render::C_RenderSys::Instance().GetScene().Get2DContext();
	ae::render::C_RenderSys::Instance().GetScene().GetScreenDimensions(screenWidth, screenHeight);

	std::stringstream ss;
	ss << "FPS " << ae::dbg::profiler.GetFrameFPS() << "(" << ae::dbg::flt_fmt(4) << ae::dbg::profiler.GetFrameMS() << "ms)" << ae::dbg::clr_fmt;
	ss << " " << screenWidth << "x" << screenHeight << std::endl;
	m_HUDText.DrawText(context2d, C_HUDText::E_HALIGN_LEFT, C_HUDText::E_VALIGN_TOP, ss);
}

void C_HUD::UpdateProfile1(float /*deltaTime*/)
{
	ae::render::C_Render2DContext& context2d = ae::render::C_RenderSys::Instance().GetScene().Get2DContext();
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

	m_HUDText.DrawText(context2d, C_HUDText::E_HALIGN_LEFT, C_HUDText::E_VALIGN_TOP, ss);
}

void C_HUD::UpdateProfile2(float /*deltaTime*/)
{
	ae::render::C_Render2DContext& context2d = ae::render::C_RenderSys::Instance().GetScene().Get2DContext();

	std::stringstream ss;
	ss << "Audio profile info" << std::endl;
	ss << "Memory usage       : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("MemoryUsageInBytes")) << std::endl;
	ss << "Latency in samples : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("CurrentLatencyInSamples")) << std::endl;
	ss << "Glitches           : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("GlitchesSinceEngineStarted")) << std::endl;
	ss << "Active sources     : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("ActiveSourceVoiceCount")) << std::endl;
	ss << "Total sources      : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("TotalSourceVoiceCount")) << std::endl;
	ss << "Submixes count     : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("ActiveSubmixVoiceCount")) << std::endl;
	ss << "Resampling count   : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("ActiveResamplerCount")) << std::endl;
	ss << "Matrix mix count   : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("ActiveMatrixMixCount")) << std::endl;
	ss << "Audio decodings    : " << ae::dbg::profiler.GetCounterValue(ae::dbg::profiler.GetCounter("AudioStreamDecodings")) << std::endl;

	m_HUDText.DrawText(context2d, C_HUDText::E_HALIGN_LEFT, C_HUDText::E_VALIGN_TOP, ss);
}


void C_HUD::OnUpdate(float deltaTime)
{
	m_Seconds += deltaTime;

	(this->*m_HUDUpdates[m_HUDIndex])(deltaTime);

	ae::input::C_InputSys& input = ae::input::C_InputSys::Inst();
	ae::render::C_RenderSys& renderSys(ae::render::C_RenderSys::Instance());
	ae::render::C_Render2DContext& context2d = renderSys.GetScene().Get2DContext();
	renderSys.GetScene().SetSceneColor(ae::math::float4(1,1,1,1));

	if(input.KeyState(ae::input::E_KS_Key_Pressed, ae::input::E_KC_F1))
		m_HUDIndex = 0;
	else if(input.KeyState(ae::input::E_KS_Key_Pressed, ae::input::E_KC_F2))
		m_HUDIndex = 1;

	std::stringstream ss;
	switch(C_Player::GetCameraType())
	{
	case E_Camera_FPS : ss << "Camera FPS" << std::endl;break;
	case E_Camera_Free : ss << "Camera Free" << std::endl;break;
	}
	m_HUDText.DrawText(context2d, C_HUDText::E_HALIGN_CENTER, C_HUDText::E_VALIGN_TOP, ss);

	std::stringstream ss1;
	ss1 << "Confidential ! Property of Frankie Calderone. Confidential !" << std::endl;
/*
	ae::u32 h,m,s;
	::ae::base::SecondsToHMS(m_Seconds, h, m, s);
	std::stringstream ss1;
	ss1 << ae::dbg::dec_fmt(2) << h << ":" << ae::dbg::dec_fmt(2) << m << ":" << ae::dbg::dec_fmt(2) << s << std::endl;
	ss1 << "Player state :          " << std::endl;
	ss1 << "Grnd pos     :          " << std::endl;
	ss1 << playerState.pos << std::endl;
	ss1 << "Grnd contact :          " << std::endl;
	ss1 << playerState.groundContact  << std::endl;
	ss1 << "Dst. to grnd : " << ae::dbg::flt_fmt(2) << playerState.distanceToGround << std::endl;
	ss1 << "Is falling   : " << (playerState.isFalling  ? "True" : "False") << std::endl;
	ss1 << "Is on grnd   : " << (playerState.isOnGround ? "True" : "False") << std::endl;
*/
	m_HUDText.DrawText(context2d, C_HUDText::E_HALIGN_CENTER, C_HUDText::E_VALIGN_BOTTOM, ss1);


// *****************************************************************************************************
// *****************************************************************************************************
#ifdef DRAW_PROJECTILE_TRAIL
	if(input.KeyState(ae::input::E_KS_Key_Pressed, ae::input::E_KC_P))
		m_bDrawProjectileTrajectory =! m_bDrawProjectileTrajectory;

	if(m_bDrawProjectileTrajectory)
		DrawProjectileTrail();
#endif // #ifdef DRAW_PROJECTILE_TRAIL
}


void C_HUD::DrawProjectileTrail()
{
	#ifdef DRAW_PROJECTILE_TRAIL
	if(!g_TrailPoints.size()) return;
	ae::render::C_RenderScene& scene = ae::render::C_RenderSys::Instance().GetScene();
	for(size_t i = 0; i < g_TrailPoints.size()-1; i++)
		scene.Get3DContext().DrawLine(g_TrailPoints[i], g_TrailPoints[i+1], ae::math::float4(0,0,1,1));
	#endif // #ifdef DRAW_PROJECTILE_TRAIL
}


} } // namespace ag { namespace entities {


#pragma once

#ifndef AG_ENTITIES_HUD_H
#define AG_ENTITIES_HUD_H

#include "../entity.h"
#include "../../../engine/math_sys/types.h"

namespace ae { namespace render {
	class C_Render2DContext;
} } // namespace ae { namespace render {

namespace ag { namespace entities {

// *****************************************************************************************************
// *****************************************************************************************************
class C_HUDText
{
	C_HUDText();
	NOCOPY_CLASS(C_HUDText);

	const ae::s32 m_FontSizeX;
	const ae::s32 m_FontSizeY;
	const ae::s32 m_FontShrinkX;
	const ae::math::float4 m_FontColor;
public:
	enum E_HAlign
	{
		E_HALIGN_LEFT,
		E_HALIGN_CENTER,
		E_HALIGN_RIGHT,
	};
	enum E_VAlign
	{
		E_VALIGN_TOP,
		E_VALIGN_CENTER,
		E_VALIGN_BOTTOM,
	};

	C_HUDText(ae::s32 fontSizeX, ae::s32 fontSizeY, ae::s32 fontShrinkX);

	void DrawText(ae::render::C_Render2DContext&, E_HAlign, E_VAlign, std::stringstream& ss);
};
// *****************************************************************************************************
// *****************************************************************************************************
class C_HUD : public I_Entity
{
	NOCOPY_CLASS(C_HUD);

	C_HUDText m_HUDText;
	typedef void (C_HUD::*T_Update)(float);
	T_Update m_HUDUpdates[2];
	T_Update m_HelpUpdates[2];
	T_Update m_ProfileUpdates[3];
	ae::u32 m_HUDIndex,m_HelpIndex, m_ProfileIndex;
	float m_Seconds;
	bool m_bDrawProjectileTrajectory;
public:
	struct S_PlayerState
	{
		float distanceToGround;
		bool isFalling, isOnGround;
		ae::math::float3 pos, groundContact;
	};
	void SetPlayerState(const S_PlayerState& ps) {
		playerState = ps;
	} 
	S_PlayerState playerState;
	C_HUD();
	~C_HUD();
protected:
	virtual void OnUpdate(float deltaTime);
	void UpdateHelp(float deltaTime);
	void UpdateProfile(float deltaTime);

	void UpdateHelp0(float deltaTime);
	void UpdateHelp1(float deltaTime);
	void UpdateProfile0(float deltaTime);
	void UpdateProfile1(float deltaTime);
	void UpdateProfile2(float deltaTime);

	void DrawProjectileTrail();
};
// *****************************************************************************************************
// *****************************************************************************************************

} } // namespace ag { namespace entities {

#endif // #ifdef AG_ENTITIES_HUD_H

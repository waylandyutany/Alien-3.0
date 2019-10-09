#include "stdafx.h"

#if defined(AE_EDITOR)

#include "prop_panel_transformation.h"
#include "../../../engine/resource_sys/imports/importers/import_utils.h"

namespace ae { namespace editor {

// *****************************************************************************************************
// *****************************************************************************************************
/*virtual*/ void C_NumberSpinCB::OnFloatChange(float value, ae::windows_sys::T_ControlID id)
{
	if(!parent.m_pXMLNode) 
		return;

	if(parent.m_EditPosX.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_PosX, value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_PosX, value);
	}
	else if(parent.m_EditPosY.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_PosY, value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_PosY, value);
	}
	else if(parent.m_EditPosZ.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_PosZ, value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_PosZ, value);
	}
	if(parent.m_EditScaleX.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_ScaleX, value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_ScaleX, value);
	}
	else if(parent.m_EditScaleY.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_ScaleY, value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_ScaleY, value);
	}
	else if(parent.m_EditScaleZ.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_ScaleZ, value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_ScaleZ, value);
	}

	if(parent.pCB) parent.pCB->OnPropertyChanged();
}

/*virtual*/ void C_NumberSpinCB::OnIntegerChange(ae::windows_sys::T_Integer value, ae::windows_sys::T_ControlID id)
{
	if(!parent.m_pXMLNode) 
		return;

	if(parent.m_EditRotX.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_RotX, (float)value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_RotX, (float)value);
	}
	else if(parent.m_EditRotY.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_RotY, (float)value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_RotY, (float)value);
	}
	else if(parent.m_EditRotZ.GetControlID() == id) {
		ae::resource::SetTransformationElementToXML(*parent.m_pSource, *parent.m_pXMLNode, ae::math::E_TE_RotZ, (float)value);
		if(parent.pCB)	parent.pCB->OnTransformationChanged(ae::math::E_TE_RotZ, (float)value);
	}

	if(parent.pCB) parent.pCB->OnPropertyChanged();
}

// *****************************************************************************************************
// *****************************************************************************************************

C_PropPanelTransformation::C_PropPanelTransformation(ae::windows_sys::C_Widget* pParent, I_PropPanelTransformationCB* pCB) :
	C_PropertyPanel(),
	pCB(pCB),
	spinCB(*this),
	m_PosLayout(16), m_RotLayout(16), m_ScaleLayout(16),
	m_EditPosX(&spinCB, pParent, 1000), m_EditPosY(&spinCB, pParent, 1001), m_EditPosZ(&spinCB, pParent, 1002),
	m_TextPosX(pParent, "Pos X"), m_TextPosY(pParent, " Y "), m_TextPosZ(pParent, " Z "),
	m_EditRotX(&spinCB, pParent, 2000), m_EditRotY(&spinCB, pParent, 2001), m_EditRotZ(&spinCB, pParent, 2002),
	m_TextRotX(pParent, "Rot X"), m_TextRotY(pParent, " Y "), m_TextRotZ(pParent, " Z "),
	m_EditScaleX(&spinCB, pParent, 3000), m_EditScaleY(&spinCB, pParent, 3001), m_EditScaleZ(&spinCB, pParent, 3002),
	m_TextScaleX(pParent, "Scale X"), m_TextScaleY(pParent, " Y "), m_TextScaleZ(pParent, " Z "),
	m_pXMLNode(0),m_pSource(0)
{
	m_EditRotX.SetMinMax(-360, 360); m_EditRotY.SetMinMax(-360, 360);	m_EditRotZ.SetMinMax(-360, 360);
	m_EditRotX.LoopValue(true); m_EditRotY.LoopValue(true); m_EditRotZ.LoopValue(true);
	m_EditScaleX.SetMinMax(0.1f, 10.0f); m_EditScaleY.SetMinMax(0.1f, 10.0f); m_EditScaleZ.SetMinMax(0.1f, 10.0f);

	const int spinWidth = 60;
	m_PosLayout.AddLayoutItem(&m_TextPosX, 42, 0, 0);
	m_PosLayout.AddLayoutItem(&m_EditPosX, spinWidth, 0, 0);
	m_PosLayout.AddLayoutItem(&m_TextPosY, 16, 0, 0);
	m_PosLayout.AddLayoutItem(&m_EditPosY, spinWidth, 0, 0);
	m_PosLayout.AddLayoutItem(&m_TextPosZ, 16, 0, 0);
	m_PosLayout.AddLayoutItem(&m_EditPosZ, spinWidth, 0, 0);

	m_RotLayout.AddLayoutItem(&m_TextRotX, 42, 0, 0);
	m_RotLayout.AddLayoutItem(&m_EditRotX, spinWidth, 0, 0);
	m_RotLayout.AddLayoutItem(&m_TextRotY, 16, 0, 0);
	m_RotLayout.AddLayoutItem(&m_EditRotY, spinWidth, 0, 0);
	m_RotLayout.AddLayoutItem(&m_TextRotZ, 16, 0, 0);
	m_RotLayout.AddLayoutItem(&m_EditRotZ, spinWidth, 0, 0);

	m_ScaleLayout.AddLayoutItem(&m_TextScaleX, 42, 0, 0);
	m_ScaleLayout.AddLayoutItem(&m_EditScaleX, spinWidth, 0, 0);
	m_ScaleLayout.AddLayoutItem(&m_TextScaleY, 16, 0, 0);
	m_ScaleLayout.AddLayoutItem(&m_EditScaleY, spinWidth, 0, 0);
	m_ScaleLayout.AddLayoutItem(&m_TextScaleZ, 16, 0, 0);
	m_ScaleLayout.AddLayoutItem(&m_EditScaleZ, spinWidth, 0, 0);

	AddLayoutItem(&m_PosLayout, 16, 100, 0);
	AddLayoutItem(&m_RotLayout, 16, 100, 0);
	AddLayoutItem(&m_ScaleLayout, 16, 100, 0);
}
	
/*virtual*/ C_PropPanelTransformation::~C_PropPanelTransformation()
{
}

void C_PropPanelTransformation::Enable(bool bEnable)
{
	m_EditPosX.EnableWindow(bEnable);
	m_EditPosY.EnableWindow(bEnable);
	m_EditPosZ.EnableWindow(bEnable);
	m_EditScaleX.EnableWindow(bEnable);
	m_EditScaleY.EnableWindow(bEnable);
	m_EditScaleZ.EnableWindow(bEnable);
	m_EditRotX.EnableWindow(bEnable);
	m_EditRotY.EnableWindow(bEnable);
	m_EditRotZ.EnableWindow(bEnable);
}

void C_PropPanelTransformation::Clear()
{
	m_pXMLNode = 0;
	m_pSource = 0;

	m_EditPosX.SetValue(0);
	m_EditPosY.SetValue(0);
	m_EditPosZ.SetValue(0);
	m_EditScaleX.SetValue(1);
	m_EditScaleY.SetValue(1);
	m_EditScaleZ.SetValue(1);
	m_EditRotX.SetValue(0);
	m_EditRotY.SetValue(0);
	m_EditRotZ.SetValue(0);
}

void C_PropPanelTransformation::Set(ae::base::C_XML_Editable<>* pSource, ae::base::S_XMLNode* pXMLNode)
{
	m_pXMLNode = pXMLNode;
	m_pSource = pSource;

	ae::math::float3 pos,rot,scale;
	ae::resource::GetPosRotScaleFromXMLTransformation(*pXMLNode, pos, rot, scale);

	SetPos(pos);
	SetRot(rot);
	SetScale(scale);
}

// *****************************************************************************************************
void C_PropPanelTransformation::SetPos(const ae::math::float3& pos)
{
	m_EditPosX.SetValue(pos.x);
	m_EditPosY.SetValue(pos.y);
	m_EditPosZ.SetValue(pos.z);
}

void C_PropPanelTransformation::GetPos(ae::math::float3& pos) const
{
	pos.x = m_EditPosX.GetValue();
	pos.y = m_EditPosY.GetValue();
	pos.z = m_EditPosZ.GetValue();
}

void C_PropPanelTransformation::SetRot(const ae::math::float3& rot)
{
	m_EditRotX.SetValue((ae::windows_sys::T_Integer)rot.x);
	m_EditRotY.SetValue((ae::windows_sys::T_Integer)rot.y);
	m_EditRotZ.SetValue((ae::windows_sys::T_Integer)rot.z);
}

void C_PropPanelTransformation::GetRot(ae::math::float3& rot) const
{
	rot.x = (float)m_EditRotX.GetValue();
	rot.y = (float)m_EditRotY.GetValue();
	rot.z = (float)m_EditRotZ.GetValue();
}

void C_PropPanelTransformation::SetScale(const ae::math::float3& scale)
{
	m_EditScaleX.SetValue(scale.x);
	m_EditScaleY.SetValue(scale.y);
	m_EditScaleZ.SetValue(scale.z);
}

void C_PropPanelTransformation::GetScale(ae::math::float3& scale) const
{
	scale.x = m_EditScaleX.GetValue();
	scale.y = m_EditScaleY.GetValue();
	scale.z = m_EditScaleZ.GetValue();
}
// *****************************************************************************************************

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

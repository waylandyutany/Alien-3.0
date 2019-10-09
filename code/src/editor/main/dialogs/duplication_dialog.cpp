#include "stdafx.h"

#if defined(AE_EDITOR)

#include "duplication_dialog.h"

#include "../../../engine/resource_sys/imports\importers\import_utils.h"

namespace ae { namespace editor {

/*virtual*/ void C_ConfigBase::LoadCfg(const ae::base::C_XML<>& xml, const ae::base::S_XMLNode& node)
{
	isLoaded = node.Attribute("True").AsUInt(0) ? true : false;
}
/*virtual*/ void C_ConfigBase::SaveCfg(ae::base::C_XML_Editable<>& xml, ae::base::S_XMLNode& node)
{
	xml.SetAttributeUInt(node, "True", 1);
}

// *****************************************************************************************************
// *****************************************************************************************************
//ae::windows_sys::S_Rect ;

/*virtual*/ void C_DuplicationDialogConfig::LoadCfg(const ae::base::C_XML<>& xml, const ae::base::S_XMLNode& n)
{
	const ae::base::S_XMLNode& node = n.Node("DuplicationDialogConfig");

	C_ConfigBase::LoadCfg(xml, node);
	ae::resource::GetPosRotScaleFromXMLTransformation(node, pos, rot, scale);
	numDuplications = node.Attribute("Duplications").AsUInt(1);
	windowRect.left = node.Attribute("Left").AsInt(0);
	windowRect.right = node.Attribute("Right").AsInt(0);
	windowRect.top = node.Attribute("Top").AsInt(0);
	windowRect.bottom = node.Attribute("Bottom").AsInt(0);
}

/*virtual*/ void C_DuplicationDialogConfig::SaveCfg(ae::base::C_XML_Editable<>& xml, ae::base::S_XMLNode& n)
{
	ae::base::S_XMLNode& node = xml.AddNode(n,"DuplicationDialogConfig");

	C_ConfigBase::SaveCfg(xml,node);
	ae::resource::SetPosRotScaleToXML(xml, node, pos, rot, scale);
	xml.SetAttributeUInt(node, "Duplications", numDuplications);

	xml.SetAttributeInt(node, "Left", windowRect.left);
	xml.SetAttributeInt(node, "Right", windowRect.right);
	xml.SetAttributeInt(node, "Top", windowRect.top);
	xml.SetAttributeInt(node, "Bottom", windowRect.bottom);
}

// *****************************************************************************************************
// *****************************************************************************************************
C_DuplicationsDialog::C_DuplicationsDialog(C_Widget* pParent) : 
	C_WDialog(pParent,"Duplicate instancies"),
	groupBox(this, " Duplication properties "),
	propPanelTransformation(this),
	groupBoxLayout(330),
	vLayout(100),
	duplicationsLayout(16),
	buttonsLayout(20),
	numDuplications(0,this,100),
	textNumDuplications(this,"Duplications : "),
	buttonOK(this,"OK",101), 
	buttonCancel(this,"Cancel",102),
	buttonDefault(this,"Default",103)
{
	numDuplications.SetMinMax(1,100);

	groupBoxLayout.AddLayoutItem(&propPanelTransformation, 80, 0, 230);
	groupBox.SetLayout(&groupBoxLayout);

	duplicationsLayout.AddLayoutItem(&textNumDuplications, 128, 0, 0);
	duplicationsLayout.AddLayoutItem(&numDuplications, 42, 0, 0);

	buttonsLayout.AddLayoutItem(&buttonDefault, 50, 0, 0);
	buttonsLayout.AddLayoutItem(&buttonOK, 50, 0, 0);
	buttonsLayout.AddLayoutItem(&buttonCancel, 50, 0, 0);

	vLayout.SetMarging(8,8);
	vLayout.AddLayoutItem(&groupBox, 100, 0, 0);
	vLayout.AddLayoutItem(&duplicationsLayout, 20, 0, 0);
	vLayout.AddLayoutItem(&buttonsLayout, 20, 0, 0);

	SetLayout(&vLayout);
	SetSize(350,220);
	
	propPanelTransformation.SetPos(ae::math::float3(0,0,0));
	propPanelTransformation.SetRot(ae::math::float3(0,0,0));
	propPanelTransformation.SetScale(ae::math::float3(1,1,1));
}

C_DuplicationsDialog::~C_DuplicationsDialog()
{
}
	
// *****************************************************************************************************
// *****************************************************************************************************

void C_DuplicationsDialog::GetConfig(C_DuplicationDialogConfig& config) const
{
	propPanelTransformation.GetPos(config.pos);
	propPanelTransformation.GetRot(config.rot);
	propPanelTransformation.GetScale(config.scale);
	config.numDuplications = numDuplications.GetValue();
	GetRect(config.windowRect);
}

void C_DuplicationsDialog::SetConfig(const C_DuplicationDialogConfig& config)
{
	if(config.IsLoaded())
	{
		propPanelTransformation.SetPos(config.pos);
		propPanelTransformation.SetRot(config.rot);
		propPanelTransformation.SetScale(config.scale);
		numDuplications.SetValue(config.numDuplications);
		SetRect(config.windowRect);
	} else {
		SetRect(S_Rect(0,0,350,220));
	}
}

// *****************************************************************************************************
// *****************************************************************************************************

/*virtual*/ void C_DuplicationsDialog::OnCommand(T_ControlID id)
{
	if(buttonOK.GetControlID() == id)	{
		EndDialog(returnOK);
	}
	else if(buttonCancel.GetControlID() == id)	{
		EndDialog(returnCancel);
	}
	else if(buttonDefault.GetControlID() == id)	{
		propPanelTransformation.Clear();
		numDuplications.SetValue(1);
	}
}

void C_DuplicationsDialog::GetTransformation(ae::math::float3& pos, ae::math::float3& rot, ae::math::float3& scale) const
{
	propPanelTransformation.GetPos(pos);
	propPanelTransformation.GetRot(rot);
	propPanelTransformation.GetScale(scale);
}

ae::u32 C_DuplicationsDialog::GetNumDuplications() const
{
	return numDuplications.GetValue();
}

} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

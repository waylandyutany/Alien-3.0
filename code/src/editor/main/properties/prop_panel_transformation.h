#pragma once
#if !defined(AE_PROP_PANEL_TRANSFORMATION_H) && defined(AE_EDITOR)
#define AE_PROP_PANEL_TRANSFORMATION_H

#include "prop_container.h"
#include "../../windows_sys/layouts/hbox_layout.h"

#include "../../windows_sys/widgets/static_text.h"
#include "../../windows_sys/widgets/edit_box.h"
#include "../../windows_sys/widgets/number_spins.h"

#include "../../../engine/base_sys/xml/xml_editable.h"
#include "../../../engine/math_sys/types.h"

namespace ae { namespace base {
	struct S_XMLNode;
} } // namespace ae { namespace base {

namespace ae { namespace editor {

// *****************************************************************************************************
// *****************************************************************************************************
	class I_PropPanelTransformationCB : public I_PropertyPanelCB
	{
	public:
		virtual void OnPropertyChanged() {};
		virtual void OnTransformationChanged(ae::math::E_TransformationElement, float value) {};
	};

// *****************************************************************************************************
// *****************************************************************************************************
	class C_PropPanelTransformation;

	class C_NumberSpinCB : public ae::windows_sys::C_NumberSpinCallback
	{
		C_PropPanelTransformation& parent;
	public:
		C_NumberSpinCB(C_PropPanelTransformation& parent) : parent(parent) {}
		virtual void OnFloatChange(float, ae::windows_sys::T_ControlID);
		virtual void OnIntegerChange(ae::windows_sys::T_Integer, ae::windows_sys::T_ControlID);
	};
// *****************************************************************************************************
// *****************************************************************************************************
	class C_PropPanelTransformation : public C_PropertyPanel
	{
		C_NumberSpinCB spinCB;
		ae::windows_sys::C_HBoxLayout m_PosLayout,m_RotLayout,m_ScaleLayout;
		ae::windows_sys::C_FloatSpin m_EditPosX, m_EditPosY, m_EditPosZ;
		ae::windows_sys::C_StaticText m_TextPosX, m_TextPosY, m_TextPosZ;
		ae::windows_sys::C_IntegerSpin m_EditRotX, m_EditRotY, m_EditRotZ;
		ae::windows_sys::C_StaticText m_TextRotX, m_TextRotY, m_TextRotZ;
		ae::windows_sys::C_FloatSpin m_EditScaleX, m_EditScaleY, m_EditScaleZ;
		ae::windows_sys::C_StaticText m_TextScaleX, m_TextScaleY, m_TextScaleZ;

		ae::base::C_XML_Editable<>* m_pSource;
		ae::base::S_XMLNode* m_pXMLNode;

		I_PropPanelTransformationCB* pCB;

		friend class C_EBCallback;
		friend class C_NumberSpinCB;
	public:
		C_PropPanelTransformation(ae::windows_sys::C_Widget* pParent, I_PropPanelTransformationCB* pCB = 0);
		virtual ~C_PropPanelTransformation();
// *****************************************************************************************************
		void Enable(bool bEnable);
		void Clear();
		void Set(ae::base::C_XML_Editable<>*, ae::base::S_XMLNode*);
// *****************************************************************************************************
		void SetPos(const ae::math::float3& pos);
		void GetPos(ae::math::float3& pos) const;
		void SetRot(const ae::math::float3& rot);
		void GetRot(ae::math::float3& rot) const;
		void SetScale(const ae::math::float3& scale);
		void GetScale(ae::math::float3& scale) const;
	};
// *****************************************************************************************************
// *****************************************************************************************************
} } //namespace ae { namespace editor {

#endif // #if !defined(AE_PROP_PANEL_TRANSFORMATION_H) && defined(AE_EDITOR)

#pragma once

#ifndef AE_RESOURCE_SYS_IMPORT_UTILS_H
#define AE_RESOURCE_SYS_IMPORT_UTILS_H

#include "../../../math_sys/types.h"
#include "../../../base_sys/xml/xml_editable.h"

namespace ae { namespace base {
	struct S_XMLNode;
} } // namespace ae { namespace base {

namespace ae { namespace resource {
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// ! ! ! ROTATION ARE IN DEGRESS ! ! !
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	void SetPosToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const ae::math::float3& pos);
	void SetRotToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const ae::math::float3& rot);
	void SetScaleToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const ae::math::float3& scale);

	void GetPosFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& pos);
	void GetRotFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& rot);
	void GetScaleFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& scale);

	void SetPosRotScaleToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const ae::math::float3& pos, const ae::math::float3& rot, const ae::math::float3& scale);

	void GetPosRotScaleFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& pos, ae::math::float3& rot, ae::math::float3& scale);
	void GetMatrixFromXMLTransformation(const ae::base::S_XMLNode& tNode, ae::math::float4x4& matrix);

	bool SetTransformationElementToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, ae::math::E_TransformationElement element, float elementValue);
	bool GetTransformationElementFromXML(const ae::base::S_XMLNode& xmlNode, ae::math::E_TransformationElement element, float& elementValue);
} } // namespace ae { namespace resource {

#endif // #ifdef AE_RESOURCE_SYS_IMPORT_UTILS_H

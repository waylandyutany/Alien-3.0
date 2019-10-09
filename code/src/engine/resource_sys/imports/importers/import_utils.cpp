#include "StdAfx.h"

#include "import_utils.h"

#include "../../../base_sys/xml/xml.h"

namespace ae { namespace resource {

static void SetFloat3PropToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const char* propName, const ae::math::float3& float3)
{
	ae::base::S_XMLNode& tNode(xmlNode.Node(propName).IsValid() ? xmlNode.Node(propName) : source.AddNode(xmlNode, propName));
	source.SetAttributeFloat(tNode,"x",float3.x);
	source.SetAttributeFloat(tNode,"y",float3.y);
	source.SetAttributeFloat(tNode,"z",float3.z);
}

void SetPosToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const ae::math::float3& pos)
{
	SetFloat3PropToXML(source, xmlNode, "pos", pos);
}

void SetRotToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const ae::math::float3& rot)
{
	SetFloat3PropToXML(source, xmlNode, "rot", rot);
}

void SetScaleToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, const ae::math::float3& scale)
{
	SetFloat3PropToXML(source, xmlNode, "scale", scale);
}

void SetPosRotScaleToXML(ae::base::C_XML_Editable<>& source, ae::base::S_XMLNode& xmlNode, const ae::math::float3& pos, const ae::math::float3& rot, const ae::math::float3& scale)
{
	SetPosToXML(source, xmlNode, pos);
	SetRotToXML(source, xmlNode, rot);
	SetScaleToXML(source, xmlNode, scale);
}

static void GetFloat3PropFromXML(const ae::base::S_XMLNode& xmlNode, const char* propName, ae::math::float3& float3)
{
	const ae::base::S_XMLNode& tNode(xmlNode.Node(propName));
	float3.x = tNode.Attribute("x").AsFloat();
	float3.y = tNode.Attribute("y").AsFloat();
	float3.z = tNode.Attribute("z").AsFloat();
}

void GetPosFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& pos)
{
	GetFloat3PropFromXML(xmlNode, "pos", pos);
}

void GetRotFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& rot)
{
	GetFloat3PropFromXML(xmlNode, "rot", rot);
}

void GetScaleFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& scale)
{
	const ae::base::S_XMLNode& tNode(xmlNode.Node("scale"));
	scale.x = tNode.Attribute("x").AsFloat(1.0f);
	scale.y = tNode.Attribute("y").AsFloat(1.0f);
	scale.z = tNode.Attribute("z").AsFloat(1.0f);
}

void GetPosRotScaleFromXMLTransformation(const ae::base::S_XMLNode& xmlNode, ae::math::float3& pos, ae::math::float3& rot, ae::math::float3& scale)
{
	GetPosFromXMLTransformation(xmlNode, pos);
	GetRotFromXMLTransformation(xmlNode, rot);
	GetScaleFromXMLTransformation(xmlNode, scale);
}

void GetMatrixFromXMLTransformation(const ae::base::S_XMLNode& tNode, ae::math::float4x4& matrix)
{
	ae::math::float3 translation, rotation, scale;
	GetPosRotScaleFromXMLTransformation(tNode, translation, rotation, scale);
	rotation.x = ::ae::math::Deg2Radians(rotation.x);
	rotation.y = ::ae::math::Deg2Radians(rotation.y);
	rotation.z = ::ae::math::Deg2Radians(rotation.z);
	matrix.TranslateRotateScale(translation, rotation, scale);
}

bool SetTransformationElementToXML(ae::base::C_XML_Editable<>& source,ae::base::S_XMLNode& xmlNode, ae::math::E_TransformationElement element, float elementValue)
{
	switch(element)
	{
	case ae::math::E_TE_PosX: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("pos").IsValid() ? xmlNode.Node("pos") : source.AddNode(xmlNode, "pos"));
			source.SetAttributeFloat(tNode,"x", elementValue);
			return true;
		}
	case ae::math::E_TE_PosY: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("pos").IsValid() ? xmlNode.Node("pos") : source.AddNode(xmlNode, "pos"));
			source.SetAttributeFloat(tNode,"y", elementValue);
			return true;
		}
	case ae::math::E_TE_PosZ: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("pos").IsValid() ? xmlNode.Node("pos") : source.AddNode(xmlNode, "pos"));
			source.SetAttributeFloat(tNode,"z", elementValue);
			return true;
		}
	case ae::math::E_TE_ScaleX:	{
			ae::base::S_XMLNode& tNode(xmlNode.Node("scale").IsValid() ? xmlNode.Node("scale") : source.AddNode(xmlNode, "scale"));
			source.SetAttributeFloat(tNode,"x", elementValue);
			return true;
		}
	case ae::math::E_TE_ScaleY: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("scale").IsValid() ? xmlNode.Node("scale") : source.AddNode(xmlNode, "scale"));
			source.SetAttributeFloat(tNode,"y", elementValue);
			return true;
		}
	case ae::math::E_TE_ScaleZ: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("scale").IsValid() ? xmlNode.Node("scale") : source.AddNode(xmlNode, "scale"));
			source.SetAttributeFloat(tNode,"z", elementValue);
			return true;
		}
	case ae::math::E_TE_RotX: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("rot").IsValid() ? xmlNode.Node("rot") : source.AddNode(xmlNode, "rot"));
			source.SetAttributeFloat(tNode,"x", elementValue);
			return true;
		}
	case ae::math::E_TE_RotY: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("rot").IsValid() ? xmlNode.Node("rot") : source.AddNode(xmlNode, "rot"));
			source.SetAttributeFloat(tNode,"y", elementValue);
			return true;
		}
	case ae::math::E_TE_RotZ: {
			ae::base::S_XMLNode& tNode(xmlNode.Node("rot").IsValid() ? xmlNode.Node("rot") : source.AddNode(xmlNode, "rot"));
			source.SetAttributeFloat(tNode,"z", elementValue);
			return true;
		}
	default:
		AE_ASSERT(0 && "Invalid transformation element !");
	}
	return false;
}

bool GetTransformationElementFromXML(const ae::base::S_XMLNode& xmlNode, ae::math::E_TransformationElement element, float& elementValue)
{
	switch(element)
	{
	case ae::math::E_TE_PosX:
		elementValue = xmlNode.Node("pos").Attribute("x").AsFloat(0);
		return true;
	case ae::math::E_TE_PosY:
		elementValue = xmlNode.Node("pos").Attribute("y").AsFloat(0);
		return true;
	case ae::math::E_TE_PosZ:
		elementValue = xmlNode.Node("pos").Attribute("z").AsFloat(0);
		return true;
	case ae::math::E_TE_ScaleX:
		elementValue = xmlNode.Node("scale").Attribute("x").AsFloat(1);
		return true;
	case ae::math::E_TE_ScaleY:
		elementValue = xmlNode.Node("scale").Attribute("y").AsFloat(1);
		return true;
	case ae::math::E_TE_ScaleZ:
		elementValue = xmlNode.Node("scale").Attribute("z").AsFloat(1);
		return true;
	case ae::math::E_TE_RotX:
		elementValue = xmlNode.Node("rot").Attribute("x").AsFloat(0);
		return true;
	case ae::math::E_TE_RotY:
		elementValue = xmlNode.Node("rot").Attribute("y").AsFloat(0);
		return true;
	case ae::math::E_TE_RotZ:
		elementValue = xmlNode.Node("rot").Attribute("z").AsFloat(0);
		return true;
	default:
		AE_ASSERT(0 && "Invalid transformation element !");
	}
	return false;
}

} } // namespace ae { namespace resource {

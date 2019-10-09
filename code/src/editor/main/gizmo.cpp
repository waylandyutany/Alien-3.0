#include "stdafx.h"

#if defined(AE_EDITOR)

#include "gizmo.h"

#include "../../../engine/render_sys/render_scene/render_scene.h"
#include "../../../engine/render_sys/render_scene/render_camera.h"
#include "../../../engine/render_sys/render_sys.h"

#include "../../../engine/math_sys/collision/collision.h"
#include "../../../engine/input_sys/input_sys.h"
#include "../../../engine/debug_sys/output/outputs.h"

namespace ae { namespace editor {

	template< ae::u32 numPoints >
	class C_CirclePoints
	{
		NOCOPY_CLASS(C_CirclePoints);
		ae::math::float2 points[numPoints];
		ae::math::float2 points360[360];
	public:
		C_CirclePoints()
		{
			const float step = ae::math::pi2 / numPoints;
			float angle = 0;
			for(ae::u32 i = 0; i < numPoints; i++)
			{
				points[i].x = cosf(angle);
				points[i].y = sinf(angle);
				angle += step;
			}

			for(ae::u32 i = 0; i < 360; i++)
			{
				float angle = ae::math::Deg2Radians((float)i);
				points360[i].x = cosf(angle);
				points360[i].y = sinf(angle);
			}
		}

		enum E_Orientation
		{
			E_OrientationX,
			E_OrientationY,
			E_OrientationZ,
		};

		void GetPoints(ae::math::float3 pts[numPoints], float radius, E_Orientation orientation)
		{
			switch(orientation)
			{
			case E_OrientationX:
				for(ae::u32 i = 0; i < numPoints; i++)	{
					pts[i].x = 0.0f;
					pts[i].y = points[i].x * radius;
					pts[i].z = points[i].y * radius;
				}
				break;
			case E_OrientationY:
				for(ae::u32 i = 0; i < numPoints; i++)	{
					pts[i].y = 0.0f;
					pts[i].x = points[i].x * radius;
					pts[i].z = points[i].y * radius;
				}
				break;
			case E_OrientationZ:
				for(ae::u32 i = 0; i < numPoints; i++)	{
					pts[i].z = 0.0f;
					pts[i].x = points[i].x * radius;
					pts[i].y = points[i].y * radius;
				}
				break;
			default:
				AE_ASSERT(0 && "Invalid orientation parameter !");
			}
		}
		// ! ! ! angle is in degree ! ! !
		void GetPoint(ae::math::float3& point, float angle, float radius, E_Orientation orientation)
		{
			ae::s32 uAngle((ae::s32)angle);
			uAngle %= 360;
			if(uAngle < 0) uAngle = 360 - uAngle;
			switch(orientation)
			{
			case E_OrientationX:
				point.x = 0.0f;
				point.y = points[uAngle].x * radius;
				point.z = points[uAngle].y * radius;
				break;
			case E_OrientationY:
				point.y = 0.0f;
				point.x = points[uAngle].x * radius;
				point.z = points[uAngle].y * radius;
				break;
			case E_OrientationZ:
				point.z = 0.0f;
				point.x = points[uAngle].x * radius;
				point.y = points[uAngle].y * radius;
				break;
			default:
				AE_ASSERT(0 && "Invalid orientation parameter !");
			}
		}
	};

	static C_CirclePoints<32> circlePoints;

	static void DrawArrow(	ae::render::C_Render3DContext& context3D, 
							C_Gizmo::E_GizmoObject object, 
							const ae::math::float3& center, 
							const ae::math::float3& halfSize,
							const ae::math::float4& color )
	{
		ae::math::float3 arrowTop(center), bottom0(center), bottom1(center), bottom2(center), bottom3(center);
		switch(object) {
		case C_Gizmo::E_TranslationX:
			arrowTop.x += halfSize.x;

			bottom0.x -=  halfSize.x;
			bottom1.x -=  halfSize.x;
			bottom2.x -=  halfSize.x;
			bottom3.x -=  halfSize.x;

			bottom0.z -= halfSize.z; // --
			bottom0.y -= halfSize.y;
			bottom1.z -= halfSize.z; // -+
			bottom1.y += halfSize.y;
			bottom2.z += halfSize.z; // ++
			bottom2.y += halfSize.y;
			bottom3.z += halfSize.z; // +-
			bottom3.y -= halfSize.y;
			break;
		case C_Gizmo::E_TranslationY:
			arrowTop.y += halfSize.y;

			bottom0.y -=  halfSize.y;
			bottom1.y -=  halfSize.y;
			bottom2.y -=  halfSize.y;
			bottom3.y -=  halfSize.y;

			bottom0.x -= halfSize.x; // --
			bottom0.z -= halfSize.z;
			bottom1.x -= halfSize.x; // -+
			bottom1.z += halfSize.z;
			bottom2.x += halfSize.x; // ++
			bottom2.z += halfSize.z;
			bottom3.x += halfSize.x; // +-
			bottom3.z -= halfSize.z;
			break;
		case C_Gizmo::E_TranslationZ:
			arrowTop.z += halfSize.z;

			bottom0.z -=  halfSize.z;
			bottom1.z -=  halfSize.z;
			bottom2.z -=  halfSize.z;
			bottom3.z -=  halfSize.z;

			bottom0.y -= halfSize.y; // --
			bottom0.x -= halfSize.x;
			bottom1.y -= halfSize.y; // -+
			bottom1.x += halfSize.x;
			bottom2.y += halfSize.y; // ++
			bottom2.x += halfSize.x;
			bottom3.y += halfSize.y; // +-
			bottom3.x -= halfSize.x;
			break;
		}

		context3D.DrawTriangle(bottom0, bottom1, arrowTop, color);
		context3D.DrawTriangle(bottom1, bottom2, arrowTop, color);
		context3D.DrawTriangle(bottom2, bottom3, arrowTop, color);
		context3D.DrawTriangle(bottom3, bottom0, arrowTop, color);
		context3D.DrawQuad(bottom0, bottom1, bottom2, bottom3, color);
	}
	namespace {
		static const ae::math::float4 gismoColor_OrientationX(1,0,0,1);
		static const ae::math::float4 gismoColor_OrientationY(0,1,0,1);
		static const ae::math::float4 gismoColor_OrientationZ(0,0,1,1);

		static const ae::math::float3 scaleBoxHalfSize(0.06f,0.06f,0.06f);
		static const float arrowWidth2(0.06f);
		static const float arrowLength(0.175f);
		static const float gizmoRotationRadius = 1.2f;

		static const ae::math::float3 rotBoxHalfSize(0.025f,0.025f,0.025f);

		static const ae::u32 numPoints = 32;
	}
	// *****************************************************************************************************
	// *****************************************************************************************************
	C_Gizmo::C_Gizmo() :
		isVisible(false),
		bDragging(false),
		gizmoScreenSize(1.0f / 20.0f),
		gizmoSize(1.0f),
		pNotifier(0),
		minScale(0.01f, 0.01f, 0.01f)
	{

	}

	void C_Gizmo::GetGizmoOrientations(ae::math::float3& orientationX, ae::math::float3& orientationY, ae::math::float3& orientationZ, float size) const
	{
		memset(&orientationX, 0, sizeof(orientationX));
		memset(&orientationY, 0, sizeof(orientationY));
		memset(&orientationZ, 0, sizeof(orientationZ));
		orientationX.x = 1.0f * size;
		orientationY.y = 1.0f * size;
		orientationZ.z = 1.0f * size;
	}

	void C_Gizmo::GetGizmoObject(E_GizmoObject object, float size, ae::math::box3d* pBox3D, ae::math::float3* pCenter, ae::math::float3* pHalfSize) const	
	{
		ae::math::float3 orientationX, orientationY, orientationZ;
		GetGizmoOrientations(orientationX, orientationY, orientationZ, size);

		ae::math::float3 center, halfSize;
		switch(object) {
		case E_TranslationX :
			center = pos + orientationX - ae::math::float3(arrowLength * 0.5f * size, 0, 0);
			halfSize = ae::math::float3(arrowLength * 0.5f * size, arrowWidth2 * size, arrowWidth2 * size);
			break;
		case E_TranslationY :
			center = pos + orientationY - ae::math::float3(0, arrowLength * 0.5f * size, 0);
			halfSize = ae::math::float3(arrowWidth2 * size, arrowLength * 0.5f * size, arrowWidth2 * size);
			break;
		case E_TranslationZ :
			center = pos + orientationZ - ae::math::float3(0, 0, arrowLength * 0.5f * size);
			halfSize = ae::math::float3(arrowWidth2 * size, arrowWidth2 * size, arrowLength * 0.5f * size);
			break;
		case E_ScaleX :
			center = pos + (orientationX * 0.5f);
			halfSize = scaleBoxHalfSize * size;
			break;
		case E_ScaleY :
			center = pos + (orientationY * 0.5f);
			halfSize = scaleBoxHalfSize * size;
			break;
		case E_ScaleZ :
			center = pos + (orientationZ * 0.5f);
			halfSize = scaleBoxHalfSize * size;
			break;
		default:
			AE_ASSERT(0 && "Invalid object parameter !");
		}

		if(pCenter) *pCenter = center;
		if(pHalfSize) *pHalfSize = halfSize;
		if(pBox3D) {
			// todo have no idea why to multiply half size by 0.5
			pBox3D->min = center - (halfSize * 0.5f);
			pBox3D->max = center + (halfSize * 0.5f);
		}
	}

	// *****************************************************************************************************
	// *****************************************************************************************************
	void C_Gizmo::Render() 
	{
		ae::math::float3 points[numPoints], rotBoxPos, tCenter, tHalfSize;
		ae::math::float3 orientationX, orientationY, orientationZ;
		const ae::math::float4 pickedColor(0,0,0,1);
		ae::render::C_Render3DContext& context3D(ae::render::C_RenderSys::Instance().GetScene().Get3DContext());

		GetGizmoOrientations(orientationX, orientationY, orientationZ, gizmoSize);

		// Gizmo axes
		context3D.DrawLine(pos, pos + orientationX, gismoColor_OrientationX);
		context3D.DrawLine(pos, pos + orientationY, gismoColor_OrientationY);
		context3D.DrawLine(pos, pos + orientationZ, gismoColor_OrientationZ);

		// Scale boxes drawing
		GetGizmoObject(E_ScaleX, gizmoSize, 0, &tCenter, &tHalfSize);
		context3D.DrawBox(tCenter, tHalfSize, pickedObject == E_ScaleX ? pickedColor : gismoColor_OrientationX);

		GetGizmoObject(E_ScaleY, gizmoSize, 0, &tCenter, &tHalfSize);
		context3D.DrawBox(tCenter, tHalfSize, pickedObject == E_ScaleY ? pickedColor : gismoColor_OrientationY);

		GetGizmoObject(E_ScaleZ, gizmoSize, 0, &tCenter, &tHalfSize);
		context3D.DrawBox(tCenter, tHalfSize, pickedObject == E_ScaleZ ? pickedColor : gismoColor_OrientationZ);

		// Translation arrows drawing
		GetGizmoObject(E_TranslationX, gizmoSize, 0, &tCenter, &tHalfSize);
		DrawArrow(context3D, E_TranslationX, tCenter, tHalfSize, pickedObject == E_TranslationX ? pickedColor : gismoColor_OrientationX);

		GetGizmoObject(E_TranslationY, gizmoSize, 0, &tCenter, &tHalfSize);
		DrawArrow(context3D, E_TranslationY, tCenter, tHalfSize, pickedObject == E_TranslationY ? pickedColor : gismoColor_OrientationY);

		GetGizmoObject(E_TranslationZ, gizmoSize, 0, &tCenter, &tHalfSize);
		DrawArrow(context3D, E_TranslationZ, tCenter, tHalfSize, pickedObject == E_TranslationZ ? pickedColor : gismoColor_OrientationZ);

/*
		// Rotation circles
		// X orientation
		{
			circlePoints.GetPoints(points, gizmoRotationRadius * gizmoSize, C_CirclePoints<32>::E_OrientationX);
			for(ae::u32 i = 0; i < numPoints; i++)
				context3D.DrawLine(pos + points[i], pos + points[(i + 1) % numPoints], gismoColor_OrientationX);
		}
		// Y orientation
		{
			circlePoints.GetPoints(points, gizmoRotationRadius * gizmoSize, C_CirclePoints<32>::E_OrientationY);
			for(ae::u32 i = 0; i < numPoints; i++)
				context3D.DrawLine(pos + points[i], pos + points[(i + 1) % numPoints], gismoColor_OrientationY);
		}
		// Z orientation
		{
			circlePoints.GetPoints(points, gizmoRotationRadius * gizmoSize, C_CirclePoints<32>::E_OrientationZ);
			for(ae::u32 i = 0; i < numPoints; i++)
				context3D.DrawLine(pos + points[i], pos + points[(i + 1) % numPoints], gismoColor_OrientationZ);
		}
*/
	}

	// *****************************************************************************************************
	// *****************************************************************************************************

	void C_Gizmo::Update()
	{
		// ! ! ! UpdateGizmoSizes must be called first in order to compute valid gizmo size which depends on camera and gizmo position  ! ! !
		if(isVisible)
		{
			UpdateGizmoSizes();
			UpdateMouseSelection();
			UpdateMouseDragging();
			Render();

		}
	}

	// *****************************************************************************************************
	// *****************************************************************************************************

	void C_Gizmo::UpdateGizmoSizes()
	{
		const ae::render::C_Camera& camera(ae::render::C_RenderSys::Instance().GetScene().GetCamera());
		const float z = (pos-camera.GetPos()).dot(camera.GetDir());
		gizmoSize = 2.0f*z*gizmoScreenSize*tanf(camera.GetFOV());
		if(gizmoSize < 1.0f) gizmoSize = 1.0f;
	}

	void C_Gizmo::UpdateMouseDragging()
	{
		const float maxDragDistance = 1000.0f;
		ae::math::float3 orientationX, orientationY, orientationZ;
		GetGizmoOrientations(orientationX, orientationY, orientationZ, gizmoSize);

		const ae::render::C_Camera& camera(ae::render::C_RenderSys::Instance().GetScene().GetCamera());
		ae::math::float3 tDragPoint, tP;

		if(ae::input::C_InputSys::Inst().MouseState(input::E_KS_Key_Pressed, input::E_MB_LEFT) && pickedObject != E_None)
		{
			if(pickedObject == E_ScaleX || pickedObject == E_TranslationX) {
				dragAxeStart = pos - orientationX * maxDragDistance;
				dragAxeEnd = pos + orientationX * maxDragDistance;
			} else if(pickedObject == E_ScaleY || pickedObject == E_TranslationY) {
				dragAxeStart = pos - orientationY * maxDragDistance;
				dragAxeEnd = pos + orientationY * maxDragDistance;
			} else if(pickedObject == E_ScaleZ || pickedObject == E_TranslationZ) {
				dragAxeStart = pos - orientationZ * maxDragDistance;
				dragAxeEnd = pos + orientationZ * maxDragDistance;
			}

			ae::math::FindClosestPointsBetweenLines(dragAxeStart, dragAxeEnd,  camera.GetPos(), camera.GetPos() + camera.GetDir() * 100.0f,  dragPoint, tP);

			bDragging = true;
			if(pNotifier)
				pNotifier->OnBeginChange();
		}
		else if(bDragging && ae::input::C_InputSys::Inst().MouseState(input::E_KS_Key_Released, input::E_MB_LEFT)) {
			if(pNotifier)
				pNotifier->OnEndChange();
			bDragging = false;
		}

		if(bDragging)
		{
			ae::math::FindClosestPointsBetweenLines(dragAxeStart, dragAxeEnd,  camera.GetPos(), camera.GetPos() + camera.GetDir() * 100.0f,  tDragPoint, tP);
			if(tDragPoint != dragPoint)
			{
				const ae::math::float3 diff(tDragPoint - dragPoint);
				if( pickedObject == E_TranslationX || pickedObject == E_TranslationY || pickedObject == E_TranslationZ )
				{
					pos += diff;
				}
				if( pickedObject == E_ScaleX || pickedObject == E_ScaleY || pickedObject == E_ScaleZ )
				{
					scale += diff;
					if(scale.x < minScale.x) scale.x = minScale.x;
					if(scale.y < minScale.y) scale.y = minScale.y;
					if(scale.z < minScale.z) scale.z = minScale.z;
				}
				dragPoint = tDragPoint;
				if(pNotifier)
					pNotifier->OnTransformationChange(pos, rot, scale);
			}
		}

	}

	void C_Gizmo::UpdateMouseSelection()
	{
		if(!bDragging)
		{
			const ae::render::C_Camera& camera(ae::render::C_RenderSys::Instance().GetScene().GetCamera());
			pickedObject = E_None;
			float minDistance = 2.0f;

			for(int i = (int)E_TranslationX; i <= (int)E_ScaleZ; i++)
			{
				ae::math::box3d collisionBox;
				GetGizmoObject((E_GizmoObject)i, gizmoSize, &collisionBox, 0, 0);
				const float t = ae::math::BoxLineIntersectionFast(collisionBox, camera.GetPos(), camera.GetPos() + camera.GetDir() * 100.0f);
				if( 0.0f <= t && t <= 1.0f && t < minDistance)
				{
					minDistance = t;
					pickedObject = (E_GizmoObject)i;
				}
			}
		}
	}


	// *****************************************************************************************************
	// *****************************************************************************************************

	void C_Gizmo::RegisterNotifier(I_GizmoNotifier* notifier) {
		pNotifier = notifier;
	}

	void C_Gizmo::UnregisterNotifier(I_GizmoNotifier* notifier) {
		pNotifier = 0;
	}

	void C_Gizmo::Show(bool show)
	{
		if(isVisible && pickedObject != E_None) {
			if(pNotifier)
				pNotifier->OnEndChange();
			pickedObject = E_None;
		}
		isVisible = show;
	}

	// *****************************************************************************************************
	// *****************************************************************************************************
} } //namespace ae { namespace editor {

#endif // #if defined(AE_EDITOR)

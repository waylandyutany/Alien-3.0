#pragma once
#if !defined(AE_GIZMO_H) && defined(AE_EDITOR)
#define AE_GIZMO_H

namespace ae { namespace editor {

	class C_AlienEditor;
	// *****************************************************************************************************
	// *****************************************************************************************************
	class I_GizmoNotifier
	{
	public:
		virtual void OnBeginChange() {};
		virtual void OnEndChange() {};
		virtual void OnTransformationChange(const ae::math::float3& pos,
											const ae::math::float3& rot,
											const ae::math::float3& scale) {}
	};

	// *****************************************************************************************************
	// *****************************************************************************************************
	class C_Gizmo
	{
	public:
		// ! ! ! keep enum order ! ! !
		enum E_GizmoObject
		{
			E_TranslationX = 0,
			E_TranslationY,
			E_TranslationZ,
			E_ScaleX,
			E_ScaleY,
			E_ScaleZ,
			E_RotX,
			E_RotY,
			E_RotZ,
			E_None
		};
	private:
		bool isVisible, bDragging;
		ae::math::float3 pos, rot, scale, dragPoint, dragAxeStart, dragAxeEnd, minScale;
		float gizmoScreenSize;
		float gizmoSize;
		I_GizmoNotifier* pNotifier;
		E_GizmoObject pickedObject;

		void GetGizmoOrientations(ae::math::float3& orientationX, ae::math::float3& orientationY, ae::math::float3& orientationZ, float size ) const;
		void GetGizmoObject(E_GizmoObject object, float size, ae::math::box3d* pBox3D, ae::math::float3* pCenter, ae::math::float3* pHalfSize) const;

		void UpdateMouseSelection();
		void UpdateMouseDragging();
		void UpdateGizmoSizes();
		void Render();
		void Update();

		friend class C_AlienEditor;
	public:
		C_Gizmo();

		bool IsVisible() const;
		void Show(bool show);

		void SetRot(const ae::math::float3&);
		const ae::math::float3& GetRot() const;

		void SetPos(const ae::math::float3&);
		const ae::math::float3& GetPos() const;

		void SetScale(const ae::math::float3&);
		const ae::math::float3& GetScale() const;

		// only one notifier can be registered at the time
		void RegisterNotifier(I_GizmoNotifier*);
		void UnregisterNotifier(I_GizmoNotifier*);
	};

} } //namespace ae { namespace editor {

#include "gizmo.inl"

#endif // #if !defined(AE_GIZMO_H) && defined(AE_EDITOR)

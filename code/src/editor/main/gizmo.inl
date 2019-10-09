namespace ae { namespace editor {

	inline bool C_Gizmo::IsVisible() const {
		return isVisible;
	}
		
	inline void C_Gizmo::SetPos(const ae::math::float3& _pos) {
		pos = _pos;
	}

	inline const ae::math::float3& C_Gizmo::GetPos() const {
		return pos;
	}

	inline void C_Gizmo::SetRot(const ae::math::float3& _rot) {
		rot = _rot;
	}

	inline const ae::math::float3& C_Gizmo::GetRot() const {
		return rot;
	}

	inline void C_Gizmo::SetScale(const ae::math::float3& _scale) {
		scale = _scale;
	}
	inline const ae::math::float3& C_Gizmo::GetScale() const {
		return scale;
	}


} } //namespace ae { namespace editor {

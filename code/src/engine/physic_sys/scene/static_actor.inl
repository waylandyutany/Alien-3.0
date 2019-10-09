namespace ae { namespace physic {

	inline T_StaticActorImpl& C_StaticActor::Impl() {
		return m_Impl;
	}

	inline const T_StaticActorImpl& C_StaticActor::Impl() const {
		return m_Impl;
	}

	inline const ae::scene::S_SceneNode* C_StaticActor::SceneNode() const {
		return m_pSceneNode;
	}

	inline ae::scene::S_SceneNode* C_StaticActor::SceneNode() {
		return m_pSceneNode;
	}

} } // namespace ae { namespace physic {

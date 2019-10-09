#pragma once

#if !defined(AG_EDITOR_INIT_H) && defined(AE_EDITOR)
#define AG_EDITOR_INIT_H

namespace ag {

	class C_AlienEditorInit
	{
	public:
		C_AlienEditorInit();
		~C_AlienEditorInit();

		void Update( float deltaTime );
	};

} // namespace ae {

#endif // #if !defined(AG_EDITOR_INIT_H) && defined(AE_EDITOR)

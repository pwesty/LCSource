
#pragma warning(disable : 4391)		// Platform SDK Warning
#pragma warning(disable : 4996)		// _CRT_SECURE_NO_WARNINGS

#if		!defined(WORLD_EDITOR)
#include <boost/function.hpp>
#endif	// WORLD_EDITOR
#include <Engine/Engine.h>
#include <Engine/Graphics/Shader.h>

/* rcg10042001 protect against Visual C-isms. */
#ifdef _MSC_VER
#define DECL_DLL _declspec(dllexport)
#endif

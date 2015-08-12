
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <commdlg.h>

#define ENGINE_INTERNAL 1
#define ENGINEGUI_EXPORTS 1

#if		!defined(WORLD_EDITOR)
#include <boost/bind.hpp>
#include <boost/function.hpp>
#endif	// WORLD_EDITOR

#include <EngineGUI/EngineGUI.h>

#include "Resource.h"
#include "DlgSelectMode.h"
#include "WndDisplayTexture.h"
#include "DlgChooseTextureType.h"
#include "DlgCreateNormalTexture.h"
#include "DlgCreateAnimatedTexture.h"
#include "DlgCreateEffectTexture.h"

// this is needed for resource setting
#ifndef NDEBUG
  #define ENGINEGUI_DLL_NAME "EngineGUID.dll"
#else
  #define ENGINEGUI_DLL_NAME "EngineGUI.dll"
#endif


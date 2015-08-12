#include "stdh.h"

#include <Engine/Graphics/ViewPort.h>

#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Base/Statistics_internal.h>
#include <Engine/Interface/UIIME.h>
extern INDEX ogl_bExclusive;
//	김영환 전역 설정 값 가져오기
extern BOOL _bClientApp;

// helper for D3D surface
static HRESULT CreateSwapChain_D3D( CViewPort *pvp, PIX pixSizeI, PIX pixSizeJ)
{
	HRESULT hr;

	// release old if still allocated
	if( pvp->vp_pSwapChain!=NULL) {
		ASSERT( pvp->vp_pSurfDepth!=NULL);
		D3DRELEASE( pvp->vp_pSwapChain, TRUE);
		D3DRELEASE( pvp->vp_pSurfDepth, TRUE);
	} else ASSERT(pvp->vp_pSurfDepth==NULL);

	// prepare new parameters
	D3DPRESENT_PARAMETERS d3dPresentParams;
	memset( &d3dPresentParams, 0, sizeof(d3dPresentParams));
	d3dPresentParams.Windowed = TRUE;
	d3dPresentParams.BackBufferWidth  = pixSizeI;
	d3dPresentParams.BackBufferHeight = pixSizeJ;
	d3dPresentParams.BackBufferFormat = _pGfx->gl_d3dColorFormat;
	d3dPresentParams.BackBufferCount  = 1;
	d3dPresentParams.MultiSampleType  = D3DMULTISAMPLE_NONE; // !!!! TODO
	d3dPresentParams.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	d3dPresentParams.hDeviceWindow = pvp->vp_hWnd;

	// WSS_VIDEOMEMORYFAIL 070615 ------------------------------->><<
	// 비디오 메모리 에러시 메세지 출력을 위해
	// create!
	hr = _pGfx->gl_pd3dDevice->CreateAdditionalSwapChain( &d3dPresentParams, &pvp->vp_pSwapChain);
	if(hr) return hr;
	//D3D_CHECKERROR(hr);	
	hr = _pGfx->gl_pd3dDevice->CreateDepthStencilSurface( pixSizeI, pixSizeJ, _pGfx->gl_d3dDepthFormat,D3DMULTISAMPLE_NONE, &pvp->vp_pSurfDepth);
	if(hr) return hr;
	//D3D_CHECKERROR(hr);

	// done
	ASSERT( pvp->vp_pSwapChain!=NULL && pvp->vp_pSurfDepth!=NULL);
	return hr;
}


static void SetAsRenderTarget_D3D( CViewPort *pvp)
{
	HRESULT hr;
	LPDIRECT3DSURFACE8 pColorSurface;

	if( pvp->vp_pSwapChain != NULL)
	{
		hr = pvp->vp_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pColorSurface);  D3D_CHECKERROR(hr);
		hr = _pGfx->gl_pd3dDevice->SetRenderTarget( pColorSurface, pvp->vp_pSurfDepth);       D3D_CHECKERROR(hr);
		D3DRELEASE( pColorSurface, TRUE);
	}
}


// helper for OGL

CTempDC::CTempDC(HWND hWnd)
{
	ASSERT(hWnd!=NULL);
	hwnd = hWnd;
	hdc = GetDC(hwnd);
	ASSERT(hdc!=NULL);
}

CTempDC::~CTempDC(void)
{
	ReleaseDC(hwnd, hdc);
}


/*
*   ViewPort functions
*/

CViewPort::CViewPort( PIX pixWidth, PIX pixHeight, HWND hWnd) :
vp_Raster( pixWidth, pixHeight, 0)
{
	vp_hWnd = NULL;
	vp_hWndParent = hWnd;
	vp_pSwapChain = NULL;
	vp_pSurfDepth = NULL;
	vp_ctDisplayChanges = 0;
	OpenCanvas();
	vp_Raster.ra_pvpViewPort = this;
}


CViewPort::~CViewPort(void)
{
	extern void DrawPort_NoCurrentDrawPort(void);
	DrawPort_NoCurrentDrawPort();
	CloseCanvas();
	// reset current viewport if needed
	if( _pGfx->gl_pvpActive==this) _pGfx->gl_pvpActive = NULL;
}




#define CViewPortCLASS "ViewPort Window"
static BOOL _bClassRegistered = FALSE;

LRESULT CALLBACK CViewPortCLASS_WindowProc(
	HWND hWnd,      // handle to window
	UINT Msg,       // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
										   )
{
	// forget erase bacground messages
	if (Msg==WM_ERASEBKGND) return TRUE;

	// if any mouse message
	if ((Msg>=WM_MOUSEFIRST&&Msg<=WM_MOUSELAST)) {
		// send it to parent
		HWND hWndParent = GetParent(hWnd);
		ASSERT(hWndParent!=NULL);
		return CallWindowProc( (WNDPROC)GetWindowLong(hWndParent, GWL_WNDPROC),
													 hWndParent, Msg, wParam, lParam);
	}
	// 이기환 수정 시작 (11. 15) : FullScreen에서 IME창 숨기기
	return _bIMEProc?0:DefWindowProc(hWnd, Msg, wParam, lParam);
	// 이기환 수정 끝 (11. 15)
}



// open overlaid window for rendering context
void CViewPort::OpenCanvas(void)
{
	// 김영환 : vp_hWndParent 을 vp_hWnd로 사용	
	if(_bClientApp)
	{
		vp_hWnd = vp_hWndParent;	// 메인 핸들 사용.
		// 윈도우 영역 및 플 스크린 정보 얻는다.
		// determine window and desktopsize
		RECT rectWindow;
		GetClientRect( vp_hWndParent, &rectWindow);
		const PIX pixWinSizeI = rectWindow.right  - rectWindow.left;
		const PIX pixWinSizeJ = rectWindow.bottom - rectWindow.top;
		CDisplayMode dm;
		_pGfx->GetCurrentDisplayMode(dm);
		ASSERT( (dm.dm_pixSizeI==0 && dm.dm_pixSizeJ==0) || (dm.dm_pixSizeI!=0 && dm.dm_pixSizeJ!=0));
		const BOOL bFullScreen = (dm.dm_pixSizeI==pixWinSizeI && dm.dm_pixSizeJ==pixWinSizeJ);

		// prepare new swap chain for D3D
		if( _pGfx->gl_eCurrentAPI==GAT_D3D && !bFullScreen)
		{
			// WSS_VIDEOMEMORYFAIL 070615 ------------------------------->>
			HRESULT hr;
			if( (hr=CreateSwapChain_D3D( this, pixWinSizeI, pixWinSizeJ)))
			{
			}
			// ----------------------------------------------------------->>
		}

		// resize raster
		Resize();
		ShowWindow( vp_hWnd, SW_SHOW);

		// set as rendering target
		if( _pGfx->gl_eCurrentAPI==GAT_D3D && vp_pSwapChain!=NULL) 
			SetAsRenderTarget_D3D(this);
	}
	else
	{
		// do nothing if not feasable
		if( vp_hWnd!=NULL || vp_hWndParent==NULL) return;

		// register class
		if( !_bClassRegistered) {
			WNDCLASS wc;
			wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
			wc.lpfnWndProc = CViewPortCLASS_WindowProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = NULL;
			wc.hIcon = NULL;
			wc.hCursor = LoadCursor( NULL, IDC_ARROW);
			wc.hbrBackground = NULL;
			wc.lpszMenuName = NULL;
			wc.lpszClassName = CViewPortCLASS;
			RegisterClass(&wc);
			_bClassRegistered = TRUE;
		}
		
		// determine window and desktopsize
		RECT rectWindow;
		GetClientRect( vp_hWndParent, &rectWindow);
		const PIX pixWinSizeI = rectWindow.right  - rectWindow.left;
		const PIX pixWinSizeJ = rectWindow.bottom - rectWindow.top;
		CDisplayMode dm;
		_pGfx->GetCurrentDisplayMode(dm);
		ASSERT( (dm.dm_pixSizeI==0 && dm.dm_pixSizeJ==0) || (dm.dm_pixSizeI!=0 && dm.dm_pixSizeJ!=0));
		const BOOL bFullScreen = (dm.dm_pixSizeI==pixWinSizeI && dm.dm_pixSizeJ==pixWinSizeJ);

		// set fullscreen attribs if window size is equal to screen size
		DWORD dwExStyle = NONE;
		DWORD dwStyle   = WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
		if( bFullScreen && ogl_bExclusive) {
			dwExStyle = WS_EX_TOPMOST;
			dwStyle   = WS_POPUP;     
		} 

		// set child window
		vp_hWnd = ::CreateWindowEx(
		  dwExStyle,
		  CViewPortCLASS,
		  "",   // title
			dwStyle,
		  0,0,
		  0,0,  // window size
		  vp_hWndParent,
		  NULL,
		  (HINSTANCE)GetWindowLong(vp_hWndParent, GWL_HINSTANCE),
		  NULL);
		ASSERT( vp_hWnd!=NULL);
		
		// prepare new swap chain for D3D
		if( _pGfx->gl_eCurrentAPI==GAT_D3D && !bFullScreen)
		{
			// WSS_VIDEOMEMORYFAIL 070615 ------------------------------->>
			HRESULT hr;
			if( (hr=CreateSwapChain_D3D( this, pixWinSizeI, pixWinSizeJ)))
			{
				/*****
				CTString tStr;
				tStr.PrintF("CreateSwapChain_D3D Failed!! - OpenCanvas %x",hr);
				MessageBox(vp_hWndParent,tStr.str_String,"ERROR",MB_OK);
				::DestroyWindow(vp_hWnd);
				return;
				*****/
			}
			// ----------------------------------------------------------->>
		}

		// resize raster
		Resize();
		ShowWindow( vp_hWnd, SW_SHOW);

		// set as rendering target
		if( _pGfx->gl_eCurrentAPI==GAT_D3D && vp_pSwapChain!=NULL) SetAsRenderTarget_D3D(this);
	}
}


// close overlaid window
void CViewPort::CloseCanvas(void)
{
	// release D3D swap chain if allocated
	if( _pGfx->gl_eCurrentAPI==GAT_D3D) 
	{
		LPDIRECT3DSURFACE8	pBackSurface = NULL; 
		_pGfx->gl_pd3dDevice->GetRenderTarget( &pBackSurface );
		if( pBackSurface == vp_pSurfDepth )
			_pGfx->gl_pd3dDevice->SetRenderTarget(NULL, NULL);
		D3DRELEASE( pBackSurface, TRUE);
		if( vp_pSwapChain!=NULL) D3DRELEASE( vp_pSwapChain, TRUE);
		if( vp_pSurfDepth!=NULL) D3DRELEASE( vp_pSurfDepth, TRUE);
	} else {
		vp_pSwapChain = NULL;
		vp_pSurfDepth = NULL;
	}
	// destroy window
	//	김영환 : 클라이언트용이 아닌 경우만.(툴에서 사용하는 경우)
	if(_bClientApp == FALSE)
	{
		if( vp_hWnd!=NULL && IsWindow(vp_hWnd)) 
		{ 
			BOOL bRes = DestroyWindow(vp_hWnd);
			ASSERT(bRes);
		}
	}
	// mark
	vp_hWnd = NULL;
}


// Change size of this viewport, it's raster and all it's drawports
void CViewPort::Resize(void)
{
	PIX pixNewWidth, pixNewHeight;
	RECT rectWindow;

	//강동민 수정 시작 시스템 마우스 작업	09.09
	extern BOOL _bFirst;
	if(!_bFirst)				_bFirst = TRUE;

	extern INDEX d3d_bDeviceChanged;
	if(!d3d_bDeviceChanged)		d3d_bDeviceChanged = TRUE;
	//강동민 수정 끝 시스템 마우스 작업		09.09
	
	// get the size of the window
	GetClientRect( vp_hWndParent, &rectWindow);
	pixNewWidth  = rectWindow.right  - rectWindow.left;
	pixNewHeight = rectWindow.bottom - rectWindow.top;
	//안태훈 수정 시작	//(Easy Use World Editor)(0.1)
	if(pixNewWidth < 1) pixNewWidth = 1;
	if(pixNewHeight < 1) pixNewHeight = 1;
	//안태훈 수정 끝	//(Easy Use World Editor)(0.1)

	// resize child window
	//	김영환 : 크기조절 안함.
	if(_bClientApp == FALSE)
	{
		ASSERT( vp_hWnd!=NULL);
		SetWindowPos( vp_hWnd, NULL, 0,0, pixNewWidth, pixNewHeight, SWP_NOZORDER|SWP_NOMOVE);
	}

	// resize the raster
	vp_Raster.Resize( pixNewWidth, pixNewHeight);

	// "resize" D3D surface (if any)
	if( _pGfx->gl_eCurrentAPI==GAT_D3D && vp_pSwapChain!=NULL)// && vp_pSurfDepth!=NULL) 
	{
		// release old surface
		D3DRELEASE( vp_pSwapChain, TRUE);
		D3DRELEASE( vp_pSurfDepth, TRUE);
		// create a new one and set it as current
		// WSS_VIDEOMEMORYFAIL 070615 ------------------------------->>
		HRESULT hr;
		if( (hr=CreateSwapChain_D3D( this, pixNewWidth, pixNewHeight)) )
		{
			/*****
			CTString tStr;
			tStr.PrintF("CreateSwapChain_D3D Failed!! - Resize %x",hr);
			MessageBox(vp_hWndParent,tStr.str_String,"ERROR",MB_OK);			
			return;
			*****/
		}				
		// ----------------------------------------------------------->>
		SetAsRenderTarget_D3D(this);
	}
}


void CViewPort::SwapBuffers( const BOOL bWaitForRetrace/*=FALSE*/)
{
	// skip if child window not present
	if( vp_hWnd==NULL) return;
	// ask the gfx lib to swap buffers
	_pGfx->SwapBuffers( this, bWaitForRetrace);
}


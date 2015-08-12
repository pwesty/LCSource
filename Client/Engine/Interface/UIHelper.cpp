#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIHelper.h>
#include <algorithm>

#define MAX_HELPER_LEVEL				(5)
#define BUTTON_SIZE						(140)
#define NEW_BUTTON_SIZE					(160)
enum eSelection
{
	SEL_HELPER_TEACHER,
	SEL_HELPER_STUDENT,
};

#define	HELPER_TAB_WIDTH				(120)
#define HELPER_BUTTON_START_Y			(HELPER_HEIGHT - 80)
#define HELPER_LIST_BOX_WIDTH			(223)
#define HELPER_LIST_BOX_HEIGHT			(198)

#define INFOMARK_SIZE 16
#define INFOMARK_TAB_SIZE 32
#define INFOMARK_Y_POS 46

#define SCROLLBAR_GAP 12

#define TAB_WIDTH 234
#define TAB_POS_LEFT 7

#define TAB3_SIZE0 114
#define TAB3_SIZE1 76
#define TAB3_SIZE2 (32 + SCROLLBAR_GAP)
#define TAB3_POS0 0
#define TAB3_POS1 TAB3_SIZE0
#define TAB3_POS2 (TAB3_SIZE0 + TAB3_SIZE1)
#define TAB3_POS3 (TAB3_SIZE0 + TAB3_SIZE1 + TAB3_SIZE2)

#define TAB4_SIZE0 98
#define TAB4_SIZE1 32
#define TAB4_SIZE2 60
#define TAB4_SIZE3 (32 + SCROLLBAR_GAP)
#define TAB4_POS0 0
#define TAB4_POS1 TAB4_SIZE0
#define TAB4_POS2 (TAB4_SIZE0 + TAB4_SIZE1)
#define TAB4_POS3 (TAB4_SIZE0 + TAB4_SIZE1 + TAB4_SIZE2)
#define TAB4_POS4 (TAB4_SIZE0 + TAB4_SIZE1 + TAB4_SIZE2 + TAB4_SIZE3)

#define STUDENT_BACK_WIDTH	287
#define STUDENT_BACK_HEIGHT	34

enum ETeach_Surface
{
	BG_UL		= 0,
	BG_UM		= 1,
	BG_UR		= 2,
	BG_ML		= 3,
	BG_MM		= 4,
	BG_MR		= 5,
	BG_BL		= 6,
	BG_BM		= 7,
	BG_BR		= 8,
};
// ----------------------------------------------------------------------------
// Name : CUIHelper()
// Desc : 
// ----------------------------------------------------------------------------
CUIHelper::CUIHelper()
{
	m_eHelperMenuState		= HELPER_MENU_REQ;
	m_bRegistredTeacher		= FALSE;

	m_nMyCurTeachCnt		= 0;
	m_nMyCompleteTeachCnt	= 0;
	m_nMyFailTeachCnt		= 0;
	m_strClassName			= CTString("");
}

// ----------------------------------------------------------------------------
// Name : ~CUIHelper()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIHelper::~CUIHelper()
{
	if( !m_vectorHelperList.empty() )
	{
		m_vectorHelperList.clear();
	}
	if ( !m_vectorTeacherInfoList.empty() )
	{
		m_vectorTeacherInfoList.clear();
	}
	if ( !m_vectorStudentInfoList.empty() )
	{
		m_vectorStudentInfoList.clear();
	}
	m_nMyCurTeachCnt		= 0;
	m_nMyCompleteTeachCnt	= 0;
	m_nMyFailTeachCnt		= 0;
	m_strClassName			= CTString("");

	Destroy();
	STOCK_RELEASE(m_ptdSubTexture);
	STOCK_RELEASE(m_ptdBtnTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int		i;
	m_pParent = (CUIBase*)pParentWnd;
	SetSize( NEW_HELPER_WIDTH, 335 );
	m_rcTitle.SetRect( 0, 0, NEW_HELPER_WIDTH, 22 );
	// Texture Data
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	m_ptdSubTexture = CreateTexture( CTString( "Data\\Interface\\New_Interface.tex" ) );
	m_ptdBtnTexture	 = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	
	//------------------------------------------------------------------
	// New_Interface.tex
	FLOAT	fTexWidth = m_ptdSubTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdSubTexture->GetPixHeight();	
	// BackGround
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,98,8,137,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(8,98,265,137,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(265,98,273,137,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,137,8,245,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(8,137,265,245,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,8,0), UIRectUV(265,137,273,245,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(0,246,8,254,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(8,246,265,254,fTexWidth,fTexHeight));
	m_NewBack.AddRectSurface(UIRect(0,0,0,0), UIRectUV(265,246,273,254,fTexWidth,fTexHeight));
	
	m_rtNewBack[BG_UL].SetRect(0,0,8,39);
	m_rtNewBack[BG_UM].SetRect(8,0,NEW_HELPER_WIDTH-8,39);
	m_rtNewBack[BG_UR].SetRect(NEW_HELPER_WIDTH-8,0,NEW_HELPER_WIDTH,39);
	m_rtNewBack[BG_ML].SetRect(0,39,8,327);
	m_rtNewBack[BG_MM].SetRect(8,39,NEW_HELPER_WIDTH-8,327);
	m_rtNewBack[BG_MR].SetRect(NEW_HELPER_WIDTH-8,39,NEW_HELPER_WIDTH,327);
	m_rtNewBack[BG_BL].SetRect(0,327,8,335);
	m_rtNewBack[BG_BM].SetRect(8,327,NEW_HELPER_WIDTH-8,335);
	m_rtNewBack[BG_BR].SetRect(NEW_HELPER_WIDTH-8,327,NEW_HELPER_WIDTH,335);
	
	// New Sub BackGround
	m_NewSubBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(446,239,618,273,fTexWidth,fTexHeight));
	m_NewSubBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(446,239,618,273,fTexWidth,fTexHeight));
	m_NewSubBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(446,239,618,273,fTexWidth,fTexHeight));
	m_NewSubBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(446,239,618,273,fTexWidth,fTexHeight));
	m_NewSubBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(446,239,618,273,fTexWidth,fTexHeight));
	m_NewSubBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(446,239,618,273,fTexWidth,fTexHeight));

	int nPosY1 = 45;
	int nPosY2 = 54;
	for( i = 0; i < 6; ++i )
	{
		m_rtStudentBack[i].SetRect(6, nPosY1, 6+STUDENT_BACK_WIDTH, nPosY1+STUDENT_BACK_HEIGHT);
		m_rtTeacherBack[i].SetRect(261, nPosY2, 261+163, nPosY2+STUDENT_BACK_HEIGHT);
		nPosY1 += 33;
		nPosY2 += 33;
	}

	// New Info BackGround
	m_NewInfoBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(0,473,96,489,fTexWidth,fTexHeight));
	m_NewInfoBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(0,473,96,489,fTexWidth,fTexHeight));
	m_NewInfoBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(0,473,96,489,fTexWidth,fTexHeight));
	m_NewInfoBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(0,473,96,489,fTexWidth,fTexHeight));
	m_NewInfoBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(0,473,96,489,fTexWidth,fTexHeight));
	m_NewInfoBack.AddRectSurface(UIRect(0, 0, 0, 0), UIRectUV(0,473,96,489,fTexWidth,fTexHeight));

	nPosY1 = 51;
	nPosY2 = 60;
	for( i = 0; i < 6; ++i )
	{
		m_rtStudentInfo[i].SetRect(124, nPosY1, 284, nPosY1+21);
		m_rtTeacherInfo[i].SetRect(384, nPosY2, 419, nPosY2+21);
		nPosY1 += 33;
		nPosY2 += 33;
	}
	m_rcTabLineUV.SetUV(340, 463, 349, 464, fTexWidth, fTexHeight);
	m_rtTabLine.SetRect(21, 54, 400, 55);
	//tool-tip
	m_rtInfoUL.SetUV( 446, 239, 456, 249, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 456, 239, 608, 249, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 608, 239, 618, 249, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 446, 249, 456, 263, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 456, 249, 608, 263, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 608, 249, 618, 263, fTexWidth, fTexHeight );	
	m_rtInfoLL.SetUV( 446, 263, 456, 273, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 456, 263, 608, 273, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 608, 263, 618, 273, fTexWidth, fTexHeight );

	//------------------------------------------------------------------
	// CommonBtn.tex
	fTexWidth  = m_ptdBtnTexture->GetPixWidth();
	fTexHeight = m_ptdBtnTexture->GetPixHeight();	
	
	int nButtonY = HELPER_BUTTON_START_Y + 10 ;
	// Button : 후견인 목록 갱신
	m_btnRefreshTeacher.Create( this, _S( 1135, "후견인 목록 갱신" ), 38, nButtonY, NEW_BUTTON_SIZE, 21 );
	m_btnRefreshTeacher.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnRefreshTeacher.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnRefreshTeacher.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnRefreshTeacher.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnRefreshTeacher.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnRefreshTeacher.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnRefreshTeacher.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnRefreshTeacher.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnRefreshTeacher.SetNewType(TRUE);
	// Button : 후견인 등록
	m_btnRegisterTeacher.Create( this, _S( 1136, "후견인 등록" ), 38, nButtonY, NEW_BUTTON_SIZE, 21 );		
	m_btnRegisterTeacher.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnRegisterTeacher.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnRegisterTeacher.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnRegisterTeacher.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnRegisterTeacher.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnRegisterTeacher.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnRegisterTeacher.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnRegisterTeacher.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnRegisterTeacher.SetNewType(TRUE);
	// Button : 후견인 등록 취소
	m_btnCancelRegister.Create( this, _S( 1137, "후견인 등록 취소" ), 38, nButtonY, NEW_BUTTON_SIZE, 21 );	
	m_btnCancelRegister.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnCancelRegister.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnCancelRegister.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnCancelRegister.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnCancelRegister.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnCancelRegister.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnCancelRegister.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnCancelRegister.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnCancelRegister.SetNewType(TRUE);
	// Button : 견습생 신청
	m_btnBeMyTeacher.Create( this, _S( 1138, "견습생 신청" ), 38 + NEW_BUTTON_SIZE + 38, nButtonY, NEW_BUTTON_SIZE, 21 );
	m_btnBeMyTeacher.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnBeMyTeacher.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnBeMyTeacher.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnBeMyTeacher.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnBeMyTeacher.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnBeMyTeacher.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnBeMyTeacher.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnBeMyTeacher.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnBeMyTeacher.SetNewType(TRUE);
	// Button : 보상 보기
	m_btnGift.Create( this, _S( 1695, "보상" ), 38 + NEW_BUTTON_SIZE + 38, nButtonY, NEW_BUTTON_SIZE, 21 );
	m_btnGift.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnGift.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnGift.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnGift.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnGift.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnGift.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnGift.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnGift.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnGift.SetNewType(TRUE);	

	nButtonY += 30;
	
	// Button : 견습생 포기
	m_btnFireMyTeacher.Create( this, _S( 1139, "견습생 포기"), 38, nButtonY, NEW_BUTTON_SIZE, 21 );	
	m_btnFireMyTeacher.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnFireMyTeacher.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnFireMyTeacher.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnFireMyTeacher.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnFireMyTeacher.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnFireMyTeacher.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnFireMyTeacher.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnFireMyTeacher.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnFireMyTeacher.SetNewType(TRUE);
	// Button : 후견인 포기
	m_btnFireMyStudent.Create( this, _S( 5068, "후견인 포기"), 38, nButtonY, NEW_BUTTON_SIZE, 21 );	
	m_btnFireMyStudent.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnFireMyStudent.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnFireMyStudent.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnFireMyStudent.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnFireMyStudent.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnFireMyStudent.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnFireMyStudent.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnFireMyStudent.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnFireMyStudent.SetNewType(TRUE);
	// Button : 닫기
	m_btnExit.Create( this, _S( 870, "닫기" ), 38 + NEW_BUTTON_SIZE + 38, nButtonY, NEW_BUTTON_SIZE, 21 );	
	m_btnExit.SetRTSurface( UBS_IDLE, UIRect(0,0,16,22), UIRectUV(113,0,129,22, fTexWidth, fTexHeight) );
	m_btnExit.SetRTSurface( UBS_IDLE, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(129,0,167,22, fTexWidth, fTexHeight) );
	m_btnExit.SetRTSurface( UBS_IDLE, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(167,0,183,22, fTexWidth, fTexHeight) );
	m_btnExit.SetRTSurface( UBS_CLICK, UIRect(0,0,16,22), UIRectUV(186,0,202,22, fTexWidth, fTexHeight) );
	m_btnExit.SetRTSurface( UBS_CLICK, UIRect(16,0,NEW_BUTTON_SIZE-16,22), UIRectUV(202,0,240,22, fTexWidth, fTexHeight) );
	m_btnExit.SetRTSurface( UBS_CLICK, UIRect(NEW_BUTTON_SIZE-16,0,NEW_BUTTON_SIZE,22), UIRectUV(240,0,256,22, fTexWidth, fTexHeight) );
	m_btnExit.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnExit.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnExit.SetNewType(TRUE);
	// Button : X (Close)
	m_btnClose.Create( this, CTString( "" ), 410, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	//------------------------------------------------------------------
	// MessageBox.tex
	fTexWidth  = m_ptdBaseTexture->GetPixWidth();
	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// ListBox : 후견인 목록
	m_lbTeacherList.Create( this, 8, 55, NEW_TAB_CLASS+NEW_TAB_NAME+NEW_TAB_TIME+NEW_TAB_COUNT+NEW_TAB_INFO, HELPER_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight()+5, 13, 3, 5, TRUE );
	m_lbTeacherList.SetSelBar( NEW_TAB_CLASS+NEW_TAB_NAME+NEW_TAB_TIME+NEW_TAB_COUNT+NEW_TAB_INFO, _pUIFontTexMgr->GetLineHeight()+5, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbTeacherList.SetColumnPosX( 0, 0 );
	m_lbTeacherList.SetColumnPosX( 1, NEW_TAB_CLASS );
	m_lbTeacherList.SetColumnPosX( 2, NEW_TAB_CLASS+NEW_TAB_NAME );
	m_lbTeacherList.SetColumnPosX( 3, NEW_TAB_CLASS+NEW_TAB_NAME+NEW_TAB_TIME);
	m_lbTeacherList.SetColumnPosX( 4, NEW_TAB_CLASS+NEW_TAB_NAME+NEW_TAB_TIME+NEW_TAB_COUNT );
	m_lbTeacherList.CreateScroll( TRUE, 0, 0, 9, HELPER_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_lbTeacherList.SetOverColor( 0xF8E1B5FF );
	m_lbTeacherList.SetSelectColor( 0xF8E1B5FF );
	// Up button
	m_lbTeacherList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbTeacherList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbTeacherList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbTeacherList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbTeacherList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbTeacherList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbTeacherList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbTeacherList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	
	
	// ListBox : 견습생 목록
	m_lbStudentList.Create( this, 8, 55, 236, HELPER_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight()+5, 13, 3, 3, TRUE );
	m_lbStudentList.SetSelBar( 236, _pUIFontTexMgr->GetLineHeight()+5, 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbStudentList.SetColumnPosX( 0, 0 );
	m_lbStudentList.SetColumnPosX( 1, NEW_TAB_CLASS );
	m_lbStudentList.SetColumnPosX( 2, 215, TEXT_RIGHT );
	m_lbStudentList.CreateScroll( TRUE, 0, 0, 9, HELPER_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );
	m_lbStudentList.SetOverColor( 0xF8E1B5FF );
	m_lbStudentList.SetSelectColor( 0xF8E1B5FF );
	// Up button
	m_lbStudentList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbStudentList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbStudentList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbStudentList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbStudentList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbStudentList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbStudentList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbStudentList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	//info mark
	m_rtInfoMarkUV.SetUV(159, 162, 175, 178, fTexWidth, fTexHeight);
	for( i = 0; i < m_lbStudentList.GetScrollBarItemsPerPage(); ++i )
	{
		m_rcInfoMark[i].SetRect(
			TAB_POS_LEFT + NEW_TAB_CLASS+NEW_TAB_NAME+NEW_TAB_TIME+NEW_TAB_COUNT + (NEW_TAB_INFO - INFOMARK_SIZE) / 2
			, 57 + i * m_lbTeacherList.GetLineHeight()
			, TAB_POS_LEFT + NEW_TAB_CLASS+NEW_TAB_NAME+NEW_TAB_TIME+NEW_TAB_COUNT + (NEW_TAB_INFO - INFOMARK_SIZE) / 2 + INFOMARK_SIZE
			, 57 + i * m_lbTeacherList.GetLineHeight() + INFOMARK_SIZE);

		m_rcClass[i].SetRect(
			TAB_POS_LEFT + (NEW_TAB_INFO - INFOMARK_SIZE) / 2 + 7
			, 57 + i * m_lbTeacherList.GetLineHeight()
			, TAB_POS_LEFT + (NEW_TAB_INFO - INFOMARK_SIZE) / 2 + INFOMARK_SIZE + 7
			, 57 + i * m_lbTeacherList.GetLineHeight() + INFOMARK_SIZE);
	}
	m_rtClass[TITAN].SetUV( 255, 179, 275, 199, fTexWidth, fTexHeight );
	m_rtClass[KNIGHT].SetUV( 280, 179, 300, 199, fTexWidth, fTexHeight );
	m_rtClass[HEALER].SetUV( 305, 179, 325, 199, fTexWidth, fTexHeight );
	m_rtClass[MAGE].SetUV( 255, 204, 275, 224, fTexWidth, fTexHeight );
	m_rtClass[ROGUE].SetUV( 280, 204, 300, 224, fTexWidth, fTexHeight );
	m_rtClass[SORCERER].SetUV( 305, 204, 325, 224, fTexWidth, fTexHeight );
	m_rtClass[NIGHTSHADOW].SetUV( 472, 132, 492, 152, fTexWidth, fTexHeight );
#ifdef CHAR_EX_ROGUE
	m_rtClass[EX_ROGUE].SetUV( 280, 204, 300, 224, fTexWidth, fTexHeight );	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_rtClass[EX_MAGE].SetUV( 255, 204, 275, 224, fTexWidth, fTexHeight );	//2013/01/08 jeil EX메이지 추가 
#endif
	m_bShowInfo = FALSE;
	m_nCurInfoLines = 0;
	m_bShowClass = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenHelper()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::OpenHelper( )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxLExist( MSGLCMD_HELPER_REQ ) || IsVisible())
		return;	

	ResetHelper();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYSTUDENT );

	CTString strTitle;
	CTString strMessage;

	const int iLevel	= _pNetwork->MyCharacterInfo.level;

	if( iLevel >=  50 )
	{
		strTitle = _S( 1140, "견습생 관리" );			
		// Create message box
		pUIManager->CreateMessageBoxL( strTitle, UI_HELPER, MSGLCMD_HELPER_REQ );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );
		
		strMessage.PrintF( _S( 1141, "게임을 시작하는 유저들을 도와주기 위해서 후견인을 등록할 수 있습니다. 후견인 등록을 하면, 신규 유저가 후견인 리스트를 볼 때 [%s]님의 이름이 보이게 됩니다." ), _pNetwork->MyCharacterInfo.name );		
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			
		
		strMessage.PrintF( _S( 5045, "견습생이 50레벨이 되는 동안, 다양한 보상을 얻을 수 있습니다." ), GetLimitLevel() );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );		
		
		strMessage.PrintF( "" );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );
		
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "확인하기." ), SEL_HELPER_TEACHER );		
	}
	else
	{
		strTitle = _S( 1147, "후견인 관리" );			
		// Create message box
		pUIManager->CreateMessageBoxL( strTitle, UI_HELPER, MSGLCMD_HELPER_REQ );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );
		
		strMessage.PrintF( _S( 1148, "후견인은 [%s]님을 %d레벨까지 성장하는데 도움을 주는 사람을 이야기합니다. 후견인 리스트에서 [%s]님께 도움을 줄 수 있는 사람을 찾아보세요." ), _pNetwork->MyCharacterInfo.name, GetLimitLevel(), _pNetwork->MyCharacterInfo.name );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			
		
		strMessage.PrintF( _S( 5044, "최초 선정까지 총 3번을 선정할 수가 있으며, 30레벨이 넘으면 후견인을 선정 할수가 없으니 참고하시기 바랍니다." ), _pNetwork->MyCharacterInfo.name );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			
		
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "확인하기." ), SEL_HELPER_STUDENT );		
	}
	
	pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE, _S( 880, "취소하기." ) );				
}

static LONG g_lChaIndex				= -1;		// 가입 신청과 관련된 부분에만 쓰임.
static CTString g_strTeacherName	= CTString("");

// ----------------------------------------------------------------------------
// Name : ResetHelper()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::ResetHelper()
{
	m_lbTeacherList.ResetAllStrings();
	m_lbStudentList.ResetAllStrings();

	m_eHelperMenuState		= HELPER_MENU_REQ;
	g_lChaIndex				= -1;
	g_strTeacherName.Clear();

	// NOTE : 길드를 탈퇴하거나 해체 하지 않는 이상, 클리어 하면 안될거 같음.
	//ClearHelperList();

	CUIManager::getSingleton()->RearrangeOrder( UI_HELPER, FALSE );
}

// ----------------------------------------------------------------------------
// Name : ClearHelperList()
// Desc : 멤버 목록을 클리어합니다.
// ----------------------------------------------------------------------------
void CUIHelper::ClearHelperList()
{
	// NOTE : 길드를 탈퇴하거나 해체 하지 않는 이상, 클리어 하면 안될거 같음.
	if( !m_vectorHelperList.empty() )
	{
		m_vectorHelperList.clear();
		m_vectorTeacherInfoList.clear();
		m_vectorStudentInfoList.clear();
		m_lbTeacherList.ResetAllStrings();
		m_lbStudentList.ResetAllStrings();
	}
}

//static LONG g_lHelperIndex	= -1;

// ----------------------------------------------------------------------------
// Name : BeMyTeacher()
// Desc : 견습생이 후견인이 되어달라고 요청함.
// ----------------------------------------------------------------------------
void CUIHelper::BeMyTeacher( )
{
	int	iSelMember = m_lbTeacherList.GetCurSel();
	if( iSelMember == -1 )
		return;

	sHelperMember &TempMember = m_vectorHelperList[iSelMember];

	g_lChaIndex		= TempMember.lIndex;
	g_strTeacherName= TempMember.strName;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1150, "후견인 신청" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_BEMYTEACHER );	
	strMessage.PrintF( _S( 1151, "[%s]님을 후견인으로 신청하시겠습니까?" ), TempMember.strName );			
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : SetRegistredTeacher()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::SetRegistredTeacher( BOOL bRegistered )
{
	m_bRegistredTeacher = bRegistered;
}

// ----------------------------------------------------------------------------
// Name : FireMyTeacher()
// Desc : 나 니 학생 안할란다...
// ----------------------------------------------------------------------------
void CUIHelper::FireMyTeacher( )
{
	ASSERT( _pNetwork->MyCharacterInfo.lTeacherIndex != -1 );

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );
	
	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1152, "견습생 포기" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_FIRE_MYTEACHER );		
	strMessage.PrintF( _S( 1153, "[%s]님의 견습생을 그만 두시겠습니까? 한번 그만 두게되면 다시는 후견인을 선정할 수 없습니다." ), _pNetwork->MyCharacterInfo.strTeacherName );	
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF( "" );
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF( _S( 5067, "남은횟수 : %d회" ), 2 - _pNetwork->MyCharacterInfo.iStudentGiveUpCnt );
	MsgBoxInfo.AddString( strMessage, 0x00ffffff );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : TeacherAccept()
// Desc : 후견인이 되어주겠는지 물어봄.
// ----------------------------------------------------------------------------
void CUIHelper::TeacherAccept( LONG lIndex, const CTString& strStudentName )
{
	g_lChaIndex			= lIndex;
	g_strTeacherName	= strStudentName;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_STUDENT_ACCEPT );
	
	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1154, "후견인 등록" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_STUDENT_ACCEPT );	
	strMessage.PrintF( _S( 1155,  "[%s] 님이 후견인이 되어달라는 부탁을 합니다. 수락하시겠습니까?"  ),  strStudentName );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : GetLimitLevel()
// Desc : 견습생과 후견인의 제한 레벨을 얻습니다.
// ----------------------------------------------------------------------------
int CUIHelper::GetLimitLevel()
{
	return 50;
}

// ----------------------------------------------------------------------------
// Name : AddToHelperList()
// Desc : 멤버 목록에 추가
// ----------------------------------------------------------------------------
void CUIHelper::AddToHelperList( LONG lIndex, const CTString& strName, int iLevel, int iJob, int iJob2 )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );
	
	if( iter == m_vectorHelperList.end() )
	{
		if( m_vectorHelperList.size() <= HELPER_MAX_STUDENTS )
		{
			sHelperMember TempHelperMember;
			TempHelperMember.lIndex			= lIndex;
			TempHelperMember.strName		= strName;
			TempHelperMember.iLevel			= iLevel;
			TempHelperMember.iJob			= iJob;
			TempHelperMember.iJob2			= iJob2;
			m_vectorHelperList.push_back(TempHelperMember);
		}
	}
}


// ----------------------------------------------------------------------------
// Name : AddToTeacherInfoList()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::AddToTeacherInfoList(LONG index, SLONG fame, SLONG cntTeaching, SLONG cntComplete, SLONG cntFail, SBYTE StartPlayTime, SBYTE EndPlayTime)
{
	if(m_vectorHelperList.size() == m_vectorTeacherInfoList.size() + 1)
	{
		sTeacherInfo info;
		info.index = index;
		info.nFame = fame;
		info.nCntTeachingStudent = cntTeaching;
		info.nCntCompleteStudent = cntComplete;
		info.nCntFailStudent = cntFail;
		info.sbStartPlayTime= StartPlayTime;
		info.sbEndPlayTime	= EndPlayTime;
		m_vectorTeacherInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorTeacherInfoList.size());
}

// ----------------------------------------------------------------------------
// Name : AddToStudentInfoList()
// Desc : 
// ----------------------------------------------------------------------------
void CUIHelper::AddToStudentInfoList(LONG index, const char *szFirstDate, const char *szFinalDate)
{
	if(m_vectorHelperList.size() == m_vectorStudentInfoList.size() + 1)
	{
		sStudentInfo info;
		info.index = index;
		strcpy(info.szFirstDate, szFirstDate);
		strcpy(info.szFinalDate, szFinalDate);
		m_vectorStudentInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorStudentInfoList.size());
}

// ----------------------------------------------------------------------------
// Name : DelFromHelperList()
// Desc : 멤버 목록에서 제거
// ----------------------------------------------------------------------------
void CUIHelper::DelFromHelperList( LONG lIndex )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		int dist = std::distance(m_vectorHelperList.begin(), iter);
		m_vectorHelperList.erase( iter );
		if(m_vectorTeacherInfoList.size() > 0)
		{
			m_vectorTeacherInfoList.erase(m_vectorTeacherInfoList.begin() + dist);
		}
		else if(m_vectorStudentInfoList.size() > 0)
		{
			m_vectorStudentInfoList.erase(m_vectorStudentInfoList.begin() + dist);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ChangeHelperLevel()
// Desc : 멤버의 레벨을 변경함.
// ----------------------------------------------------------------------------
void CUIHelper::ChangeHelperLevel( LONG lIndex, int iLevel )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		(*iter).iLevel = iLevel;
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshTeacherList()
// Desc : 목록을 갱신합니다.
// ----------------------------------------------------------------------------
void CUIHelper::RefreshTeacherList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbTeacherList.ResetAllStrings();
	std::sort(m_vectorHelperList.begin(), m_vectorHelperList.end());
	std::sort(m_vectorTeacherInfoList.begin(), m_vectorTeacherInfoList.end());
	COLOR		crLevel = 0xAAAAAAFF;	

	CTString strMessage;
	for ( int i=0; i<m_vectorHelperList.size(); ++i)
	{
		// 클래스 Mark 공간
		m_lbTeacherList.AddString( 0, CTString(" ") );
		// 이름
		m_lbTeacherList.AddString( 1, m_vectorHelperList[i].strName, 0x00ffffff );
		// 주접속 시간
		strMessage.PrintF(_S( 5069, "%d시 ~ %d시"), m_vectorTeacherInfoList[i].sbStartPlayTime, m_vectorTeacherInfoList[i].sbEndPlayTime );
		m_lbTeacherList.AddString( 2, strMessage, 0x99ff33ff);
		// 양성 수
		strMessage.PrintF(_S(5070, "%d명"), m_vectorTeacherInfoList[i].nCntCompleteStudent);
		m_lbTeacherList.AddString( 3, strMessage, 0x99ff33ff);
		// info mark용 공간
		m_lbTeacherList.AddString( 4, CTString(" ") );		
	}
	PrepareOpen();
}

// ----------------------------------------------------------------------------
// Name : RefreshStudentList()
// Desc : 목록을 갱신합니다.
// ----------------------------------------------------------------------------
void CUIHelper::RefreshStudentList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbStudentList.ResetAllStrings();
	std::sort(m_vectorHelperList.begin(), m_vectorHelperList.end());

	COLOR		crLevel = 0xAAAAAAFF;
	CTString strLevel;
	
	std::vector<sHelperMember>::const_iterator member_end = m_vectorHelperList.end();
	std::vector<sHelperMember>::iterator it = m_vectorHelperList.begin();
	for(it = m_vectorHelperList.begin(); it != member_end; ++it)
	{		
		if((*it).iJob>=0 && (*it).iJob<TOTAL_JOB)
		{
			const int iJob		= (*it).iJob;
			const int iJob2		= (*it).iJob2;
			const int iLevel	= (*it).iLevel;
			strLevel.PrintF( "%d", iLevel );
			m_lbStudentList.AddString( 0, CTString(" "), 0xffcc66FF );						// info mark용 공간
			m_lbStudentList.AddString( 1, (*it).strName, 0xffcc66FF );						// 이름
			m_lbStudentList.AddString( 2, strLevel, 0xffcc66FF );							// 레벨
		} 
		else 
			ASSERTALWAYS("JOB NUMBER must have this range [iJob>=0 && iJob<TOTAL_JOB]!!");
	}

	PrepareOpen();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::Render()
{
	RenderNew();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIHelper::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			//on info mark?
			m_bShowInfo = FALSE;
			m_bShowClass = FALSE;
			int listCount = 0;
			if(m_eHelperMenuState == HELPER_MENU_STUDENT)
				listCount = m_vectorHelperList.size() - m_lbStudentList.GetScrollBarPos();
			else if(m_eHelperMenuState == HELPER_MENU_TEACHER)
				listCount = m_vectorHelperList.size() - m_lbTeacherList.GetScrollBarPos();
	
			for(int i=0; i<listCount; ++i)
			{
				if(IsInsideRect(nX, nY, m_rcInfoMark[i]) && (m_eHelperMenuState == HELPER_MENU_STUDENT && _pNetwork->MyCharacterInfo.lTeacherIndex == -1) )
				{
					m_bShowInfo = TRUE;
					ShowInfo(i, FALSE);
					break;
				}
				if ( IsInsideRect(nX, nY, m_rcClass[i]) )
				{
					m_bShowClass = TRUE;
					ShowClass(i);
					break;
				}
			}
			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// Close Button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL || 
				m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_eHelperMenuState == HELPER_MENU_STUDENT )
			{
				if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnRefreshTeacher.MouseMessage( pMsg ) != WMSG_FAIL )			// 후견인 목록 갱신
					return WMSG_SUCCESS;						
				else if(  _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnBeMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
					return WMSG_SUCCESS;
				else if(  _pNetwork->MyCharacterInfo.lTeacherIndex != -1 && m_btnFireMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
					return WMSG_SUCCESS;
				else if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			else
			{
				// List box
				if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( !m_bRegistredTeacher && m_btnRegisterTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록
					return WMSG_SUCCESS;
				else if( m_bRegistredTeacher && m_btnCancelRegister.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록 취소
					return WMSG_SUCCESS;
				else if ( m_btnGift.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if ( m_vectorHelperList.size()>0 && m_btnFireMyStudent.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL ||
					m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}				
				else if( m_eHelperMenuState == HELPER_MENU_STUDENT )
				{
					if(  _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnRefreshTeacher.MouseMessage( pMsg ) != WMSG_FAIL )			// 후견인 목록 갱신
						return WMSG_SUCCESS;						
					else if(  _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && m_btnBeMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
						return WMSG_SUCCESS;
					else if(  _pNetwork->MyCharacterInfo.lTeacherIndex != -1 && m_btnFireMyTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 견습생
						return WMSG_SUCCESS;
					else if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				else
				{
					// List box
					if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if( !m_bRegistredTeacher && m_btnRegisterTeacher.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록
						return WMSG_SUCCESS;
					else if( m_bRegistredTeacher && m_btnCancelRegister.MouseMessage( pMsg ) != WMSG_FAIL )	// 후견인 등록 취소
						return WMSG_SUCCESS;
					else if ( m_btnGift.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					else if ( m_vectorHelperList.size()>0 && m_btnFireMyStudent.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_HELPER, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// Close button
			if( ((wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL ) ||
				((wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL ) )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetHelper();
				}
				return WMSG_SUCCESS;
			}
			else if( m_eHelperMenuState == HELPER_MENU_STUDENT )
			{					
				if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && ((wmsgResult = m_btnRefreshTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ))
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						// 서버에 목록 갱신 메세지 보내기.
						_pNetwork->TeachRefreshTeacherList();
					}
					return WMSG_SUCCESS;
				}
				else if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 && ( (wmsgResult = m_btnBeMyTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ) )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						// 후견인에게 견습생으로 신청합니다.
						BeMyTeacher();
					}
					return WMSG_SUCCESS;
				}
				else if( _pNetwork->MyCharacterInfo.lTeacherIndex != -1 && ( (wmsgResult = m_btnFireMyTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ) )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						FireMyTeacher();
					}
					return WMSG_SUCCESS;
				}
				// List box
				else if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}					
			}
			else
			{
				// List box
				if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
				else if( !m_bRegistredTeacher && ((wmsgResult = m_btnRegisterTeacher.MouseMessage( pMsg ) ) != WMSG_FAIL ))
				{
					// 선생 목록에 추가함.
					if(wmsgResult == WMSG_COMMAND)
					{
						RegisterTeacher();
					}
					return WMSG_SUCCESS;
				}
				else if( m_bRegistredTeacher && ((wmsgResult = m_btnCancelRegister.MouseMessage( pMsg ) ) != WMSG_FAIL ))
				{
					// 나 선생 안해~!!!
					if(wmsgResult == WMSG_COMMAND)
					{
						CancelRegisterTeacher();
					}
					return WMSG_SUCCESS;
				}
				else if ( m_btnGift.MouseMessage( pMsg ) != WMSG_FAIL )
				{	// 보상 요청
					_pNetwork->TeachGiftReq();
					return WMSG_SUCCESS;
				
				}
				else if ( m_btnFireMyStudent.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					FireMyStudent();
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{				
				if( m_eHelperMenuState == HELPER_MENU_STUDENT )
				{
					// List box of member list
					if( m_lbTeacherList.MouseMessage( pMsg ) != WMSG_FAIL )		// 후견인 목록
						return WMSG_SUCCESS;
				}
				else if( m_eHelperMenuState == HELPER_MENU_TEACHER )
				{
					// List box of member list
					if( m_lbStudentList.MouseMessage( pMsg ) != WMSG_FAIL )		// 견습생 목록
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_NOTIFY:		// 에러 발생
		{			
		}
		break;
	case MSGCMD_HELPER_BEMYTEACHER:	// 후견인 신청 중...
		{
			if( bOK == FALSE )
				return;

			CUIManager* pUIManager = CUIManager::getSingleton();

			pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 1162, "후견인 신청" ), UMBS_CANCEL, UI_HELPER, MSGCMD_HELPER_BEMYTEACHER_REQ );	
			strMessage.PrintF( _S( 1163, "%s 님께 후견인이 되어달라는 메시지를 보내고, 수락을 기다리는 중입니다."  ), g_strTeacherName );	
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			_pNetwork->TeachTeacherRequest( g_lChaIndex, g_strTeacherName );
		}
		break;
		
	case MSGCMD_HELPER_BEMYTEACHER_REQ:
		// 학생이 후견인 신청을 취소한 경우.
		{	
			_pNetwork->TeachTeacherReject( TRUE, g_lChaIndex, g_strTeacherName );
		}
		break;

	case MSGCMD_HELPER_FIRE_MYTEACHER:		// 학생이 선생을 때려침(내가 학생일 때지...)
		{
			if( !bOK )
				return;

			// 서버에 메세지를 보냄.
			_pNetwork->TeachTeacherGiveUp( _pNetwork->MyCharacterInfo.lTeacherIndex, _pNetwork->MyCharacterInfo.strTeacherName, 
				g_lChaIndex, g_strTeacherName );
		}
		break;

	case MSGCMD_HELPER_REGISTER:			// 선생 목록에 추가합니다.
		{
			if( !bOK )
				return;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strTemp;
			MsgBoxInfo.SetMsgBoxInfo( _S( 5071, "후견인 시스템"), UMBS_OKCANCEL | UMBS_INPUTBOX | UMBS_SECOND_INPUTBOX, UI_HELPER, MSGCMD_HELPER_PLAYTIME, 250);	
			MsgBoxInfo.AddString( _S( 5073, "주로 접속하는 시간을 입력해주세요.") );
			MsgBoxInfo.AddString( _S( 5074, "(견습생이 후견인 선정에 있어 참고가 됩니다.)") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddStringEx( _s("h ~ "), 5, 15 );
			MsgBoxInfo.AddStringEx( _s("h"), 5, 30 );
			MsgBoxInfo.SetInputBox( 5, 5, 2, 50 );
			MsgBoxInfo.SetSEInputBox( 5 , 20, 2, 50 );
			MsgBoxInfo.m_nInputPosY += 4;
			MsgBoxInfo.m_nSEInputPosY += 4;
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		}
		break;

	case MSGCMD_HELPER_CANCEL_REGISTER:		// 선생 목록에서 내림
		{
			if( !bOK )
				return;
			// 서버에 메세지를 보냄.
			_pNetwork->TeachTeacherCancelRegister();
		}
		break;	

	case MSGCMD_HELPER_STUDENT_ACCEPT:
		// 학생이 후견인 신청을 요청한 상태에서...(현재 선생의 입장이지...)
		// 허가냐? 취소냐?
		{
			if( !bOK )		// 학생을 안받아 들임
			{
				_pNetwork->TeachTeacherReject( FALSE, g_lChaIndex, g_strTeacherName );
			}
			else			// 학생 받어~!!!
			{
				_pNetwork->TeachTeacherAccept( _pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name, 
					g_lChaIndex, g_strTeacherName );
			}			
		}
		break;
	case MSGCMD_HELPER_FIRE_MYSTUDENT:
		{	// 선생이 학생 포기
			if ( !bOK )
				return;
			int nPos = m_lbStudentList.GetCurSel() + m_lbStudentList.GetScrollBarPos();
			int nIndex = m_vectorHelperList[nPos].lIndex;
			CTString strStudentName = m_vectorHelperList[nPos].strName;
			_pNetwork->TeachTeacherGiveUp(nIndex, strStudentName, _pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_HELPER_REQ:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			switch ( nResult ) 
			{
			case SEL_HELPER_TEACHER:		// 견습생 관리
				{
					m_eHelperMenuState = HELPER_MENU_TEACHER; 					
					RefreshStudentList( TRUE );
					PrepareOpen();
					pUIManager->RearrangeOrder( UI_HELPER, TRUE );
				}
				break;
				
			case SEL_HELPER_STUDENT:		// 후견인 관리
				{
					m_eHelperMenuState = HELPER_MENU_STUDENT; 
					RefreshTeacherList( TRUE );
					PrepareOpen();
					pUIManager->RearrangeOrder( UI_HELPER, TRUE );										
				}
				break;

			default:		// 취소
				{
					m_eHelperMenuState = HELPER_MENU_REQ;
					pUIManager->RearrangeOrder( UI_HELPER, FALSE );
				}
				break;
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SetFocus()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
}

// ----------------------------------------------------------------------------
// Name : RegisterTeacher()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::RegisterTeacher( )					// 후견인 등록
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1164, "후견인 등록" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_REGISTER );	
	strMessage.PrintF( _S( 1165, "후견인 목록에 등록하시겠습니까?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : CancelRegisterTeacher()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::CancelRegisterTeacher( )			// 후견인 등록 취소
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1166, "후견인 등록 취소" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_CANCEL_REGISTER );	
	strMessage.PrintF( _S( 1167, "후견인 목록에서 삭제하시겠습니까?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : RenderInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::RenderInfo()
{
	if( m_bShowInfo == FALSE && m_bShowClass == FALSE )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdSubTexture );

	// information region
	pDrawPort->AddTexture( m_rcInfo.Left, m_rcInfo.Top,
										m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top,
										m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top,
										m_rcInfo.Right, m_rcInfo.Top + 7,
										m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left, m_rcInfo.Top + 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Top + 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Top + 7,
										m_rcInfo.Right, m_rcInfo.Bottom - 7,
										m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left, m_rcInfo.Bottom - 7,
										m_rcInfo.Left + 7, m_rcInfo.Bottom,
										m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Left + 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right - 7, m_rcInfo.Bottom,
										m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcInfo.Right - 7, m_rcInfo.Bottom - 7,
										m_rcInfo.Right, m_rcInfo.Bottom,
										m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
										0xFFFFFFFF );

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render information

	int	nInfoX, nInfoY= m_rcInfo.Top + 8;
	if ( m_bShowInfo )
	{
		nInfoX = m_rcInfo.Left + 12;

		for( int iInfo = 0; iInfo < m_nCurInfoLines; iInfo++ )
		{
			pDrawPort->PutTextEx( m_strInfo[iInfo], nInfoX, nInfoY, m_colInfo[iInfo] );
			nInfoY += _pUIFontTexMgr->GetLineHeight();
		}
	}
	else if ( m_bShowClass )
	{
		nInfoX = m_rcInfo.Left + 15;
		nInfoY = m_rcInfo.Top + 5;
		pDrawPort->PutTextEx( m_strClassName, nInfoX, nInfoY);
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ShowInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIHelper::ShowInfo(int line, BOOL bRenew)
{
	if(!m_bShowInfo) return;
	int absLine = line;

	if(m_eHelperMenuState == HELPER_MENU_STUDENT)//견습생이 창을 띄움.
	{
		absLine += m_lbStudentList.GetScrollBarPos();
		if(absLine < m_lbStudentList.GetScrollBarPos()
		|| absLine >= m_vectorHelperList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sHelperMember &helpMem = m_vectorHelperList[absLine];
		//TEMP : 초고급 제련석 관련
		LONG i;
		for(i = 0; i < m_vectorTeacherInfoList.size(); ++i)
		{
			if(helpMem.lIndex == m_vectorTeacherInfoList[i].index)
			{
				break;
			}
		}
		if(i == m_vectorTeacherInfoList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sTeacherInfo &info = m_vectorTeacherInfoList[i];
		m_nCurInfoLines = 5;
		m_strInfo[0].PrintF(_S( 1662, "명성 수치 : %d" ), info.nFame);
		m_colInfo[0] = 0xE0FF40FF;
		m_strInfo[1].PrintF(_S( 1663, "양성중인 초보자 : %d" ), info.nCntTeachingStudent);
		m_colInfo[1] = 0xFFFFFFFF;
		m_strInfo[2].PrintF(_S( 1664, "양성 완료 인원 : %d" ), info.nCntCompleteStudent);
		m_colInfo[2] = 0xFFFFFFFF;
		m_strInfo[3].PrintF(_S( 1665, "양성 실패 인원 : %d" ), info.nCntFailStudent);
		m_colInfo[3] = 0xFFFFFFFF;
		int percent = int(info.nCntCompleteStudent / float(info.nCntCompleteStudent + info.nCntFailStudent) * 100 + 0.5f);//반올림
		m_strInfo[4].PrintF(_S( 1666, "양성 성공률 : %d %%" ), percent); 
		m_colInfo[4] = 0xFFFFFFFF;
	}
	else if(m_eHelperMenuState == HELPER_MENU_TEACHER)//선생이 창을 띄움.
	{
		absLine += m_lbTeacherList.GetScrollBarPos();
		if(absLine < m_lbTeacherList.GetScrollBarPos()
		|| absLine >= m_vectorHelperList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sHelperMember &helpMem = m_vectorHelperList[absLine];
		//TEMP : 초고급 제련석 관련
		LONG i;
		for(i = 0; i < m_vectorStudentInfoList.size(); ++i)
		{
			if(helpMem.lIndex == m_vectorStudentInfoList[i].index)
			{
				break;
			}
		}
		if(i == m_vectorStudentInfoList.size())
		{
			m_bShowInfo = FALSE;
			return;
		}
		sStudentInfo &info = m_vectorStudentInfoList[i];
		m_nCurInfoLines = 4;
		m_strInfo[0].PrintF(_S( 1667, "후견인 결성일" )); 
		m_colInfo[0] = 0xFFFFFFFF;
		m_strInfo[1].PrintF("%s", info.szFirstDate);
		m_colInfo[1] = 0xFFFFFFFF;

		m_strInfo[2].PrintF(_S( 1668, "종료 시간" )); 
		m_colInfo[2] = 0xFFFFFFFF;
		m_strInfo[3].PrintF("%s", info.szFinalDate);
		m_colInfo[3] = 0xFFFFFFFF;
	}

	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;

	GetAbsPos(nInfoPosX, nInfoPosY);
	nInfoPosX += m_rcInfoMark[line].Left;
	nInfoPosY += m_rcInfoMark[line].Top;
	
	nInfoWidth = 0;
	for(int i=0; i<m_nCurInfoLines; ++i)
	{
		int infoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strInfo[i].Length() *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		if(infoWidth > nInfoWidth) nInfoWidth = infoWidth;
	}
	nInfoHeight = 19 - _pUIFontTexMgr->GetLineSpacing() + m_nCurInfoLines * _pUIFontTexMgr->GetLineHeight();
	
	// Update item information box
	nInfoPosX += INFOMARK_SIZE / 2 - nInfoWidth / 2;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( nInfoPosX < pUIManager->GetMinI() )
		nInfoPosX = pUIManager->GetMinI();
	else if( nInfoPosX + nInfoWidth > pUIManager->GetMaxI() )
		nInfoPosX = pUIManager->GetMaxI() - nInfoWidth;

	if( nInfoPosY - nInfoHeight < pUIManager->GetMinJ() )
	{
		nInfoPosY += INFOMARK_SIZE;
		m_rcInfo.SetRect( nInfoPosX, nInfoPosY, nInfoPosX + nInfoWidth, nInfoPosY + nInfoHeight );
	}
	else
	{
		m_rcInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
	}
}

// ----------------------------------------------------------------------------
// Name : RenderNew()
// Desc : 후견인 시스템 확장 Render
// ----------------------------------------------------------------------------
void CUIHelper::RenderNew()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	//------------------------------------------------------------------
	// BackGround
	pDrawPort->InitTextureData( m_ptdSubTexture );

	for ( int i = 0; i < 9; ++i )
	{
		m_NewBack.m_RectSurfaceArray[i].m_RT = UIRect(m_rtNewBack[i].Left+m_nPosX,m_rtNewBack[i].Top+m_nPosY, m_rtNewBack[i].Right+m_nPosX, m_rtNewBack[i].Bottom+m_nPosY);
	}
	m_NewBack.RenderRectSurface(pDrawPort,0xFFFFFFFF);

	pDrawPort->FlushRenderingQueue();
	
	//------------------------------------------------------------------
	if ( m_eHelperMenuState == HELPER_MENU_STUDENT )
	{	// 견습생일때,
		RenderStudent();
	}
	else
	{	// 후견인일때,
		RenderTeacher();
	}
	//------------------------------------------------------------------	
}

// ----------------------------------------------------------------------------
// Name : RenderStudent()
// Desc : 견습생 UI
// ----------------------------------------------------------------------------
void CUIHelper::RenderStudent()
{
	int	nX, nY, i;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if ( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 )
	{	// 후견인이 없다면
		//------------------------------------------------------------------
		// New_Interface.tex : Sub BackGround(Line)
		pDrawPort->InitTextureData( m_ptdSubTexture );

		nY = 0;
		
		for( i = 0; i < 11; ++i)
		{
			pDrawPort->AddTexture(m_rtTabLine.Left + m_nPosX, m_rtTabLine.Top + m_nPosY + nY, m_rtTabLine.Right + m_nPosX, m_rtTabLine.Bottom + m_nPosY + nY, 
				m_rcTabLineUV.U0, m_rcTabLineUV.V0, m_rcTabLineUV.U1, m_rcTabLineUV.V1, 0xFFFFFFFF);
			nY += _pUIFontTexMgr->GetLineHeight()+5;
		}

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		//------------------------------------------------------------------
		// MessageBox.tex : List, Info Mark
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		m_lbTeacherList.Render();
		
		//Info Mark
		int listCount = 0;
		if(m_eHelperMenuState == HELPER_MENU_STUDENT)
			listCount = m_vectorHelperList.size() - m_lbStudentList.GetScrollBarPos();
		else if(m_eHelperMenuState == HELPER_MENU_TEACHER)
			listCount = m_vectorHelperList.size() - m_lbTeacherList.GetScrollBarPos();
		for( i = 0; i < listCount; ++i)
		{
			int nX0 = m_rcInfoMark[i].Left + m_nPosX;
			int nX1 = m_rcInfoMark[i].Right + m_nPosX;
			int nY0 = m_rcInfoMark[i].Top + m_nPosY;
			int nY1 = m_rcInfoMark[i].Bottom + m_nPosY;
			pDrawPort->AddTexture(
				nX0, nY0, nX1, nY1
				, m_rtInfoMarkUV.U0, m_rtInfoMarkUV.V0, m_rtInfoMarkUV.U1, m_rtInfoMarkUV.V1
				, 0xFFFFFFFF );
			
			nX0 = m_rcClass[i].Left + m_nPosX;
			nX1 = m_rcClass[i].Right + m_nPosX;
			nY0 = m_rcClass[i].Top + m_nPosY;
			nY1 = m_rcClass[i].Bottom + m_nPosY;
			pDrawPort->AddTexture(
				nX0, nY0, nX1, nY1
				, m_rtClass[m_vectorHelperList[i].iJob].U0, m_rtClass[m_vectorHelperList[i].iJob].V0, m_rtClass[m_vectorHelperList[i].iJob].U1, m_rtClass[m_vectorHelperList[i].iJob].V1
				, 0xFFFFFFFF );
		}

		//------------------------------------------------------------------
		// Text
		pDrawPort->PutTextExCX( _S(5071, "후견인 시스템"), GetWidth()/2+m_nPosX, 19+m_nPosY);

		nY = m_nPosY + 40;
		nX = m_nPosX + TAB_POS_LEFT + 14;
		pDrawPort->PutTextEx( _S( 1160, "클래스" ), nX, nY, 0x6B6B6BFF );		
		
		nX = m_nPosX + TAB_POS_LEFT + NEW_TAB_CLASS + 14;
		pDrawPort->PutTextEx( _S( 1159, "이 름" ), nX, nY, 0x6B6B6BFF );		
		
		nX = m_nPosX + TAB_POS_LEFT + NEW_TAB_CLASS + NEW_TAB_NAME + 14;
		pDrawPort->PutTextEx( _S( 5075, "주접속 시간" ), nX, nY, 0x6B6B6BFF );
		
		nX = m_nPosX + TAB_POS_LEFT + NEW_TAB_CLASS + NEW_TAB_NAME + NEW_TAB_TIME + 14;
		pDrawPort->PutTextEx( _S( 5076, "양성수" ), nX, nY, 0x6B6B6BFF );
		
		nX = m_nPosX + TAB_POS_LEFT + NEW_TAB_CLASS+NEW_TAB_NAME+NEW_TAB_TIME+NEW_TAB_COUNT + NEW_TAB_INFO / 2;
		pDrawPort->PutTextExCX( _S( 1661, "정보" ), nX, nY, 0x6B6B6BFF );
				
		// Render all elements
		pDrawPort->FlushRenderingQueue();

		//------------------------------------------------------------------
		// CommonBtn.tex : Button
		pDrawPort->InitTextureData( m_ptdBtnTexture );
		m_btnExit.Render();						// Button : Exit
		m_btnClose.Render();					// Button : Close
		m_btnRefreshTeacher.Render();			// Button : 목록 갱신
		m_btnBeMyTeacher.Render();				// Button : 견습생 신청
		m_btnFireMyTeacher.Render();			// Button : 견습생 포기
		//------------------------------------------------------------------
	}
	else
	{	// 후견인이 있다면
		//------------------------------------------------------------------
		// New_Interface.tex : Sub BackGround
		pDrawPort->InitTextureData( m_ptdSubTexture );
		
		for ( i = 0; i < 6; ++i )
		{
			m_NewSubBack.m_RectSurfaceArray[i].m_RT = UIRect(m_rtStudentBack[i].Left+m_nPosX,m_rtStudentBack[i].Top+m_nPosY, m_rtStudentBack[i].Right+m_nPosX, m_rtStudentBack[i].Bottom+m_nPosY);
			m_NewInfoBack.m_RectSurfaceArray[i].m_RT = UIRect(m_rtStudentInfo[i].Left+m_nPosX,m_rtStudentInfo[i].Top+m_nPosY, m_rtStudentInfo[i].Right+m_nPosX, m_rtStudentInfo[i].Bottom+m_nPosY);
		}
		m_NewSubBack.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
		m_NewInfoBack.RenderRectSurface(pDrawPort, 0xFFFFFFFF);

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		//------------------------------------------------------------------
		// Text : 후견인 정보
		pDrawPort->PutTextExCX( _S(5072, "후견인 상세 정보"), GetWidth()/2+m_nPosX, 19+m_nPosY);
		
		nY = m_nPosY + 55;
		nX = m_nPosX + 22;

		pDrawPort->PutTextEx( _S( 71, "이름" ), nX, nY, 0xffff99FF );
		nY += 33;
		pDrawPort->PutTextEx( _S( 1160, "클래스" ), nX, nY, 0xffff99FF );
		nY += 33;
		pDrawPort->PutTextEx( _S( 5077, "양성중인 인원" ), nX, nY, 0xffff99FF );
		nY += 33;
		pDrawPort->PutTextEx( _S( 5078, "양성 완료 인원" ), nX, nY, 0xffff99FF );
		nY += 33;
		pDrawPort->PutTextEx( _S( 5079, "양성 성공률" ), nX, nY, 0xffff99FF );

		nY = m_nPosY + 55;
		nX = m_nPosX + 278;

		CTString strText = _pNetwork->MyCharacterInfo.strTeacherName;
		pDrawPort->PutTextCharExRX( strText, strText.Length(), nX, nY );
		nY += 33;
		strText = CJobInfo::getSingleton()->GetName( m_vectorHelperList[0].iJob,  m_vectorHelperList[0].iJob2 );
		pDrawPort->PutTextCharExRX( strText, strText.Length(), nX, nY );
		pDrawPort->PutTextCharExRX( strText, strText.Length(), nX, nY );
		nY += 33;
		strText.PrintF( _S( 5070, "%d명"), m_vectorTeacherInfoList[0].nCntTeachingStudent);
		pDrawPort->PutTextCharExRX( strText, strText.Length(), nX, nY );
		nY += 33;
		strText.PrintF( _S( 5070, "%d명"), m_vectorTeacherInfoList[0].nCntCompleteStudent);
		pDrawPort->PutTextCharExRX( strText, strText.Length(), nX, nY );
		nY += 33;
		int percent = 0;
		int nTry = m_vectorTeacherInfoList[0].nCntCompleteStudent + m_vectorTeacherInfoList[0].nCntFailStudent;
		if (nTry > 0)
			percent = int(m_vectorTeacherInfoList[0].nCntCompleteStudent / float(nTry) * 100.f + 0.5f);//반올림
		strText.PrintF("%d%%", percent);
		pDrawPort->PutTextCharExRX( strText, strText.Length(), nX, nY );
		
		//------------------------------------------------------------------
		// CommonBtn.tex : Button
		pDrawPort->InitTextureData( m_ptdBtnTexture );
		m_btnExit.Render();							// Button : Exit
		m_btnClose.Render();						// Button : Close
		m_btnFireMyTeacher.Render();				// Button : 견습생 포기
		//------------------------------------------------------------------
	}
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	RenderInfo();
}

// ----------------------------------------------------------------------------
// Name : RenderTeacher()
// Desc : 후견인 UI
// ----------------------------------------------------------------------------
void CUIHelper::RenderTeacher()
{
	int		i;
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	//------------------------------------------------------------------
	// New_Interface.tex : Sub BackGround
	pDrawPort->InitTextureData( m_ptdSubTexture );
	
	for ( i = 0; i < 6; ++i )
	{
		m_NewSubBack.m_RectSurfaceArray[i].m_RT = UIRect(m_rtTeacherBack[i].Left+m_nPosX,m_rtTeacherBack[i].Top+m_nPosY, m_rtTeacherBack[i].Right+m_nPosX, m_rtTeacherBack[i].Bottom+m_nPosY);
		m_NewInfoBack.m_RectSurfaceArray[i].m_RT = UIRect(m_rtTeacherInfo[i].Left+m_nPosX,m_rtTeacherInfo[i].Top+m_nPosY, m_rtTeacherInfo[i].Right+m_nPosX, m_rtTeacherInfo[i].Bottom+m_nPosY);
	}
	m_NewSubBack.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
	m_NewInfoBack.RenderRectSurface(pDrawPort, 0xFFFFFFFF);
	
	int nY = 0;
	for ( i = 0; i < 11; ++i)
	{
		pDrawPort->AddTexture(m_rtTabLine.Left + m_nPosX, m_rtTabLine.Top + m_nPosY + nY, m_rtTabLine.Right + m_nPosX, m_rtTabLine.Bottom + m_nPosY + nY, 
			m_rcTabLineUV.U0, m_rcTabLineUV.V0, m_rcTabLineUV.U1, m_rcTabLineUV.V1, 0xFFFFFFFF);
		nY += _pUIFontTexMgr->GetLineHeight()+5;
	}

	pDrawPort->FlushRenderingQueue();

	//------------------------------------------------------------------
	// MessageBox.tex : List
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	m_lbStudentList.Render();

	//Info Mark
	int listCount = 0;
	if(m_eHelperMenuState == HELPER_MENU_STUDENT)
		listCount = m_vectorHelperList.size() - m_lbStudentList.GetScrollBarPos();
	else if(m_eHelperMenuState == HELPER_MENU_TEACHER)
		listCount = m_vectorHelperList.size() - m_lbTeacherList.GetScrollBarPos();
	for( i = 0; i < listCount; ++i )
	{
		int nX0 = m_rcClass[i].Left + m_nPosX;
		int nX1 = m_rcClass[i].Right + m_nPosX;
		int nY0 = m_rcClass[i].Top + m_nPosY;
		int nY1 = m_rcClass[i].Bottom + m_nPosY;
		pDrawPort->AddTexture(
			nX0, nY0, nX1, nY1
			, m_rtClass[m_vectorHelperList[i].iJob].U0, m_rtClass[m_vectorHelperList[i].iJob].V0, m_rtClass[m_vectorHelperList[i].iJob].U1, m_rtClass[m_vectorHelperList[i].iJob].V1
			, 0xFFFFFFFF );
	}
	pDrawPort->FlushRenderingQueue();

	//------------------------------------------------------------------
	// CommonBtn.tex : Button
	pDrawPort->InitTextureData( m_ptdBtnTexture );

	if( !m_bRegistredTeacher )
		m_btnRegisterTeacher.Render();			// Button : 후견인 등록
	else
		m_btnCancelRegister.Render();			// Button : 후견인 등록 취소

	m_btnGift.Render();							// Button : 보상 요청
	m_btnFireMyStudent.Render();				// Button : 후견인 포기
	m_btnClose.Render();						// Button : Close
	m_btnExit.Render();							// Button : Exit

	pDrawPort->FlushRenderingQueue();

	//------------------------------------------------------------------
	// Text
	pDrawPort->PutTextExCX( _S(5071, "후견인 시스템"), GetWidth()/2+m_nPosX, 19+m_nPosY);

	nY = m_nPosY + 40;
	int nX = m_nPosX + 21;

	pDrawPort->PutTextEx( _S( 1160, "클래스" ), nX, nY, 0x6B6B6BFF );		
	
	nX = m_nPosX + 71;
	pDrawPort->PutTextEx( _S( 1159, "이 름" ), nX, nY, 0x6B6B6BFF );		
	
	nX = m_nPosX + 216;
	pDrawPort->PutTextEx( _S( 73, "레벨" ), nX, nY, 0x6B6B6BFF );

	nX = m_nPosX + 262;
	pDrawPort->PutTextEx( _S( 5072, "후견인 상세 정보" ), nX, nY, 0x6B6B6BFF );

	nX = m_nPosX + 265;
	nY = m_nPosY + 63;

	pDrawPort->PutTextEx( _S( 5077, "양성중인 인원" ), nX, nY, 0xffff99FF );
	
	nY += 33;
	pDrawPort->PutTextEx( _S( 5078, "양성 완료 인원" ), nX, nY, 0xffff99FF );

	nY += 33;
	pDrawPort->PutTextEx( _S( 5082, "양성 실패 인원" ), nX, nY, 0xffff99FF );

	nY += 33;
	pDrawPort->PutTextEx( _S( 5079, "양성 성공률" ), nX, nY, 0xffff99FF );

	nX = m_nPosX + 415;
	nY = m_nPosY + 63;	

	CTString strMessage;
	strMessage.PrintF("%d", m_nMyCurTeachCnt);
	pDrawPort->PutTextExRX( strMessage, nX, nY );

	nY += 33;
	strMessage.PrintF("%d", m_nMyCompleteTeachCnt);
	pDrawPort->PutTextExRX( strMessage, nX, nY );

	nY += 33;
	strMessage.PrintF("%d", m_nMyFailTeachCnt);
	pDrawPort->PutTextExRX( strMessage, nX, nY );

	nY += 33;
	int nCnt = 0;
	if (m_nMyCompleteTeachCnt + m_nMyFailTeachCnt > 0)
		nCnt = int(m_nMyCompleteTeachCnt / float(m_nMyCompleteTeachCnt + m_nMyFailTeachCnt) * 100 + 0.5f);
	strMessage.PrintF("%d%%", nCnt);
	pDrawPort->PutTextExRX( strMessage, nX, nY );

	pDrawPort->EndTextEx();

	RenderInfo();
}

// ----------------------------------------------------------------------------
// Name : PrepareOpen()
// Desc : UI Setting
// ----------------------------------------------------------------------------
void CUIHelper::PrepareOpen()
{
	if ( m_eHelperMenuState == HELPER_MENU_STUDENT )
	{	// 견습생일때,
		if( _pNetwork->MyCharacterInfo.lTeacherIndex == -1 )
		{
			SetSize( NEW_HELPER_WIDTH, 335 );
			m_rtTabLine.SetRect(21, 54, 400, 55);
			m_btnRefreshTeacher.SetEnable(TRUE);			// 후견인 목록 갱신		
			m_btnBeMyTeacher.SetEnable(TRUE);				// 견습생
			m_btnFireMyTeacher.SetEnable(FALSE);
			m_btnFireMyTeacher.SetPos( 38, HELPER_BUTTON_START_Y + 40 );
			m_btnClose.SetPos( 410, 4 );
			m_btnExit.SetPos( 38 + NEW_BUTTON_SIZE + 38, HELPER_BUTTON_START_Y + 40 );
			m_rtNewBack[BG_UL].SetRect(0,0,8,39);
			m_rtNewBack[BG_UM].SetRect(8,0,NEW_HELPER_WIDTH-8,39);
			m_rtNewBack[BG_UR].SetRect(NEW_HELPER_WIDTH-8,0,NEW_HELPER_WIDTH,39);
			m_rtNewBack[BG_ML].SetRect(0,39,8,327);
			m_rtNewBack[BG_MM].SetRect(8,39,NEW_HELPER_WIDTH-8,327);
			m_rtNewBack[BG_MR].SetRect(NEW_HELPER_WIDTH-8,39,NEW_HELPER_WIDTH,327);
			m_rtNewBack[BG_BL].SetRect(0,327,8,335);
			m_rtNewBack[BG_BM].SetRect(8,327,NEW_HELPER_WIDTH-8,335);
			m_rtNewBack[BG_BR].SetRect(NEW_HELPER_WIDTH-8,327,NEW_HELPER_WIDTH,335);
		}
		else
		{
			SetSize( 300, 335 );
			m_btnFireMyTeacher.SetPos( 70, HELPER_BUTTON_START_Y+10 );
			m_btnExit.SetPos( 70, HELPER_BUTTON_START_Y+40 );
			m_btnClose.SetPos( 275, 4 );
			m_btnRefreshTeacher.SetEnable(FALSE);			// 후견인 목록 갱신		
			m_btnBeMyTeacher.SetEnable(FALSE);				// 견습생
			m_btnFireMyTeacher.SetEnable(TRUE);

			m_rtNewBack[BG_UL].SetRect(0,0,8,39);
			m_rtNewBack[BG_UM].SetRect(8,0,300-8,39);
			m_rtNewBack[BG_UR].SetRect(300-8,0,300,39);
			m_rtNewBack[BG_ML].SetRect(0,39,8,327);
			m_rtNewBack[BG_MM].SetRect(8,39,300-8,327);
			m_rtNewBack[BG_MR].SetRect(300-8,39,300,327);
			m_rtNewBack[BG_BL].SetRect(0,327,8,335);
			m_rtNewBack[BG_BM].SetRect(8,327,300-8,335);
			m_rtNewBack[BG_BR].SetRect(300-8,327,300,335);
		}
	}
	else
	{	// 후견인일때,
		SetSize( NEW_HELPER_WIDTH, 335 );
		m_rtTabLine.SetRect(22,53,243,54);
		m_btnClose.SetPos( 410, 4 );
		m_btnExit.SetPos( 38 + NEW_BUTTON_SIZE + 38, HELPER_BUTTON_START_Y + 40 );
		m_rtNewBack[BG_UL].SetRect(0,0,8,39);
		m_rtNewBack[BG_UM].SetRect(8,0,NEW_HELPER_WIDTH-8,39);
		m_rtNewBack[BG_UR].SetRect(NEW_HELPER_WIDTH-8,0,NEW_HELPER_WIDTH,39);
		m_rtNewBack[BG_ML].SetRect(0,39,8,327);
		m_rtNewBack[BG_MM].SetRect(8,39,NEW_HELPER_WIDTH-8,327);
		m_rtNewBack[BG_MR].SetRect(NEW_HELPER_WIDTH-8,39,NEW_HELPER_WIDTH,327);
		m_rtNewBack[BG_BL].SetRect(0,327,8,335);
		m_rtNewBack[BG_BM].SetRect(8,327,NEW_HELPER_WIDTH-8,335);
		m_rtNewBack[BG_BR].SetRect(NEW_HELPER_WIDTH-8,327,NEW_HELPER_WIDTH,335);
		if ( m_vectorHelperList.size() > 0 )
		{	// 견습생이 있을 때,
			m_btnFireMyStudent.SetEnable( TRUE );
		}
		else
		{	// 견습생이 없을 때,
			m_btnFireMyStudent.SetEnable( FALSE );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : Edit Box 2개일 때,
// ----------------------------------------------------------------------------
void CUIHelper::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput, CTString &strSEInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_PLAYTIME:
		{
			if ( bOK )
			{
				// 후견인 신청 메시지 보내기 : Client -> Server
				CTString strMessage = CTString("");
				CUIMsgBox_Info MsgBoxInfo;
				int sbStartPlayTime;
				int sbEndPlayTime;
				
				if ( strInput.IsInteger() && strSEInput.IsInteger() )
				{
					sbStartPlayTime = atoi(strInput);
					sbEndPlayTime = atoi(strSEInput);
					
					if ( sbStartPlayTime > 24 || sbEndPlayTime > 24 )
					{
						strMessage = _S( 5080, "24 이하의 숫자를 입력해주세요.");
					}
					else if ( sbStartPlayTime < 0 || sbEndPlayTime < 0 )
					{
						strMessage = _S( 4348, "0 이상의 숫자를 입력해 주시기 바랍니다.");
					}
				}
				else
				{
					strMessage = _S(4351, "숫자를 입력해 주시기 바랍니다.");
				}

				if (strMessage.Length() > 0)
				{
					MsgBoxInfo.SetMsgBoxInfo( _S( 5071, "후견인 시스템"), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage );
					CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
					return;
				}

				_pNetwork->TeachTeacherRegister( sbStartPlayTime, sbEndPlayTime );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ShowClass()
// Desc : 클래스 이름 팝업
// ----------------------------------------------------------------------------
void CUIHelper::ShowClass(int line)
{
	if (!m_bShowClass)	
	{	
		return;	
	}

	m_strClassName = CJobInfo::getSingleton()->GetName( m_vectorHelperList[line].iJob,  m_vectorHelperList[line].iJob2 );
	int		nInfoWidth, nInfoHeight;
	int		nInfoPosX, nInfoPosY;
	
	GetAbsPos(nInfoPosX, nInfoPosY);
	nInfoPosX += m_rcClass[line].Left;
	nInfoPosY += m_rcClass[line].Top;
	
	nInfoWidth = 30 + m_strClassName.Length() * _pUIFontTexMgr->GetFontWidth();
	nInfoHeight = 10 + _pUIFontTexMgr->GetFontHeight();
	
	if ( nInfoPosX - nInfoWidth > 0 )
	{
		nInfoPosX -= nInfoWidth;
		m_rcInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
	}
	else
	{
		nInfoPosX = CUIManager::getSingleton()->GetMinI();
		m_rcInfo.SetRect( nInfoPosX, nInfoPosY - nInfoHeight, nInfoPosX + nInfoWidth, nInfoPosY );
	}
	m_nCurInfoLines = 1;
}

// ----------------------------------------------------------------------------
// Name : FireMyStudent()
// Desc : 후견인이 견습생 포기
// ----------------------------------------------------------------------------
void CUIHelper::FireMyStudent()
{
	int nPos = m_lbStudentList.GetCurSel() + m_lbStudentList.GetScrollBarPos();
	if ( nPos >= 0 )
	{
		CTString	strStudentName = m_vectorHelperList[nPos].strName;
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 5068, "후견인 포기" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_FIRE_MYSTUDENT );		
		strMessage.PrintF( _S( 5081, "[%s]님의 후견인을 그만 두시겠습니까? 그만 두게되면 명성치 5를 잃게 됩니다." ), strStudentName );	
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );	
	}
}

// ----------------------------------------------------------------------------
// Name : SetMyTeachInfo()
// Desc : 내 정보 설정
// ----------------------------------------------------------------------------
void CUIHelper::SetMyTeachInfo( SLONG CurTeachCnt, SLONG CompleteTeachCnt, SLONG FailTeachCnt )
{
	m_nMyCurTeachCnt		= CurTeachCnt;
	m_nMyCompleteTeachCnt	= CompleteTeachCnt;
	m_nMyFailTeachCnt		= FailTeachCnt;
}

void CUIHelper::Clear()
{
	m_nMyCurTeachCnt		= 0;
	m_nMyCompleteTeachCnt	= 0;
	m_nMyFailTeachCnt		= 0;
	m_strClassName			= CTString("");
}

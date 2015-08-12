#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UILacarette.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>

#define MUSIC_LACABALL_DROPPED	CTFILENAME("Data\\Sounds\\Game\\System\\GS_caution.wav")
#define MUSIC_LACABALL_WIN		CTFILENAME("Data\\Sounds\\Game\\Equipment\\GE_moonstone_win.wav")

extern CDrawPort	*_pdpMain;

// ----------------------------------------------------------------------------
// Name : CUILacaball()
// Desc :
// ----------------------------------------------------------------------------
CUILacaball::CUILacaball() :
	m_bShowToolTip( FALSE ),
	m_bPickTitle( FALSE ),
	m_iCurrentTokenPos(0),
	m_iCurrentCoursePos(0),
	m_iDropPosGap( 0 ),
	m_bIsPlayAnim(FALSE)
{
	m_bStartAnim = false;
	m_iCurrentAnim = LCBALL_ANIM_BASE;
	m_bShowDropEndAnim	= false;
	m_bShowRewardEndAnim = false;
	m_sqShowingTime = 0;
}

CUILacaball::~CUILacaball()
{
	SAFE_DELETE(m_pIconRewardInBall);
}

void		CUILacaball::_TokenInfo::SettingTokenInfo(const INDEX _tokenIdx, const BYTE _tokenPos )
{
	this->tokenIndex = _tokenIdx;
	this->tokenPos = _tokenPos;
	CItemData* pID = _pNetwork->GetItemData( _tokenIdx );
	this->tokenName = pID->GetName();
}

void		CUILacaball::_SubCourseBox::Clear()
{
	this->pIconreward->clearIconData();
	this->rewardItemRemainCount = 0;
	this->rewardItemMaxCount = 0;
	this->Restore();
}
void		CUILacaball::_SubCourseBox::SettingItemInfo(const INDEX _rewardItemIdx )
{
	this->pIconreward->clearIconData();
	if( _rewardItemIdx < 0 )
		return;
	CItemData* pID = _pNetwork->GetItemData( _rewardItemIdx );
	this->pIconreward->setData(UBET_ITEM, pID->GetItemIndex());
	this->Used();

}

void		CUILacaball::_SubCourse_Reward::Clear()
{
	this->pIconreward->clearIconData();
	this->rewardItemRemainCount = 0;
	this->rewardItemMaxCount = 0;
	this->Restore();
}
void		CUILacaball::_SubCourse_Reward::SettingItemInfo(const INDEX _rewardItemIdx )
{
	this->pIconreward->clearIconData();
	if( _rewardItemIdx < 0 )
		return;
	CItemData* pID = _pNetwork->GetItemData( _rewardItemIdx );

	this->pIconreward->setData(UBET_ITEM, pID->GetItemIndex());
	this->Used();
}


// private & protected functions.
void		CUILacaball::_initBackground(const FLOAT fTexWidth, const FLOAT fTexHeight )
{
	m_rtsBase.AddRectSurface( UIRect(0, 0, 192, 42),		UIRectUV( 0, 0, 192, 42, fTexWidth, fTexHeight ) );		// LT
	m_rtsBase.AddRectSurface( UIRect(192, 0, 308, 42),		UIRectUV( 192, 0, 308, 42, fTexWidth, fTexHeight ) );	// CT
	m_rtsBase.AddRectSurface( UIRect(308, 0, 500, 42),		UIRectUV( 308, 0, 500, 42, fTexWidth, fTexHeight ) );	// RT
	m_rtsBase.AddRectSurface( UIRect(0, 42, 192, 353),		UIRectUV( 0, 42, 192, 78, fTexWidth, fTexHeight ) );	// LC
	m_rtsBase.AddRectSurface( UIRect(192, 42, 308, 353),	UIRectUV( 192, 42, 308, 78, fTexWidth, fTexHeight ) );	// CC
	m_rtsBase.AddRectSurface( UIRect(308, 42, 500, 353),	UIRectUV( 308, 42, 500, 78, fTexWidth, fTexHeight ) );	// RC
	m_rtsBase.AddRectSurface( UIRect(0, 353, 192, 375),		UIRectUV( 0, 78, 192, 100, fTexWidth, fTexHeight ) );	// LB
	m_rtsBase.AddRectSurface( UIRect(192, 353, 308, 375),	UIRectUV( 192, 78, 308, 100, fTexWidth, fTexHeight ) );	// CB
	m_rtsBase.AddRectSurface( UIRect(308, 353, 500, 375),	UIRectUV( 308, 78, 500, 100, fTexWidth, fTexHeight ) );	// RB
	// close button.
	m_btnClose.Create( this, CTString(""), 476, 4, 16, 16 );
	m_btnClose.SetUV(UBS_IDLE, 475, 177, 491, 193,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK, 492, 177, 508, 193,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);
	// token select background.
	m_rtsBase.AddRectSurface( UIRect( 224, 37, 234, 59 ),	UIRectUV( 80, 104, 90, 126, fTexWidth, fTexHeight ) );	// left bg.
	m_rtsBase.AddRectSurface( UIRect( 234, 37, 484, 59 ),	UIRectUV( 90, 104, 188, 126, fTexWidth, fTexHeight ) );	// center bg.
	m_rtsBase.AddRectSurface( UIRect( 484, 37, 494, 59 ),	UIRectUV( 188, 104, 198, 126, fTexWidth, fTexHeight ) );// right bg.
	// token select Prev & Next button.
	m_btnTokenPrev.Create( this, CTString(""), 226, 41, 14, 14 );
	m_btnTokenPrev.SetUV(UBS_IDLE, 155, 129, 169, 143,fTexWidth,fTexHeight);
	m_btnTokenPrev.SetUV(UBS_CLICK, 155, 145, 169, 159,fTexWidth,fTexHeight);
	m_btnTokenPrev.CopyUV(UBS_IDLE,UBS_ON);
	m_btnTokenPrev.CopyUV(UBS_IDLE,UBS_DISABLE);
	m_btnTokenNext.Create( this, CTString(""), 478, 41, 14, 14 );
	m_btnTokenNext.SetUV(UBS_IDLE, 171, 129, 185, 143,fTexWidth,fTexHeight);
	m_btnTokenNext.SetUV(UBS_CLICK, 171, 145, 185, 159,fTexWidth,fTexHeight);
	m_btnTokenNext.CopyUV(UBS_IDLE,UBS_ON);
	m_btnTokenNext.CopyUV(UBS_IDLE,UBS_DISABLE);
	// course select background.
	m_rcCoursRect.Left		= 224;
	m_rcCoursRect.Right		= 494;
	m_rcCoursRect.Top		= 60;
	m_rcCoursRect.Bottom	= 138; 
	m_rtsBase.AddRectSurface( UIRect(224, 60, 244, 70),		UIRectUV( 205, 105, 225, 115, fTexWidth, fTexHeight ) );// course LT bg.
	m_rtsBase.AddRectSurface( UIRect(244, 60, 474, 70),		UIRectUV( 225, 105, 276, 115, fTexWidth, fTexHeight ) );// course CT bg.
	m_rtsBase.AddRectSurface( UIRect(474, 60, 494, 70),		UIRectUV( 276, 105, 296, 115, fTexWidth, fTexHeight ) );// course RT bg.
	m_rtsBase.AddRectSurface( UIRect(224, 70, 244, 128),	UIRectUV( 205, 115, 225, 173, fTexWidth, fTexHeight ) );// course LC bg.
	m_rtsBase.AddRectSurface( UIRect(244, 70, 474, 128),	UIRectUV( 225, 115, 276, 173, fTexWidth, fTexHeight ) );// course CC bg.
	m_rtsBase.AddRectSurface( UIRect(474, 70, 494, 128),	UIRectUV( 276, 115, 296, 173, fTexWidth, fTexHeight ) );// course RC bg.
	m_rtsBase.AddRectSurface( UIRect(224, 128, 244, 138),	UIRectUV( 205, 166, 225, 176, fTexWidth, fTexHeight ) );// course LB bg.
	m_rtsBase.AddRectSurface( UIRect(244, 128, 474, 138),	UIRectUV( 225, 166, 276, 176, fTexWidth, fTexHeight ) );// course CB bg.
	m_rtsBase.AddRectSurface( UIRect(474, 128, 494, 138),	UIRectUV( 276, 166, 296, 176, fTexWidth, fTexHeight ) );// course RB bg.
	// remain / max count background.
	m_rtsBase.AddRectSurface( UIRect(224, 139, 244, 149),	UIRectUV( 205, 105, 225, 115, fTexWidth, fTexHeight ) );// remain LT bg.
	m_rtsBase.AddRectSurface( UIRect(244, 139, 474, 149),	UIRectUV( 225, 105, 276, 115, fTexWidth, fTexHeight ) );// remain CT bg.
	m_rtsBase.AddRectSurface( UIRect(474, 139, 494, 149),	UIRectUV( 276, 105, 296, 115, fTexWidth, fTexHeight ) );// remain RT bg.
	m_rtsBase.AddRectSurface( UIRect(224, 149, 244, 157),	UIRectUV( 205, 115, 225, 123, fTexWidth, fTexHeight ) );// remain LC bg.
	m_rtsBase.AddRectSurface( UIRect(244, 149, 474, 157),	UIRectUV( 225, 115, 276, 123, fTexWidth, fTexHeight ) );// remain CC bg.
	m_rtsBase.AddRectSurface( UIRect(474, 149, 494, 157),	UIRectUV( 276, 115, 296, 123, fTexWidth, fTexHeight ) );// remain RC bg.
	m_rtsBase.AddRectSurface( UIRect(224, 157, 244, 167),	UIRectUV( 205, 166, 225, 176, fTexWidth, fTexHeight ) );// remain LB bg.
	m_rtsBase.AddRectSurface( UIRect(244, 157, 474, 167),	UIRectUV( 225, 166, 276, 176, fTexWidth, fTexHeight ) );// remain CB bg.
	m_rtsBase.AddRectSurface( UIRect(474, 157, 494, 167),	UIRectUV( 276, 166, 296, 176, fTexWidth, fTexHeight ) );// remain RB bg.
	// remain / max count view.
	m_rtsBase.AddRectSurface( UIRect(372, 141, 490, 163),	UIRectUV( 80, 104, 198, 126, fTexWidth, fTexHeight ) );		// left remain count frame.
	// reward background.
	m_rcRewardRect.Left		= 224;
	m_rcRewardRect.Right	= 494;
	m_rcRewardRect.Top		= 168;
	m_rcRewardRect.Bottom	= 371;
	m_rtsBase.AddRectSurface( UIRect(224, 168, 244, 188),	UIRectUV( 205, 105, 225, 125, fTexWidth, fTexHeight ) );	// LT
	m_rtsBase.AddRectSurface( UIRect(244, 168, 474, 188),	UIRectUV( 225, 105, 276, 125, fTexWidth, fTexHeight ) );	// CT
	m_rtsBase.AddRectSurface( UIRect(474, 168, 494, 188),	UIRectUV( 276, 105, 296, 125, fTexWidth, fTexHeight ) );	// RT
	m_rtsBase.AddRectSurface( UIRect(224, 188, 244, 351),	UIRectUV( 205, 125, 225, 156, fTexWidth, fTexHeight ) );	// LC
	m_rtsBase.AddRectSurface( UIRect(244, 188, 474, 351),	UIRectUV( 225, 125, 276, 156, fTexWidth, fTexHeight ) );	// CC
	m_rtsBase.AddRectSurface( UIRect(474, 188, 494, 351),	UIRectUV( 276, 125, 296, 156, fTexWidth, fTexHeight ) );	// RC
	m_rtsBase.AddRectSurface( UIRect(224, 351, 244, 371),	UIRectUV( 205, 156, 225, 176, fTexWidth, fTexHeight ) );	// LB
	m_rtsBase.AddRectSurface( UIRect(244, 351, 474, 371),	UIRectUV( 225, 156, 276, 176, fTexWidth, fTexHeight ) );	// CB
	m_rtsBase.AddRectSurface( UIRect(474, 351, 494, 371),	UIRectUV( 276, 156, 296, 176, fTexWidth, fTexHeight ) );	// RB
	// reward info background.
	m_rtsBase.AddRectSurface( UIRect(226, 170, 246, 189), UIRectUV( 346, 141, 366, 160, fTexWidth, fTexHeight ) );	// reward info name left frame.
	m_rtsBase.AddRectSurface( UIRect(246, 170, 400, 189), UIRectUV( 356, 141, 407, 160, fTexWidth, fTexHeight ) );	// reward info name center frame.
	m_rtsBase.AddRectSurface( UIRect(400, 170, 420, 189), UIRectUV( 397, 141, 417, 160, fTexWidth, fTexHeight ) );	// reward info name right frame.
	m_rtsBase.AddRectSurface( UIRect(421, 170, 461, 189), UIRectUV( 346, 141, 386, 160, fTexWidth, fTexHeight ) );	// reward count left frame.
	m_rtsBase.AddRectSurface( UIRect(461, 170, 490, 189), UIRectUV( 386, 141, 417, 160, fTexWidth, fTexHeight ) );	// reward count right frame.
}

void		CUILacaball::_initCourseInfo(const FLOAT fTexWidth, const FLOAT fTexHeight )
{
	INDEX i = 0;
	// create course info.
	m_vecTokenInfo.clear();
	m_vecCourseInfo.clear();
	m_iCurrentTokenPos = 0;		// empty.

	// create course box.	fixed position.
	// default position. it is not move position.
	m_btnCoursePrev.Create( this, CTString(""), 226, 63, 22, 69 );
	m_btnCoursePrev.SetUV(UBS_IDLE, 297, 104, 319, 174,fTexWidth,fTexHeight);
	m_btnCoursePrev.SetUV(UBS_CLICK, 463, 103, 485, 173,fTexWidth,fTexHeight);
	m_btnCoursePrev.CopyUV(UBS_IDLE,UBS_ON);
	m_btnCoursePrev.CopyUV(UBS_IDLE,UBS_DISABLE);
	m_btnCourseNext.Create( this, CTString(""), 468, 63, 22, 69 );
	m_btnCourseNext.SetUV(UBS_IDLE, 321, 104, 343, 174,fTexWidth,fTexHeight);
	m_btnCourseNext.SetUV(UBS_CLICK, 487, 103, 509, 173,fTexWidth,fTexHeight);
	m_btnCourseNext.CopyUV(UBS_IDLE,UBS_ON);
	m_btnCourseNext.CopyUV(UBS_IDLE,UBS_DISABLE);

	UIRect	rtCourse(250, 65, 322, 134);
	for( i = 0; i < LACABALL_COURSE_MAX; ++i )
	{
		m_Course[i].boxRect = rtCourse;
		m_Course[i].boxSurface.AddRectSurface( rtCourse, UIRectUV( 0, 105, 71, 176, fTexWidth, fTexHeight ) );		// outer frame.
		m_Course[i].boxSurface.AddRectSurface( UIRect(rtCourse.Left+31, rtCourse.Top+6, rtCourse.Left+31+34, rtCourse.Top+6+34), UIRectUV( 80, 128, 114, 162, fTexWidth, fTexHeight ) );	// inner item frame.
		m_Course[i].boxSurface.AddRectSurface( UIRect(rtCourse.Left+6, rtCourse.Top+43, rtCourse.Left+6+30, rtCourse.Bottom-5), UIRectUV( 80, 104, 110, 126, fTexWidth, fTexHeight ) );		// left remain count frame.
		m_Course[i].boxSurface.AddRectSurface( UIRect(rtCourse.Left+6+30, rtCourse.Top+43, rtCourse.Right-6, rtCourse.Bottom-5), UIRectUV( 168, 104, 198, 126, fTexWidth, fTexHeight ) );	// right remain count frame.
		m_Course[i].pIconreward->Create(this, rtCourse.Left + 32, rtCourse.Top + 7, 32, 32, UI_LACABALL, UBET_ITEM);
		
		int nWidth = rtCourse.GetWidth() + 1;
		rtCourse.Left += nWidth;
		rtCourse.Right += nWidth;
	}
	
	// create course list.
	UIRect	rtList(227, 192, 262, 226);
	for( i = 0; i < LACABALL_REWARD_MAX; ++i )
	{
		m_CourseRewardList[i].rewardRect = UIRect( rtList.Left, rtList.Top, rtList.Right+229, rtList.Bottom );

		m_CourseRewardList[i].rewardSurface.AddRectSurface( rtList, UIRectUV( 80, 128, 114, 162, fTexWidth, fTexHeight ) );	// item box.
		
		m_CourseRewardList[i].rewardSurface.AddRectSurface( UIRect(rtList.Right+1, rtList.Top-1, rtList.Right+41, rtList.Bottom-1), UIRectUV( 346, 104, 386, 139, fTexWidth, fTexHeight ) );	// reward name left frame.
		m_CourseRewardList[i].rewardSurface.AddRectSurface( UIRect(rtList.Right+41, rtList.Top-1, rtList.Right+118, rtList.Bottom-1), UIRectUV( 356, 104, 433, 139, fTexWidth, fTexHeight ) );	// reward name center frame.
		m_CourseRewardList[i].rewardSurface.AddRectSurface( UIRect(rtList.Right+118, rtList.Top-1, rtList.Right+158, rtList.Bottom-1), UIRectUV( 402, 104, 442, 139, fTexWidth, fTexHeight ) );	// reward name right frame.

		m_CourseRewardList[i].rewardSurface.AddRectSurface( UIRect(rtList.Right+159, rtList.Top-1, rtList.Right+194, rtList.Bottom-1), UIRectUV( 346, 104, 381, 139, fTexWidth, fTexHeight ) );	// reward count left frame.
		m_CourseRewardList[i].rewardSurface.AddRectSurface( UIRect(rtList.Right+194, rtList.Top-1, rtList.Right+229, rtList.Bottom-1), UIRectUV( 407, 104, 442, 139, fTexWidth, fTexHeight ) );	// reward count right frame.
		m_CourseRewardList[i].pIconreward->Create(this, rtList.Left + 1, rtList.Top + 1, 32, 32, UI_LACABALL, UBET_ITEM);

		int nHeight = rtList.GetHeight() + 1;
		rtList.Top += nHeight;
		rtList.Bottom += nHeight;
	}

	m_rtItemBlankCap.SetUV( 118, 128, 150, 160, fTexWidth, fTexHeight );
	m_rtSelCourse.SetUV( 442, 198, 509, 265, fTexWidth, fTexHeight );
	m_rtDisableL.SetUV( 441, 267, 451, 301, fTexWidth, fTexHeight );
	m_rtDisableC.SetUV( 445, 267, 505, 301, fTexWidth, fTexHeight );
	m_rtDisableR.SetUV( 499, 267, 509, 301, fTexWidth, fTexHeight );
}

void		CUILacaball::_initBallInfo(const FLOAT fTexWidth, const FLOAT fTexHeight )
{
	// left background.
	m_rtsLeftBase.AddRectSurface( UIRect(6, 37, 222, 370),		UIRectUV( 0, 180, 216, 512, fTexWidth, fTexHeight ) );

	m_animBall.NumberOfFrames = 13;
	m_animBall.speed = 0.1f;
	//m_rtsBase.AddRectSurface( UIRect(6, 37, 222, 370),		UIRectUV( 0, 180, 216, 512, fTexWidth, fTexHeight ) );
	m_animBall.Base.SetRectEx( UIRect( 10,67, 218, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );
	// W : 216  // H : 178
	m_animBall.AddRectSurface( UIRect( 11,67, 219, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 1
	m_animBall.AddRectSurface( UIRect( 10,67, 218, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 2
	m_animBall.AddRectSurface( UIRect( 9,67, 217, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 3
	m_animBall.AddRectSurface( UIRect( 10,67, 218, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 4
	m_animBall.AddRectSurface( UIRect( 11,67, 219, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 5
	m_animBall.AddRectSurface( UIRect( 10,67, 218, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 6
	m_animBall.AddRectSurface( UIRect( 9,67, 217, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 7
	m_animBall.AddRectSurface( UIRect( 10,67, 218, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 8
	m_animBall.AddRectSurface( UIRect( 11,67, 219, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 9
	m_animBall.AddRectSurface( UIRect( 10,67, 218, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 10
	m_animBall.AddRectSurface( UIRect( 9,67, 217, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 11
	m_animBall.AddRectSurface( UIRect( 10,67, 218, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 12
	m_animBall.AddRectSurface( UIRect( 11,67, 219, 225 ),		UIRectUV( 218, 334, 426, 492, fTexWidth, fTexHeight ) );	// 13
	// ball UV
	m_rcBall.SetUV( 450, 384, 498, 432, fTexWidth, fTexHeight );
	// basket UV
	m_rcBasket.SetUV( 70, 483, 146, 500, fTexWidth, fTexHeight );

	// start button.
	m_btnStart.Create( this, CTString(""), 160, 315, 38, 38 );
	m_btnStart.SetUV(UBS_IDLE, 434, 436, 472, 474,fTexWidth,fTexHeight);
	m_btnStart.SetUV(UBS_CLICK, 474, 436, 512, 474,fTexWidth,fTexHeight);
	m_btnStart.CopyUV(UBS_IDLE,UBS_ON);
	m_btnStart.CopyUV(UBS_IDLE,UBS_DISABLE);
}

void		CUILacaball::_initRewardBallInfo(const FLOAT fTexWidth, const FLOAT fTexHeight )
{
	m_rtLeftHalfBall.SetUV( 450, 330, 474, 378, fTexWidth, fTexHeight );
	m_rtRightHalfBall.SetUV( 480, 330, 504, 378, fTexWidth, fTexHeight );
	m_rtCCHalfBall.SetUV( 450, 111, 458, 151, fTexWidth, fTexHeight );
	
	m_rtsBall.AddRectSurface( UIRect( (m_nWidth/2)-113,(m_nHeight/2)-24, (m_nWidth/2)-89, (m_nHeight/2)+24 ),	UIRectUV( 450, 330, 474, 378, fTexWidth, fTexHeight ) );	// left
	m_rtsBall.AddRectSurface( UIRect( (m_nWidth/2)-89,(m_nHeight/2)-20, (m_nWidth/2)+89, (m_nHeight/2)+20 ),	UIRectUV( 450, 111, 458, 151, fTexWidth, fTexHeight ) );	// center
	m_rtsBall.AddRectSurface( UIRect( (m_nWidth/2)+89,(m_nHeight/2)-24, (m_nWidth/2)+113, (m_nHeight/2)+24 ),	UIRectUV( 480, 330, 504, 378, fTexWidth, fTexHeight ) );	// right
	m_rtsBall.AddRectSurface( UIRect( (m_nWidth/2)-87,(m_nHeight/2)-16, (m_nWidth/2)-53, (m_nHeight/2)+18 ),	UIRectUV( 80, 128, 114, 162, fTexWidth, fTexHeight ) );		// 34
	m_rtsBall.AddRectSurface( UIRect( (m_nWidth/2)-52,(m_nHeight/2)-16, (m_nWidth/2)+87, (m_nHeight/2)+18 ),	UIRectUV( 436, 478, 498, 512, fTexWidth, fTexHeight ) );	// desc
	m_pIconRewardInBall = new CUIIcon;
	m_pIconRewardInBall->Create(this, (m_nWidth / 2) - 86, (m_nHeight / 2) - 15, 32, 32, UI_LACABALL, UBET_ITEM);
}

void		CUILacaball::_initData()
{
	_clearData();
}

void		CUILacaball::_clearData()
{
	m_iCurrentTokenPos = 0;
	m_sqStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_vecTokenInfo.clear();
	m_iCurrentAnim = LCBALL_ANIM_BASE;
	
	_clearCourseData();
	_clearRewardData();
}

void		CUILacaball::_clearNetData()
{
	m_iCurrentTokenPos = 0;
	m_sqStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_vecTokenInfo.clear();
	_clearNetCourseData();
	_clearNetRewardData();
}

void		CUILacaball::_clearNetCourseData()
{
	m_vecCourseInfo.clear();
}

void		CUILacaball::_clearNetRewardData()
{
	m_vecRewardInfo.clear();
}

void		CUILacaball::_clearCourseData()
{
	m_iCurrentCoursePos = 0;
	m_iShowCoursePos = 0;
	for( int i = 0; i < LACABALL_COURSE_MAX; ++i )
	{
		m_Course[i].Clear();
	}
}

void		CUILacaball::_clearRewardData()
{
	for( int i = 0; i < LACABALL_REWARD_MAX; ++i )
	{
		m_CourseRewardList[i].Clear();
	}
}

// render functions.
void		CUILacaball::_renderBackground()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsBase.SetPos( m_nPosX, m_nPosY );
	m_rtsBase.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	m_btnTokenPrev.Render();
	m_btnTokenNext.Render();
	m_btnClose.Render();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->PutTextExCX( _S( 5350, "라카볼"),		m_nPosX + m_nWidth/2,
															m_nPosY + 15, 0xDED9A0FF );
	pDrawPort->EndTextEx();
}

void		CUILacaball::_renderCourseInfo()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int i = 0;
	INDEX	remainCnt = 0;
	INDEX	maxCnt = 0;
	// draw course button. 
	// but not used this button event.
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_btnCoursePrev.Render();
	m_btnCourseNext.Render();
	pDrawPort->FlushRenderingQueue();
	CTString szTokenName;
	// draw token info.
	if( m_vecTokenInfo.size() != 0 )
	{
		szTokenName.PrintF( "%s", m_vecTokenInfo[m_iCurrentTokenPos].tokenName );
	}
	else
	{
		szTokenName = "None";
	}
	pDrawPort->PutTextExCX( szTokenName,		m_nPosX + 224 + 135,
															m_nPosY + 37 + 4, 0xDED9A0FF );
	// draw course remain / max count.
	szTokenName.PrintF( "%s/%s", _S( 5351, "남은개수" ), _S( 5352, "총개수") );
	pDrawPort->PutTextExRX( szTokenName,		m_nPosX + 368,
															m_nPosY + 139 + 6, 0xDED9A0FF );
	remainCnt = m_Course[m_iCurrentCoursePos - m_iShowCoursePos].rewardItemRemainCount;
	maxCnt = m_Course[m_iCurrentCoursePos - m_iShowCoursePos].rewardItemMaxCount;
	szTokenName.PrintF( "%d / %d", remainCnt, maxCnt );
	pDrawPort->PutTextExCX( szTokenName,		m_nPosX + 372+59,
															m_nPosY + 141 + 4, 
															_queryColor( remainCnt, maxCnt ) );
	pDrawPort->PutTextExCX( _S( 1866, "보상품" ),		m_nPosX + 226+97,
																	m_nPosY + 170 + 4, 
																	0xDED9A0FF );
	pDrawPort->PutTextExCX( _S( 5351, "남은개수" ),			m_nPosX + 421+35,
																	m_nPosY + 170 + 4, 
																	0xDED9A0FF );
	pDrawPort->EndTextEx();

	// draw course info.
	for( i = 0; i < LACABALL_COURSE_MAX; ++i )
	{
		
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		m_Course[i].boxSurface.SetPos( m_nPosX, m_nPosY );
		m_Course[i].boxSurface.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
		pDrawPort->FlushRenderingQueue();
		
		if (m_Course[i].IsUsed())
		{
			m_Course[i].pIconreward->Render(pDrawPort);
			CTString szRemainMax;
			INDEX iCourseRemainCount = m_Course[i].rewardItemRemainCount;
			INDEX iCourseMaxCount = m_Course[i].rewardItemMaxCount;
			szRemainMax.PrintF( "%d/%d", iCourseRemainCount, iCourseMaxCount );
			pDrawPort->PutTextExCX( szRemainMax,	m_nPosX + m_Course[i].boxRect.Left+36, 
																m_nPosY + m_Course[i].boxRect.Top+47, 
																_queryColor( iCourseRemainCount, iCourseMaxCount ) );
			pDrawPort->EndTextEx();
		}
	}
	
	// selected highlight.
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	UIRect	rtCourse(250, 64, 322, 133);
	int nWidth = rtCourse.GetWidth() + 1;
	rtCourse.Left = rtCourse.Left + (nWidth * (m_iCurrentCoursePos - m_iShowCoursePos) );
	rtCourse.Right = rtCourse.Right + (nWidth * (m_iCurrentCoursePos - m_iShowCoursePos) );
	pDrawPort->AddTexture( m_nPosX + rtCourse.Left, m_nPosY + rtCourse.Top, 
										m_nPosX + rtCourse.Right, m_nPosY + rtCourse.Bottom,
										m_rtSelCourse.U0, m_rtSelCourse.V0, m_rtSelCourse.U1, m_rtSelCourse.V1, 0xFFFFFF55 );
	pDrawPort->FlushRenderingQueue();

	// draw selected reward info.
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	for( i = 0; i < LACABALL_REWARD_MAX; ++i )
	{
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		m_CourseRewardList[i].rewardSurface.SetPos( m_nPosX, m_nPosY );
		m_CourseRewardList[i].rewardSurface.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
		pDrawPort->FlushRenderingQueue();
		// if has data, draw item box & text.
		if( m_CourseRewardList[i].IsUsed() )
		{
			m_CourseRewardList[i].pIconreward->Render(pDrawPort);
			
			CItemData* pID = _pNetwork->GetItemData( m_CourseRewardList[i].pIconreward->getIndex() );
			CTString szRemainMax;
			INDEX iRewardRemainCount = m_CourseRewardList[i].rewardItemRemainCount;
			INDEX iRewardMaxCount = m_CourseRewardList[i].rewardItemMaxCount;
			COLOR colRewardCol = _queryColor( iRewardRemainCount, iRewardMaxCount );
			pDrawPort->PutTextExCX( pID->GetName(),	m_nPosX + m_CourseRewardList[i].rewardRect.Left+35+78, 
																m_nPosY + m_CourseRewardList[i].rewardRect.Top+11, 0xDED9A0FF );
			szRemainMax.PrintF( "%d/%d", iRewardRemainCount, iRewardMaxCount );
			pDrawPort->PutTextExCX( szRemainMax,	m_nPosX + m_CourseRewardList[i].rewardRect.Left+196+35, 
																m_nPosY + m_CourseRewardList[i].rewardRect.Top+11, colRewardCol );
			pDrawPort->EndTextEx();
		}
		else
		{
			CTString szRemainMax;
			pDrawPort->AddTexture( m_nPosX+m_CourseRewardList[i].rewardRect.Left+1,m_nPosY+m_CourseRewardList[i].rewardRect.Top+1,
												m_nPosX+m_CourseRewardList[i].rewardRect.Left+1+32,m_nPosY+m_CourseRewardList[i].rewardRect.Top+1+32,
										m_rtItemBlankCap.U0, m_rtItemBlankCap.V0, m_rtItemBlankCap.U1, m_rtItemBlankCap.V1,
										0xFFFFFFFF );
			pDrawPort->FlushRenderingQueue();
			szRemainMax.PrintF( "-" );
			pDrawPort->PutTextExCX( szRemainMax,	m_nPosX + m_CourseRewardList[i].rewardRect.Left+35+78, 
																m_nPosY + m_CourseRewardList[i].rewardRect.Top+11, 0xADA97EFF );
 			szRemainMax.PrintF( "-/-\n" );
 			pDrawPort->PutTextExCX( szRemainMax,	m_nPosX + m_CourseRewardList[i].rewardRect.Left+196+35, 
 																m_nPosY + m_CourseRewardList[i].rewardRect.Top+11, 0xADA97EFF );
			pDrawPort->EndTextEx();
		}
		// disable reward item list.
		if( m_CourseRewardList[i].rewardItemRemainCount == 0 )
		{
			pDrawPort->InitTextureData( m_ptdBaseTexture );
			pDrawPort->AddTexture( m_nPosX+m_CourseRewardList[i].rewardRect.Left-2, m_nPosY+m_CourseRewardList[i].rewardRect.Top-2,
												m_nPosX+m_CourseRewardList[i].rewardRect.Left+9, m_nPosY+m_CourseRewardList[i].rewardRect.Bottom,
												m_rtDisableL.U0, m_rtDisableL.V0, m_rtDisableL.U1, m_rtDisableL.V1, 0xFFFFFFA0 );
			pDrawPort->AddTexture( m_nPosX+m_CourseRewardList[i].rewardRect.Left+9, m_nPosY+m_CourseRewardList[i].rewardRect.Top-2,
												m_nPosX+m_CourseRewardList[i].rewardRect.Right-11, m_nPosY+m_CourseRewardList[i].rewardRect.Bottom,
												m_rtDisableC.U0, m_rtDisableC.V0, m_rtDisableC.U1, m_rtDisableC.V1, 0xFFFFFFA0 );
			pDrawPort->AddTexture( m_nPosX+m_CourseRewardList[i].rewardRect.Right-11, m_nPosY+m_CourseRewardList[i].rewardRect.Top-2,
												m_nPosX+m_CourseRewardList[i].rewardRect.Right, m_nPosY+m_CourseRewardList[i].rewardRect.Bottom,
												m_rtDisableR.U0, m_rtDisableR.V0, m_rtDisableR.U1, m_rtDisableR.V1, 0xFFFFFFA0 );
			pDrawPort->FlushRenderingQueue();
		}
	}
}

void		CUILacaball::_renderBallInfo()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	_updateDropPos();
	if( m_animBall.IsAnim() && m_animBall.GetFrame()+1 >= m_animBall.NumberOfFrames ) // base 제외.
	{
		m_animBall.EndAnim();
		m_iCurrentAnim = LCBALL_ANIM_DROP;
		m_iDropAnimGap = 0;
		m_bBoundBall = FALSE;
		m_sqShowingTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	}
	// percent 75, 50, 25, 0
	m_animBall.SetPos( m_nPosX, m_nPosY + m_iDropPosGap*30 );
	m_animBall.Render( pDrawPort, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();
	m_rtsLeftBase.SetPos( m_nPosX, m_nPosY );
	m_rtsLeftBase.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	m_btnStart.Render();
	pDrawPort->FlushRenderingQueue();
}

// new render. only animations. [1/24/2011 rumist]
void		CUILacaball::_renderAnim()
{
	if( !m_bStartAnim )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if( m_iCurrentAnim == LCBALL_ANIM_DROP )
	{
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		UIRect rt( 93, 224, 141, 272);
		rt.Top += m_iDropAnimGap;
		rt.Bottom += m_iDropAnimGap;
		pDrawPort->AddTexture( m_nPosX + rt.Left, m_nPosY + rt.Top, m_nPosX + rt.Right, m_nPosY + rt.Bottom, 
											m_rcBall.U0, m_rcBall.V0, m_rcBall.U1, m_rcBall.V1, 0xFFFFFFFF );
		pDrawPort->FlushRenderingQueue();
		SQUAD currentTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		//if( currentTime - m_sqShowingTime > 500 )		// 0.5seconds.
		if( _queryTimer( 1 ) )						// 0.5 seconds.
		{
			if( m_bBoundBall == 0 )
			{
				if( rt.Bottom < (m_nHeight - 18) )
				{
					m_iDropAnimGap += 5;
				}
				else
				{
					++m_bBoundBall;
					m_LacaballSound.Play_t( MUSIC_LACABALL_DROPPED, 0 );
				}
			}
			else if( m_bBoundBall == 1 )
			{
				if( rt.Top > ( m_nHeight - 78) )
				{
					m_iDropAnimGap -= 3;
				}
				else
				{
					++m_bBoundBall;
				}
			}
			else if( m_bBoundBall == 2 )
			{
				if( rt.Bottom < ( m_nHeight - 18 ) )
				{
					m_iDropAnimGap  += 2;
				}
				else
				{
					++m_bBoundBall;
				}
			}
			else if( m_bBoundBall == 3 )
			{
				m_bShowDropEndAnim = true;
				//m_iCurrentAnim = LCBALL_ANIM_REWARD;
				m_byAlpha = 0x00;
				_sendUseToken( m_vecTokenInfo[m_iCurrentTokenPos].tokenIndex, m_iCurrentCoursePos );
				++m_bBoundBall;
			}
		}
	}

	if( m_bShowDropEndAnim )
	{
		COLOR colBall = 0xFFFFFF00;
		colBall |= (0xFF - m_byAlpha);
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		UIRect rt( 93, 223, 141, 271);
		pDrawPort->AddTexture( m_nPosX + rt.Left, m_nPosY + rt.Top+m_iDropAnimGap, 
											m_nPosX + rt.Right, m_nPosY + rt.Bottom+m_iDropAnimGap, 
											m_rcBall.U0, m_rcBall.V0, m_rcBall.U1, m_rcBall.V1, colBall );
		pDrawPort->FlushRenderingQueue();
	}

	// basket texture.
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	pDrawPort->AddTexture( m_nPosX + 76, m_nPosY + (m_nHeight - 34),
										m_nPosX + 152, m_nPosY + (m_nHeight - 17),
										m_rcBasket.U0, m_rcBasket.V0, m_rcBasket.U1, m_rcBasket.V1, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();


	if( m_iCurrentAnim == LCBALL_ANIM_REWARD )
	{
		COLOR colReward = 0xFFFFFF00;
		colReward |= m_byAlpha;
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		m_rtsBall.SetPos( m_nPosX, m_nPosY );
		m_rtsBall.RenderRectSurface( pDrawPort, colReward );
		pDrawPort->FlushRenderingQueue();
		if( _queryTimer( 1 ) )
			m_byAlpha += 15;

		if( m_byAlpha >= 0xFF )
		{
			m_bShowRewardEndAnim = true;
			m_sqShowingTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			m_iCurrentAnim = LCBALL_ANIM_BASE;
			m_LacaballSound.Play_t( MUSIC_LACABALL_WIN, 0 );
		}
	}

	if( m_bShowRewardEndAnim )
	{
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		m_rtsBall.SetPos( m_nPosX, m_nPosY );
		m_rtsBall.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
		pDrawPort->FlushRenderingQueue();
		m_pIconRewardInBall->Render(pDrawPort);
		CTString strName = _pNetwork->GetItemName( m_pIconRewardInBall->getIndex() );
		pDrawPort->PutTextExCX( strName,	m_nPosX+(m_nWidth/2)+20 , m_nPosY+(m_nHeight/2)-8 , 0xDED9A0FF );
		pDrawPort->EndTextEx();
		pDrawPort->FlushRenderingQueue();

		SQUAD currentTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		if( currentTime - m_sqShowingTime > 3000 )		// 3seconds.
		{
			_initAnimProperties();
			_updateCourseInfo();
			_updateRewardInfo();
			m_bIsPlayAnim = FALSE;
		}
	}
}

// network functions
void		CUILacaball::_receiveInfo(CNetworkMessage* istr )
{
	_clearNetData();
	INDEX		i						= 0;
	INDEX		iCurrentTokenIndex		= 0;
	BYTE		byCurrentCourseIndex	= 0;
	BYTE		byTokenLoopCount		= 0;
	INDEX		iTokenIndex				= 0;
	BYTE		bySubCourseLoopCount	= 0;
	BYTE		byRewardItemLoopCount	= 0;
	INDEX		iRewardItemIndex		= 0;
	ULONG		iRewardItemCount		= 0;
	INDEX		iRewardItemRemainCount	= 0;
	INDEX		irewardItemMaxCount		= 0;

	
	(*istr) >> byTokenLoopCount;
	for( i = 0; i < byTokenLoopCount; ++i )
	{
		(*istr) >> iTokenIndex;
		TOKENINFO	tmpToken;
		tmpToken.SettingTokenInfo( iTokenIndex, i );
		m_vecTokenInfo.push_back( tmpToken );
	}
		
 	(*istr) >> bySubCourseLoopCount;
	for( i = 0; i < bySubCourseLoopCount; ++i )
	{
		COURSEINFO	tmpCourse;
		(*istr) >> tmpCourse.rewardItem;
		(*istr) >> tmpCourse.rewardItemCount;
 		(*istr) >> tmpCourse.rewardItemRemainCount;
 		(*istr) >> tmpCourse.rewardItemMaxCount;
		tmpCourse.courseIdx = i;		// course position.
		m_vecCourseInfo.push_back( tmpCourse );
	}

	(*istr) >> iCurrentTokenIndex;
	(*istr) >> byCurrentCourseIndex;
	
	m_iCurrentTokenPos = _queryToken( iCurrentTokenIndex );
	m_iCurrentCoursePos = byCurrentCourseIndex;
	if( m_iCurrentCoursePos < 1 )
		m_iShowCoursePos = m_iCurrentCoursePos;		// 제일 앞에 선택시.
	else if( m_iCurrentCoursePos >= bySubCourseLoopCount-1 )
		m_iShowCoursePos = m_iCurrentCoursePos -2;	// 제일 뒤일경우.
	else
	{
		m_iShowCoursePos = m_iCurrentCoursePos -1;
	}

	(*istr) >> byRewardItemLoopCount;
	for( i = 0; i < byRewardItemLoopCount; ++i )
	{
		REWARDINFO	tmpReward;
		(*istr) >> tmpReward.rewardItem;
		(*istr) >> tmpReward.rewardItemCount;
		(*istr) >> tmpReward.rewardItemRemainCount;
		(*istr) >> tmpReward.rewardItemMaxCount;	

		m_vecRewardInfo.push_back( tmpReward );
	}
	
	// if first open ui, set data. [1/26/2011 rumist]
	if( !m_bIsPlayAnim )
	{
		_updateCourseInfo();
		_updateRewardInfo();
	}
	else
	{
		_stopAllAnim();
	}
}

void		CUILacaball::_receiveWinInfo(CNetworkMessage* istr )
{
	CTString	strWinnerName;
	INDEX		iWinnerRewardItemIndex = 0;
	(*istr) >> m_strWinner;
	(*istr) >> m_iWinnerItemIdx;
	// print winner info use to notice system.
	_showWinner();
}

void		CUILacaball::_receiveUsedResult(CNetworkMessage* istr )
{
	BYTE	byErrorCode			= 0;
	INDEX	iRewardItemIndex	= 0;
	CUIMsgBox_Info MsgBoxInfo;
	(*istr) >> byErrorCode;
	if( !byErrorCode )
	{
		// if you success, receive reward item index.
		(*istr) >> iRewardItemIndex;
		CItemData* pID = _pNetwork->GetItemData( iRewardItemIndex );
		m_pIconRewardInBall->clearIconData();

		m_pIconRewardInBall->setData(UBET_ITEM, pID->GetItemIndex());

		_startRewardAnim();
		return;
	}
	else if( byErrorCode == 1 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_LACABALL, MSGCMD_LACABALL_ERROR );
		MsgBoxInfo.AddString( _S( 116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다." ) );
	}
	else if( byErrorCode == 2 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_LACABALL, MSGCMD_LACABALL_ERROR );
		MsgBoxInfo.AddString( _S( 4860, "토큰이 부족합니다." ) );
	}
	else
	{
		;//??
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_LACABALL_ERROR ) )
		pUIManager->CloseMessageBox( MSGCMD_LACABALL_ERROR );

	pUIManager->CreateMessageBox( MsgBoxInfo );
	_stopAllAnim();
}

void		CUILacaball::_sendChangeToken(const INDEX _tokenIdx, const UBYTE _course )
{
	CNetworkMessage nmChangeToken( MSG_EXTEND );
	nmChangeToken << (INDEX)MSG_EX_LCBALL;
	nmChangeToken << (UBYTE)MSG_EX_LCBALL_INFO;
	nmChangeToken << (INDEX)_tokenIdx;
	nmChangeToken << (UBYTE)_course;

	_pNetwork->SendToServerNew( nmChangeToken );
}

void		CUILacaball::_sendUseToken(const INDEX _tokenIdx, const UBYTE _course )
{
	CNetworkMessage nmUseToken( MSG_EXTEND );
	nmUseToken << (INDEX)MSG_EX_LCBALL;
	nmUseToken << (UBYTE)MSG_EX_LCBALL_USE;
	nmUseToken << (INDEX)_tokenIdx;
	nmUseToken << (UBYTE)_course;

	_pNetwork->SendToServerNew( nmUseToken );
}

void		CUILacaball::_openLacaball()
{	
	ResetPosition( _pdpMain->dp_MinI, _pdpMain->dp_MinJ, _pdpMain->dp_MaxI, _pdpMain->dp_MaxJ );
	_initData();
	_clearNetData();
	m_bIsPlayAnim = FALSE;
	// stop character.
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StopMove();
	CUIManager::getSingleton()->RearrangeOrder( UI_LACABALL, TRUE );
	// request info at first chance.
	_sendChangeToken( 0, 0 );
}

void		CUILacaball::_closeLacaball()
{
	if( m_bIsPlayAnim )
		return;
	ResetPosition( _pdpMain->dp_MinI, _pdpMain->dp_MinJ, _pdpMain->dp_MaxI, _pdpMain->dp_MaxJ );
	_clearData();
	m_bIsPlayAnim = FALSE;
	CUIManager::getSingleton()->RearrangeOrder( UI_LACABALL, FALSE );
}

void		CUILacaball::_prevToken()
{
	INDEX iBeforePos = m_iCurrentTokenPos;
	if( m_vecTokenInfo.size() == 0 )
		return;
	--m_iCurrentTokenPos;
	// revise.
	if( m_iCurrentTokenPos < 0 )
		m_iCurrentTokenPos = 0;
	// 같을 경우 굳이 요청할 필요가 없다.	
	if( iBeforePos == m_iCurrentTokenPos )
		return;
	// 처음은 항상 1번 코스만 선택됨.
	_sendChangeToken( m_vecTokenInfo[m_iCurrentTokenPos].tokenIndex, 0 );
	m_btnCoursePrev.SetEnable( TRUE );
	m_btnCourseNext.SetEnable( TRUE );
}

void		CUILacaball::_nextToken()
{
	INDEX iBeforePos = m_iCurrentTokenPos;
	if( m_vecTokenInfo.size() == 0 )
		return;
	++m_iCurrentTokenPos;
	// revise
	if( m_vecTokenInfo.size() <= m_iCurrentTokenPos && m_iCurrentTokenPos != 0 )
		m_iCurrentTokenPos = m_vecTokenInfo.size() -1;
	
	// 같을 경우 굳이 요청할 필요가 없다.
	if( iBeforePos == m_iCurrentTokenPos )
		return;

	_sendChangeToken( m_vecTokenInfo[m_iCurrentTokenPos].tokenIndex, 0 );
	m_btnCoursePrev.SetEnable( TRUE );
	m_btnCourseNext.SetEnable( TRUE );
}

void		CUILacaball::_prevCourse()
{
	// 데이터가 변경될 경우에만 업데이트.
	if( _setPrevCourseInfo() )
		_updateCourseInfo();
}

void		CUILacaball::_nextCourse()
{
	// 데이터가 변경될 경우에만 업데이트.
	if( _setNextCourseInfo() )
		_updateCourseInfo();
}

void		CUILacaball::_selectCourse(const INDEX _iSel )
{
	if( m_vecTokenInfo.size() == 0 )
		return;
	m_iCurrentCoursePos = _iSel;
	_sendChangeToken( m_vecTokenInfo[m_iCurrentTokenPos].tokenIndex, m_iCurrentCoursePos );
}

void		CUILacaball::_startGame()
{
	//_sendUseToken( m_vecTokenInfo[m_iCurrentTokenPos].tokenIndex, m_iCurrentCoursePos );
	if( m_vecTokenInfo.size() == 0 )
		return;
	if( m_vecRewardInfo.size() == 0 )
		return;
	if( m_vecCourseInfo.size() == 0 )
		return;

	m_bIsPlayAnim = TRUE;
//	m_LacaballSound.Play_t( MUSIC_LACABALL_START, 0 );
	_startDroppedAnim();
}

const INDEX	CUILacaball::_queryToken(const INDEX _tokenIdx )
{
	_FindTokenPos finder(_tokenIdx);
	std::vector<TOKENINFO>::const_iterator citr = std::find_if( m_vecTokenInfo.begin(), m_vecTokenInfo.end(), finder );
	
	if( citr != m_vecTokenInfo.end() )
		return (*citr).GetTokenPos();
	return 0;		// default pos.
}

const COLOR	CUILacaball::_queryColor(const INDEX _remainCnt, const INDEX _maxCnt )
{
	COLOR retCol = 0xDED9A0FF;				// base color.
	if( _remainCnt <= (_maxCnt/2) )			// 50%
		retCol = 0x00FF00FF;
	if( _remainCnt <= (_maxCnt/5))		// 20%
		retCol = 0xFFFF00FF;
	if( _remainCnt <= (_maxCnt/10))		// 10%
		retCol = 0xFF0000FF;
	
	return retCol;
}

const BOOL	CUILacaball::_setPrevCourseInfo()
{
	int vecCount = m_vecCourseInfo.size();
	if( !vecCount )
		return FALSE;

	--m_iShowCoursePos;
	--m_iCurrentCoursePos;
	if( m_iShowCoursePos < 0 )
		m_iShowCoursePos = 0;
	if( m_iCurrentCoursePos < 0 )
		m_iCurrentCoursePos = 0;

	_sendChangeToken( m_vecTokenInfo[m_iCurrentTokenPos].tokenIndex, m_iCurrentCoursePos );
	_updateCourseInfo();

	return TRUE;
}

const BOOL	CUILacaball::_setNextCourseInfo()
{
	int vecCount = m_vecCourseInfo.size();
	if( !vecCount )
		return FALSE;
	
	if( m_iShowCoursePos+3 > vecCount )
	{
		return FALSE;
	}
	
	++m_iShowCoursePos;
	++m_iCurrentCoursePos;

	if( m_iShowCoursePos + 3 > vecCount )
		--m_iShowCoursePos;
	if( m_iCurrentCoursePos >= vecCount )
		--m_iCurrentCoursePos;

	_sendChangeToken( m_vecTokenInfo[m_iCurrentTokenPos].tokenIndex, m_iCurrentCoursePos );
	_updateCourseInfo();

	return TRUE;
}

void		CUILacaball::_updateCourseInfo()
{	
	int i = 0;
	for(i = 0; i < LACABALL_COURSE_MAX; ++i )
		m_Course[i].Clear();

	size_t vecSize = m_vecCourseInfo.size();

	// safe code.
	for( i = 0; i < LACABALL_COURSE_MAX; ++i )
	{
		if( vecSize > m_iShowCoursePos+i )
			m_Course[i].SettingInfo( m_vecCourseInfo[m_iShowCoursePos+i] );			
	}
	//m_Course[0].SettingInfo( m_vecCourseInfo[m_iShowCoursePos+0] );
	//m_Course[1].SettingInfo( m_vecCourseInfo[m_iShowCoursePos+1] );
	//m_Course[2].SettingInfo( m_vecCourseInfo[m_iShowCoursePos+2] );
}

void		CUILacaball::_updateRewardInfo()
{
	int i = 0;
	for( i = 0; i < LACABALL_REWARD_MAX; ++i )
	{
		m_CourseRewardList[i].Clear();
	}
	
	for( i = 0; i < m_vecRewardInfo.size(); ++i )
	{
		m_CourseRewardList[i].SettingInfo( m_vecRewardInfo[i] );
	}
}

WMSG_RESULT	CUILacaball::_showToopTip(MSG *pMsg )
{
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	int i = 0;

	if( IsInsideRect( nX, nY, m_rcCoursRect ) )
	{
		for( int i = 0; i < LACABALL_COURSE_MAX; ++i )
		{
			if( IsInsideRect( nX, nY, m_Course[i].boxRect ) && m_Course[i].IsUsed() )
			{
				m_Course[i].pIconreward->MouseMessage(pMsg);
			}
		}
	}
	else if( IsInsideRect( nX, nY, m_rcRewardRect ) )
	{
		// show tooltip in reward box.
		for( int i = 0; i < LACABALL_REWARD_MAX; ++i )
		{
			if( IsInsideRect( nX, nY, m_CourseRewardList[i].rewardRect ) && m_CourseRewardList[i].IsUsed() )
			{
				m_CourseRewardList[i].pIconreward->MouseMessage(pMsg);
			}
		}
	}

	return WMSG_FAIL;
}

bool		CUILacaball::_updateDropPos()
{
	const INDEX iDevide = 25;
	const INDEX iRevDevide = 4;
	const float fDevide = 24.99f;
	bool bRet = false;
	// BUG FIX : lacaball [4/28/2011 rumist]
	INDEX pos = iRevDevide - (INDEX)(((float)m_Course[m_iCurrentCoursePos - m_iShowCoursePos].rewardItemRemainCount / m_Course[m_iCurrentCoursePos - m_iShowCoursePos].rewardItemMaxCount)*100+fDevide) / iDevide;
	
// 	if( m_iDropPosGap < pos )
// 	{
 		m_iDropPosGap = pos;
// 		return true;
// 	}
 	return false;
}

void		CUILacaball::_startDroppedAnim()
{
	_initAnimProperties();
	m_bStartAnim = true;
	m_iCurrentAnim = LCBALL_ANIM_BASE;
	m_animBall.StartAnim();
	_lockBtn();
}

void		CUILacaball::_startRewardAnim()
{
	m_iCurrentAnim = LCBALL_ANIM_REWARD;
}

void		CUILacaball::_stopAllAnim()
{
	_initAnimProperties();
	_updateCourseInfo();
	_updateRewardInfo();
	m_bIsPlayAnim = FALSE;
}

void		CUILacaball::_initAnimProperties()
{
	m_bStartAnim = false;
	m_iCurrentAnim = LCBALL_ANIM_BASE;
	m_bShowDropEndAnim	= false;
	m_bShowRewardEndAnim = false;
	m_sqShowingTime = 0;
	_unlockBtn();
}

void		CUILacaball::_setBtnStatus(const BOOL bLocked )
{
	m_btnClose.SetEnable( bLocked );
	m_btnStart.SetEnable( bLocked );
	m_btnTokenPrev.SetEnable( bLocked );
	m_btnTokenNext.SetEnable( bLocked );
	m_btnCoursePrev.SetEnable( bLocked );
	m_btnCourseNext.SetEnable( bLocked );
}


void		CUILacaball::_showWinner()
{
	CTString strMsg;
	strMsg.PrintF( _S( 5349, "%s님이 %s 아이템을 획득하였습니다."), m_strWinner, _pNetwork->GetItemName( m_iWinnerItemIdx ) );
	_UIAutoHelp->SetGMNotice( strMsg );
	// winner 가 등장했을 때만 리스트가 초기화 됨.
	_resetDropPos();
}


const BOOL	CUILacaball::_queryTimer(const SQUAD _limit )
{
	SQUAD currentTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if( currentTime - m_sqStartTime > _limit )
	{
		m_sqStartTime = currentTime;
		return TRUE;
	}

	return FALSE;
}

// public functions. [1/11/2011 rumist]
void		CUILacaball::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, LACABALL_UI_WIDTH, LACABALL_UI_HEIGHT);
	
	// load texture.
	m_ptdBaseTexture = CreateTexture( CTString("Data\\Interface\\lacaball.tex"));
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rcTitle.SetRect( 0, 0, nWidth, 37 );
	
	_initBackground( fTexWidth, fTexHeight );
	_initCourseInfo( fTexWidth, fTexHeight );
	_initBallInfo( fTexWidth, fTexHeight );
	_initRewardBallInfo( fTexWidth, fTexHeight );

	m_LacaballSound.SetVolume( 1.0f );
}

void		CUILacaball::Render()
{
	CUIManager::getSingleton()->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
	
	_renderBackground();
	_renderBallInfo();
	_renderCourseInfo();
	_renderAnim();
}

void		CUILacaball::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void		CUILacaball::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

WMSG_RESULT	CUILacaball::MouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );


	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;

				m_btnClose.MouseMessage( pMsg );
				m_btnStart.MouseMessage( pMsg );
				m_btnTokenPrev.MouseMessage( pMsg );
				m_btnTokenNext.MouseMessage( pMsg );
				m_btnCoursePrev.MouseMessage( pMsg );
				m_btnCourseNext.MouseMessage( pMsg );

				// Move UI window.
				if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
				{
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				else
				{	// show tooltip in course box.
					if( WMSG_FAIL != _showToopTip( pMsg ) )
						return WMSG_SUCCESS;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					if( !pUIManager->DoesMessageBoxExist( MSGCMD_LACABALL_ERROR ) )
					{
						pUIManager->RearrangeOrder( UI_LACABALL, TRUE );
					}
					nOldX = nX;
					nOldY = nY;

					if( WMSG_FAIL != m_btnClose.MouseMessage( pMsg ) )
					{
						// nothing.
					}
					else if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						// UI 움직임 금지. // ITS 796 관련 수정.
						//m_bPickTitle = TRUE;
					}
					else if( WMSG_FAIL != m_btnStart.MouseMessage( pMsg ) )
					{
						// nothing.
					}
					else if( WMSG_FAIL != m_btnTokenPrev.MouseMessage( pMsg ) )
					{
						// nothing.
					}
					else if( WMSG_FAIL != m_btnTokenNext.MouseMessage( pMsg ) )
					{
						// nothing.
					}
					else if( WMSG_FAIL != m_btnCoursePrev.MouseMessage( pMsg ) )
					{
						// nothing.
					}
					else if( WMSG_FAIL != m_btnCourseNext.MouseMessage( pMsg ) )
					{
						// nothing.
					}
					else if( IsInsideRect( nX, nY, m_rcCoursRect ) )
					{
						// nothing.
					}
					else if( IsInsideRect( nX, nY, m_rcRewardRect ) )
					{
						// nothing.
					}
					return WMSG_SUCCESS;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				m_bPickTitle = FALSE;
				if( IsInside( nX, nY ) )
				{
					// 애니메이션 중에는 아무 동작도 할 수 없다.
					if( m_bIsPlayAnim )
						return WMSG_SUCCESS;

					if( WMSG_FAIL != ( wmsgResult = m_btnClose.MouseMessage( pMsg) ) )
					{
						if( WMSG_COMMAND == wmsgResult )
							_closeLacaball();
					}
					else if( WMSG_FAIL != ( wmsgResult = m_btnStart.MouseMessage( pMsg ) ) )
					{
						if( WMSG_COMMAND == wmsgResult )
							_startGame();
					}
					else if( WMSG_FAIL != (wmsgResult = m_btnTokenPrev.MouseMessage( pMsg ) ) )
					{
						if( WMSG_COMMAND == wmsgResult )
							_prevToken();
					}
					else if( WMSG_FAIL != ( wmsgResult = m_btnTokenNext.MouseMessage( pMsg ) ) )
					{
						if( WMSG_COMMAND == wmsgResult )
							_nextToken();
					}
					else if( IsInsideRect( nX, nY, m_rcCoursRect ) )
					{
						if( WMSG_FAIL != ( wmsgResult = m_btnCoursePrev.MouseMessage( pMsg ) ) )
						{
							if( WMSG_COMMAND == wmsgResult )
								_prevCourse();
						}
						else if( WMSG_FAIL != ( wmsgResult = m_btnCourseNext.MouseMessage( pMsg ) ) )
						{
							if( WMSG_COMMAND == wmsgResult )
								_nextCourse();
						}
						else
						{
							for( int i = 0; i < LACABALL_COURSE_MAX; ++i )
							{
								if( IsInsideRect( nX, nY, m_Course[i].boxRect ) )
								{
									_selectCourse( m_Course[i].courseIdx );
									// escape loop.
									break;
								}
							}
						}
					}
					else if( IsInsideRect( nX, nY, m_rcRewardRect ) )
					{
						for( int i = 0; i < LACABALL_REWARD_MAX; ++i )
						{
							if( IsInsideRect( nX, nY, m_CourseRewardList[i].rewardRect ) )
							{
								; // nothing.
							}
						}
					}
					return WMSG_SUCCESS;
				}
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
				// ITS 796 관련 수정.
				// 아이리스 상점 위로 드래그 후 db click 시 send error 현상 수정.
				// ITS 889 : DB 클릭을 통한 아이템 사용 블럭 처리.
				return WMSG_SUCCESS;
			}
			break;
	}
	return WMSG_FAIL;
}

// lacaball network functions. [1/11/2011 rumist]
// receive from server.
void		CUILacaball::ReceiveLcBallMessage(CNetworkMessage* istr )
{
	UCHAR chrSubType;

	(*istr) >> chrSubType;

	switch( chrSubType )
	{
		case MSG_EX_LCBALL_INFO:
			_receiveInfo( istr );
			break;
		case MSG_EX_LCBALL_MSG:
			_receiveWinInfo( istr );
			break;
		case MSG_EX_LCBALL_USE:
			_receiveUsedResult( istr );		// include error process.
			break;
	}
}

// lacaball user definition functions.
void		CUILacaball::ToggleVisible()
{
	if( IsVisible() )
		_closeLacaball();
	else
		_openLacaball();
}

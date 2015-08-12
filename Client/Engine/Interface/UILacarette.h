// ***************************************************************
//  UILacarette   version:  1.0   ? date: 03/03/2010
//  -------------------------------------------------------------
//	Author 	: sora
//  Desc	: 라카렛 시스템
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef UILACARETTE_H_
#define UILACARETTE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define LACARETTE_WIDTH		(615)
#define LACARETTE_HEIGHT	(402)

#include <map>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Interface/UIIcon.h>

class CUILacaball : public CUIWindow
{
	// User Constant Variables.
public:
	enum __tagLacaball_UI
	{
		LACABALL_UI_POS_X			= 100,
		LACABALL_UI_POS_Y			= 100,
		LACABALL_UI_WIDTH			= 500,
		LACABALL_UI_HEIGHT			= 375,
		LACABALL_COURSE_MAX			= 3,
		LACABALL_REWARD_MAX			= 5
	};

	typedef enum __tagLacaball_Anim_Sequence
	{
		LCBALL_ANIM_BASE	= 0,
		LCBALL_ANIM_SHAKING = 1,
		LCBALL_ANIM_DROP	= 2,
		LCBALL_ANIM_REWARD	= 3,
		LCBALL_ANIM_END		= 4,
	} A_SEQUENCE;
	
	typedef struct _TokenInfo
	{
		INDEX						tokenIndex;
		CTString					tokenName;
		BYTE						tokenPos;

		_TokenInfo() : tokenIndex(-1), tokenPos(0)	{}
		void						SettingTokenInfo( const INDEX _tokenIdx, const BYTE _tokenPos );
		const BYTE					GetTokenPos() const					{ return tokenPos;						}
	} TOKENINFO, *PTOKENINFO;
	
	typedef struct _FindTokenPos
	{
		INDEX						targetTokenIndex;
		_FindTokenPos( const INDEX _targetIdx )	: targetTokenIndex(0)	{ SetTarget(_targetIdx);				}
		void						SetTarget( const INDEX _targetIdx )	{ this->targetTokenIndex = _targetIdx;	}
		bool						operator()	( TOKENINFO& _other )	
									{ if( targetTokenIndex == _other.tokenIndex ) return true; return false;	}
	};

	typedef struct _CourseInfo	// network info.
	{
		INDEX						rewardItem;
		ULONG						rewardItemCount;
		INDEX						rewardItemRemainCount;
		INDEX						rewardItemMaxCount;
		BYTE						courseIdx;				// position.

		_CourseInfo()													{ Init();								}
		_CourseInfo(  const _CourseInfo& other )						{ Init(); *this = other;				}
		~_CourseInfo()													{}

		void						Init()
		{
			rewardItem = 0;
			rewardItemCount = 0;
			rewardItemMaxCount = 0;
			rewardItemRemainCount = 0;
			courseIdx = 0;
		}

		_CourseInfo&				operator= (  const _CourseInfo& other )
		{
			this->rewardItem = other.rewardItem;
			this->rewardItemCount = other.rewardItemCount;
			this->rewardItemRemainCount = other.rewardItemRemainCount;
			this->rewardItemMaxCount = other.rewardItemMaxCount;
			this->courseIdx = other.courseIdx;

			return *this;
		}
	} COURSEINFO, *PCOURSEINFO, REWARDINFO, *PREWARDINFO;

	typedef struct _SubCourseBox	// render info.
	{
		CUIRectSurface				boxSurface;
		UIRect						boxRect;
		BOOL						empty;
		CUIIcon*					pIconreward;		// render only. not move & click & drag.
		INDEX						rewardItemRemainCount;
		INDEX						rewardItemMaxCount;
		BYTE						courseIdx;

		_SubCourseBox() : rewardItemRemainCount(0), rewardItemMaxCount(0), empty(TRUE), courseIdx(0) 
		{
			pIconreward = new CUIIcon;
		}
		~_SubCourseBox()
		{
			SAFE_DELETE(pIconreward);
		}
		void						SettingItemInfo( const INDEX _rewardItemIdx );
		void						SettingItemCount( const SQUAD _rewardItemCount )	{ pIconreward->setCount(_rewardItemCount);	}
		void						SettingInfo( const COURSEINFO& _src )
		{
			this->rewardItemRemainCount = _src.rewardItemRemainCount;
			this->rewardItemMaxCount = _src.rewardItemMaxCount;
			this->SettingItemInfo( _src.rewardItem );
			this->SettingItemCount( _src.rewardItemCount );
			this->courseIdx = _src.courseIdx;
			Used();
		}
		void						Used()												{ empty = FALSE;				}
		void						Restore()											{ empty = TRUE;					}
		const BOOL					IsUsed() const										{ return !empty;				}
		void						Clear();

		_SubCourseBox&				operator= ( const COURSEINFO& _course )				{ SettingInfo( _course ); return *this;	}

	} SUBCOURSEBOX, *PSUBCOURSEBOX;

	typedef struct _SubCourse_Reward
	{
		CUIRectSurface				rewardSurface;
		UIRect						rewardRect;		// support selecting??
		BOOL						empty;			// true : not has data.
		CUIIcon*					pIconreward;		// render & data container only.
		INDEX						rewardItemRemainCount;
		INDEX						rewardItemMaxCount;
		_SubCourse_Reward() : rewardItemRemainCount(0), rewardItemMaxCount(0), empty(TRUE) 
		{
			pIconreward = new CUIIcon;
		}
		~_SubCourse_Reward()		
		{
			SAFE_DELETE(pIconreward);
		}
		void						SettingItemInfo( const INDEX _rewardItemIdx );
		void						SettingItemCount( const SQUAD _rewardItemCount )	{ pIconreward->setCount(_rewardItemCount);	}
		void						SettingInfo( const REWARDINFO& _src )
		{
			this->rewardItemRemainCount = _src.rewardItemRemainCount;
			this->rewardItemMaxCount = _src.rewardItemMaxCount;
			this->SettingItemInfo( _src.rewardItem );
			this->SettingItemCount( _src.rewardItemCount );
			Used();
		}
		void						Used()												{ empty = FALSE;					}
		void						Restore()											{ empty = TRUE;						}
		const BOOL					IsUsed() const										{ return !empty;					}
		void						Clear();
		
	} SUBCOURSE_REWARD, *PSUBCOURSE_REWARD;

	// User Functions.
private:
protected:
	// Initialize Functions.
	void				_initBackground( const FLOAT fTexWidth, const FLOAT fTexHeight );
	void				_initCourseInfo( const FLOAT fTexWidth, const FLOAT fTexHeight );
	void				_initBallInfo( const FLOAT fTexWidth, const FLOAT fTexHeight );
	void				_initRewardBallInfo( const FLOAT fTexWidth, const FLOAT fTexHeight );
	void				_clearData();
	void				_clearCourseData();
	void				_clearRewardData();
	void				_clearNetData();
	void				_clearNetCourseData();
	void				_clearNetRewardData();
	void				_initData();
	// Render Functions.
	void				_renderBackground();
	void				_renderCourseInfo();
	void				_renderBallInfo();
	void				_renderAnim();
	
	void				_showWinner();

	// Network Functions.
	void				_receiveInfo( CNetworkMessage* istr );
	void				_receiveWinInfo( CNetworkMessage* istr );
	void				_receiveUsedResult( CNetworkMessage* istr );
	void				_sendChangeToken( const INDEX _tokenIdx, const UBYTE _course );
	void				_sendUseToken( const INDEX _tokenIdx, const UBYTE _course );
	// User Functions.
	void				_openLacaball();
	void				_closeLacaball();
	void				_prevToken();
	void				_nextToken();
	void				_prevCourse();
	void				_nextCourse();
	void				_selectCourse( const INDEX _iSel );
	void				_startGame();
	const INDEX			_queryToken( const INDEX _tokenIdx );
	const COLOR			_queryColor( const INDEX _remainCnt, const INDEX _maxCnt );
	void				_updateCourseInfo();

	void				_updateRewardInfo();

	const BOOL			_setPrevCourseInfo();
	const BOOL			_setNextCourseInfo();
	WMSG_RESULT			_showToopTip( MSG *pMsg );

	bool				_updateDropPos();
	void				_resetDropPos()								{ m_iDropPosGap = 0;							}
	void				_calcNextAnim( UIRect& rcPos );
	void				_initNexAnimInfo();

	// new func. [1/24/2011 rumist]
	void				_startDroppedAnim();
	void				_startRewardAnim();
	void				_stopAllAnim();

	// animation sequence control functions.
	void				_initAnimProperties();

	// button locked functions in animations.
	void				_setBtnStatus( const BOOL bLocked );
	void				_lockBtn()									{ _setBtnStatus( FALSE );						}
	void				_unlockBtn()								{ _setBtnStatus( TRUE );						}

	// timer functions.
	const BOOL			_queryTimer( const SQUAD _limit );
public:
						CUILacaball();
						~CUILacaball();


	// Virtual Functions.
	void				Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void				Render();
	void				AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void				ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	WMSG_RESULT			MouseMessage(MSG *pMsg );
	BOOL				CloseWindowByEsc()							{ ToggleVisible(); return TRUE;					}

	// Network Functions.
	void				ReceiveLcBallMessage( CNetworkMessage* istr );
	// User Function.
	void				ToggleVisible();
	void				InitLacaballData()							{ _initData(); m_bIsPlayAnim = TRUE;			}

// User Properties.
private:
	CUIRectSurface		m_rtsBase;
	CUIButton			m_btnClose;

	UIRect				m_rcTitle;
	CTString			m_strTitle;
	// flags.
	BOOL				m_bShowToolTip;
	BOOL				m_bPickTitle;
	UIRectUV			m_rtBackground;
	UIRectUV			m_rtItemBlankCap;

	CUIButton			m_btnStart;
	
	// token info.
	CUIButton			m_btnTokenPrev;
	CUIButton			m_btnTokenNext;
	std::vector<TOKENINFO>	m_vecTokenInfo;

	// course info.
	std::vector<COURSEINFO>	m_vecCourseInfo;
	INDEX				m_iShowCoursePos;

	// reward info.
	std::vector<REWARDINFO>	m_vecRewardInfo;

	// course render info.
	UIRect				m_rcCoursRect;
	SUBCOURSEBOX		m_Course[LACABALL_COURSE_MAX];
	UIRectUV			m_rtSelCourse;
	CUIButton			m_btnCoursePrev;
	CUIButton			m_btnCourseNext;

	// reward info.
	UIRect				m_rcRewardRect;
	SUBCOURSE_REWARD	m_CourseRewardList[LACABALL_REWARD_MAX];
	UIRectUV			m_rtDisableL;
	UIRectUV			m_rtDisableC;
	UIRectUV			m_rtDisableR;

	// init info.
	INDEX				m_iCurrentTokenPos;	
	INDEX				m_iCurrentCoursePos;
	BOOL				m_bIsPlayAnim;

	// ball animation.
	CUIRectSurface		m_rtsLeftBase;
	CUIAnim				m_animBall;
	INDEX				m_iDropPosGap;

	// drop animation.
	UIRectUV			m_rcBall;
	UIRectUV			m_rcBasket;
	INDEX				m_iDropAnimGap;
	BOOL				m_bBoundBall;

	// winner info.
	CTString			m_strWinner;
	INDEX				m_iWinnerItemIdx;

	// reward item animation.
	UIRectUV			m_rtLeftHalfBall;
	UIRectUV			m_rtRightHalfBall;
	UIRectUV			m_rtCCHalfBall;
	CUIRectSurface		m_rtsBall;
	CUIIcon*			m_pIconRewardInBall;
	int					m_nRewardAnimGap;
	BYTE				m_byAlpha;


	// new render properties.
	BOOL				m_bStartAnim;
	INDEX				m_iCurrentAnim;
	BOOL				m_bShowDropEndAnim;
	BOOL				m_bShowRewardEndAnim;
	SQUAD				m_sqShowingTime;

	// timer properties.
	SQUAD				m_sqStartTime;

	// sounds.
	CSoundObject		m_LacaballSound;

protected:
public:
};

#endif // UILACARETTE_H_
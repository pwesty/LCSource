/********************************************************************
	created:	2010/04/20
	created:	20:4:2010   12:41
	filename: 	D:\LAST_CHAOS\SOURCE\LASTCHAOS_KOR\ENGINE\INTERFACE\UIProgressBar.h
	file path:	D:\LAST_CHAOS\SOURCE\LASTCHAOS_KOR\ENGINE\INTERFACE
	file base:	UIProgressBar
	file ext:	h
	author:		rumist
	
	purpose:	ui windows class about progress bar.
*********************************************************************/

#ifndef UIPROGRESSBAR_H__
#define UIPROGRESSBAR_H__

#ifdef PRAGMA_ONCE
	#pragma once
#endif

typedef enum _tagProgressStatus
{
	PR_STAT_IDLE = 0,
	PR_STAT_DOING,
	PR_STAT_COMPLETE,
	PR_STAT_TAG_END,
} STAT;

#define		UI_PROGRESS_TEX_X			 266
#define		UI_PROGRESS_TEX_Y			 42
#define		UI_PROGRESS_INNER_TEX_X		 428
#define		UI_PROGRESS_INNER_TEX_Y		 44
#define		UI_PROGRESS_INNER_TEX_WIDTH	 12
#define		UI_PROGRESS_INNER_TEX_HEIGHT 16

class ENGINE_API CUIProgressBar : public CUIWindow
{
protected:
	enum _tagProgressSurfaceRect
	{
		TL = 0,
		TM,
		TR,
		ML,
		MM,
		MR,
		BL,
		BM,
		BR,
		FRAME_TAG_END,
	};

	CUIRectSurface			m_rtsBase[FRAME_TAG_END];
	UIRect					m_rtBar;
	UIRectUV				m_rtuBar;

	_tagProgressStatus		m_enStatus;
	FLOAT					m_fCurPercent;

	CTString				m_strText;
	// 스트링별 테스트.
	CTString				m_strCompleteText;
	CTString				m_strDoingText;
	CTString				m_strIdleText;

	COLOR					m_colText;
	COLOR					m_colCompleteText;
	COLOR					m_colDoingText;
	COLOR					m_colIdleText;

	BOOL					m_bShowPercentInComplete;
	BOOL					m_bShowPercentInDoing;
	BOOL					m_bShowPercentInIdle;
	BOOL					m_bAutoProgressComplete;

	SQUAD					m_sqLower;
	SQUAD					m_sqUpper;
	SQUAD					m_sqOffset;
	SQUAD					m_sqStep;
	SQUAD					m_sqCurrentPos;
	FLOAT					m_nTexGap;
	BOOL					m_bShowText;
	BOOL					m_bShowPercentText;
	void					_setTextureGap();
	const FLOAT				_calcProgressPercent();
public:
	CUIProgressBar();
	~CUIProgressBar();

	CUIBase* Clone();

	void					Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void					Render();

	// SET PROPERTIES
	void					SetText( CTString &strText, BOOL _bShowPercentText = TRUE, COLOR colText = 0xFFFFFFFF )	
							{ 
								m_strText = strText; 
								SetIdleText( strText );
								SetCompleteText( strText );
								SetDoingText( strText );
								SetTextColor( colText ); 
								SetIdleTextColor( colText );
								SetCompleteTextColor( colText );
								SetDoingTextColor( colText );
								SetShowPercentText(_bShowPercentText);		
								SetShowIdlePercent( _bShowPercentText );
								SetShowCompletePercent( _bShowPercentText );
								SetShowDoingPercent( _bShowPercentText );
							}
	void					SetIdleText( CTString &strText, BOOL _bShowPercent = FALSE, COLOR colText = 0xFFFFFFF )
							{ m_strIdleText = strText; SetIdleTextColor(colText); SetShowIdlePercent(_bShowPercent);				}
	void					SetCompleteText( CTString &strText, BOOL _bShowPercent = FALSE, COLOR colText = 0xFFFFFFF )
							{ m_strCompleteText = strText; SetCompleteTextColor(colText); SetShowCompletePercent(_bShowPercent);	}
	void					SetDoingText( CTString &strText, BOOL _bShowPercent = FALSE, COLOR colText = 0xFFFFFFF )
							{ m_strDoingText = strText; SetDoingTextColor(colText); SetShowDoingPercent(_bShowPercent);				}
	void					SetTextColor( COLOR colText )				{ m_colText			= colText;			}
	void					SetIdleTextColor( COLOR colText )			{ m_colIdleText		= colText;			}
	void					SetCompleteTextColor( COLOR colText )		{ m_colCompleteText = colText;			}
	void					SetDoingTextColor( COLOR colText )			{ m_colDoingText	= colText;			}
	void					SetShowText( BOOL _show = TRUE )			{ m_bShowText		= _show;			}
	void					SetShowPercentText( BOOL _show = TRUE )		{ m_bShowPercentText		= _show;	}
	void					SetShowIdlePercent( BOOL _show = TRUE )		{ m_bShowPercentInIdle		= _show;	}
	void					SetShowCompletePercent( BOOL _show = TRUE )	{ m_bShowPercentInComplete	= _show;	}
	void					SetShowDoingPercent( BOOL _show = TRUE )	{ m_bShowPercentInDoing		= _show;	}
	void					SetEnable( BOOL bEnable )
							{
								if( bEnable )
								{
									m_dwWndState |= UWS_ENABLE;
								}
								else
								{
									m_dwWndState &= ~UWS_ENABLE;
								}
							}
	inline	const STAT		SetStatus( STAT _status = PR_STAT_IDLE )	{ STAT temp = m_enStatus; m_enStatus = _status; return temp; }
	inline	void			SetAutoComplete( BOOL _auto = TRUE )		{ m_bAutoProgressComplete = _auto;	}
	inline	void			StartProgress()								{ m_enStatus = PR_STAT_DOING;	}
	inline	void			IdleProgress()								{ m_enStatus = PR_STAT_IDLE;	}
	inline	void			CompleteProgress();

	inline	void			SetProgressRange( SQUAD sqRower = 0, SQUAD sqUpper = 100 )	
							{ ASSERT( sqRower < sqUpper ); m_sqLower = sqRower; m_sqUpper = sqUpper;	_setTextureGap();	}
	inline const SQUAD		SetProgressPos( SQUAD sqPos )		
							{
								SQUAD retVal = m_sqCurrentPos;
								m_sqCurrentPos = sqPos;	
								m_sqCurrentPos > m_sqUpper ? m_sqCurrentPos=m_sqUpper:m_sqCurrentPos;
								return retVal;
							}
	inline const SQUAD		OffsetProgressPos( SQUAD sqPos )	
							{
								SQUAD retVal = m_sqCurrentPos;
								m_sqCurrentPos = sqPos;
								m_sqCurrentPos > m_sqUpper ? m_sqCurrentPos=m_sqUpper:m_sqCurrentPos;
								return retVal;
							}
	inline const SQUAD		SetProgressStep( SQUAD sqStep )	
							{ 
								SQUAD retVal = m_sqStep;
								m_sqStep = sqStep;
								m_sqStep > m_sqUpper ? m_sqStep=m_sqUpper:m_sqStep;
								return retVal;
							}
	inline const SQUAD		ProgressStepIt()
							{
								SQUAD retVal = m_sqCurrentPos;
								m_sqCurrentPos += m_sqStep;
								m_sqCurrentPos > m_sqUpper ? m_sqCurrentPos=m_sqUpper:m_sqCurrentPos;
								return retVal;
							}
	// GET PROPERTIES
	inline const COLOR		GetTextColor()		const					{ return m_colText;					}

	inline const COLOR		GetIdleTextColor()			const			{ return m_colIdleText;				}
	inline const COLOR		GetCompleteTextColor()		const			{ return m_colCompleteText;			}
	inline const COLOR		GetDoingTextColor()			const			{ return m_colDoingText;			}

	inline const BOOL		IsShowPercentText() const					{ return m_bShowPercentText;		}
	
	inline const BOOL		IsShowIdlePercentText()		const			{ return m_bShowPercentInIdle;		}
	inline const BOOL		IsShowCompletePercentText() const			{ return m_bShowPercentInComplete;	}
	inline const BOOL		IsShowDoingPercentText()	const			{ return m_bShowPercentInDoing;		}

	inline const CTString	GetText()			const					{ return m_strText;					}

	inline const CTString	GetIdleText()				const			{ return m_strIdleText;				}
	inline const CTString	GetCompleteText()			const			{ return m_strCompleteText;			}
	inline const CTString	GetDoingText()				const			{ return m_strDoingText;			}

	inline const STAT		GetStatus()					const			{ return m_enStatus;				}

	inline const int		GetProgressPos()	const					{ return m_sqCurrentPos;	}
	inline	void			GetProgressRange( int& nLower, int& nUpper ) const	{ nLower = m_sqLower; nUpper = m_sqUpper;	}

	inline BOOL				IsDoing()					const			{ return m_enStatus == PR_STAT_DOING;	}
	inline BOOL				IsComplete()				const			{ return m_enStatus == PR_STAT_COMPLETE;}
	inline BOOL				IsIdle()					const			{ return m_enStatus == PR_STAT_IDLE;	}

// UI툴에서 사용할 인터페이스. 기존 코드의 복잡성으로 인해 UI툴에서 필요한 인터페이스들 정의.
public:
	int			getStep()												{ return m_sqStep; }
	BOOL		getAutoComplete()										{ return m_bAutoProgressComplete; }
	BOOL		getShowPercentText( _tagProgressStatus state )			{ return m_bPercentText[state]; }
	UIRectUV	getUVBack();
	UIRectUV	getUVBar();
	void		getBarStartPos(int& nSX, int& nSY)						{ nSX = m_nBarSX; nSY = m_nBarSY; }
	COLOR		getColProgressText( _tagProgressStatus state )			{ return m_colProgressText[state]; }
	char*		getProgressText( _tagProgressStatus state ) 
				{ return m_strProgressText[state].str_String; }

	void		setUVBack( UIRectUV uv ); 
	void		setUVBar( UIRectUV uv );
	void		setBarStartPos(int nSX, int nSY)						
				{ 
					m_nBarSX = nSX;	
					m_nBarSY = nSY; 
					_setTextureGap();
				}
	
	void		setProgressTextColor( _tagProgressStatus state, COLOR colText ) { m_colProgressText[state] = colText; }
	void		setProgressText( _tagProgressStatus state, CTString &strText,
					BOOL _bShowPercent = FALSE, COLOR colText = 0xFFFFFFF );

	void SetCmdComplete(Command* pCmd)	{SAFE_DELETE(m_pCmdComplete); m_pCmdComplete = pCmd;}
#ifdef UI_TOOL
	void		setStringIdx(_tagProgressStatus eState, INDEX idx)		{ m_iStringIdx[eState] = idx; }
	INDEX		getStringIdx(_tagProgressStatus eState)					{ return m_iStringIdx[eState];		}
private:
	INDEX		m_iStringIdx[PR_STAT_TAG_END];
#endif // UI_TOOL

private:
	UIRectUV	m_uvBack;
	CTString	m_strProgressText[PR_STAT_TAG_END];
	BOOL		m_bPercentText[PR_STAT_TAG_END];
	COLOR		m_colProgressText[PR_STAT_TAG_END];
	int			m_nBarSX;
	int			m_nBarSY;
	int			m_nStartTime;
	Command*	m_pCmdComplete;

	void		UpdateBarPos();
protected:
	void OnRender(CDrawPort* pDraw);
	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	void initialize();
};

#endif // UIPROGRESSBAR_H__
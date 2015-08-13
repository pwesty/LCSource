#ifndef		UI_GPS_H_
#define		UI_GPS_H_

#include <Engine/Interface/UIWindow.h>

class CUIEditBox;
class CUITextBox;
class CUIText;
class CUIImage;

class CUICompass : public CUIWindow
{
public:
	CUICompass();

	enum eCOVER
	{
		eCOVER_NORMAL,
		eCOVER_RED,
		eCOVER_GREEN,
		
		eCOVER_MAX
	};

	void initialize();
	void showCompass(bool bShow);
	void setAngle(float fAngle);
	void setCover(eCOVER eType);

	void Open();
	void Close();

	WMSG_RESULT OnLButtonDown( UINT16 x, UINT16 y );
	WMSG_RESULT OnLButtonUp( UINT16 x, UINT16 y );
	WMSG_RESULT OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg );

private:
	bool		m_bClick;
	CUIImage*	m_pImgQuestion;
	CUIImage*	m_pImgCompass;
	CUIImage*	m_pImgCover[3];
	UINT16		ox, oy;
};

class CUIGPS : public CUIWindow
{
public:
	CUIGPS();
	~CUIGPS();

	void initialize();	
	
	void Open();
	void Close();

	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	BOOL IsEditBoxFocused();

	void FindUser();

	void OnUpdate(float fDeltaTime, ULONG ElapsedTime);

	WMSG_RESULT OnLButtonDown( UINT16 x, UINT16 y );
	WMSG_RESULT OnLButtonUp( UINT16 x, UINT16 y );
	WMSG_RESULT OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg );
	WMSG_RESULT OnKeyMessage( MSG* pMsg );

private:
	CUIEditBox*		m_pEdit;
	CUITextBox*		m_pTxtBox;
	CUIText*		m_pTxtTitle;
	bool			m_bTitleClick;
	CUIImage*		m_pImgIn;
	UINT16			ox, oy;
	float			m_fDeltaIn;
};

#endif		// UI_GPS_H_
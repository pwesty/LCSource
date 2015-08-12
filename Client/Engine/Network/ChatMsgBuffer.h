#ifndef __CHAT_MSB_BUFFER_H__
#define __CHAT_MSB_BUFFER_H__

#include <Engine/Base/CTString.h>

// Define chat message buffer
#define	CHATMSG_LINE_MAX			3
#define	CHATMSG_CHAR_MAX			40
#define	CHATMSG_SQRDIST				900.0f			// 30m
#define	CHATMSG_TIME_DELAY			7000			// milliseconds

// ----------------------------------------------------------------------------
// Name : ChatMsgBuffer
// Desc :
// ----------------------------------------------------------------------------
class ChatMsgBuffer
{
protected:
	UBYTE		m_ubMsgCount;
	CTString	m_strMsg[CHATMSG_LINE_MAX];
	COLOR		m_colMsg;
	int			m_nWidth;
	__int64		m_llMsgTime;
	BOOL		m_bShowNPCTalk;

protected:
	void	AddString( CTString &strMessage );

public:
	ChatMsgBuffer();
	~ChatMsgBuffer();

	void	Reset() { m_ubMsgCount = 0; }
	void	SetChatMsg( CTString &strMessage, COLOR colMessage = 0xC5C5C5FF, BOOL bShowNPCTalk = FALSE );

	void		CheckShowNPCTalk();

	UBYTE		GetCount() const { return m_ubMsgCount; }
	CTString	&GetString( int nIndex ) { return m_strMsg[nIndex]; }
	COLOR		GetColor() const { return m_colMsg; }
	int			GetWidth() const { return m_nWidth; }
	__int64		GetTime() const { return m_llMsgTime; }
};

#endif //__CHAT_MSB_BUFFER_H__
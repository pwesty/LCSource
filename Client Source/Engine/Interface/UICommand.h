
#ifndef		UI_COMMNAD_H_
#define		UI_COMMNAD_H_

#include <Engine/Base/Command.h>
#include <Engine/Interface/UITab.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Network/CNetwork.h>

enum eCMD_ID
{
	eCMD_ITEMGET = 0,
	eCMD_GONPC,
};
// Command

class CmdSelectTab : public Command
{
public:
	CmdSelectTab() : _prarent(NULL), _idx(-1) {}
	~CmdSelectTab() {}

	void setData(CUITab* parent, int idx)	{ _prarent = parent; _idx = idx; }
	void execute()
	{
		if (_prarent && _idx >= 0)
			_prarent->OnChangeTab(_idx);
	}

protected:
	int			_idx;
	CUITab*		_prarent;
};

class CmdClose : public Command
{
public:
	CmdClose() : _pParent(NULL), _idx(UI_NONE) {}

	void setData(CUIBase* parent, int idx) { _pParent = parent; _idx = idx; }
	void execute()
	{
		if (_pParent != NULL)
		{
			_pParent->Hide(TRUE);

			CUIManager::getSingleton()->RearrangeOrder(_idx, FALSE);

			// 사용된 포커스가 있을 수 있다. 빼주자..
			CUIFocus::getSingleton()->setUI(NULL);		
		}
	}


private:
	CUIBase*	_pParent;
	int			_idx;

};

class CmdDragIcon : public Command
{
public:
	CmdDragIcon() : _pIcon(NULL) {}
	void setData(CUIIcon* pIcon)	
	{
		_pIcon = pIcon;
	}
	void execute()
	{
		if (_pIcon != NULL)
			UIMGR()->SetHoldBtn(_pIcon);
	}
private:
	CUIIcon*	_pIcon;
};

class CmdItemGet : public Command
{
public:
	CmdItemGet() : m_nItemIndex(-1)	{}
	CmdItemGet(int nItemIndex) : m_nItemIndex(nItemIndex) {}
	void setData(int nItemIndex)	{ m_nItemIndex = nItemIndex;	}
	void execute()
	{
#ifdef	VER_TEST
		if (m_nItemIndex < 0)
			return;

		CTString str;
		CItemData* pItemData = CItemData::getData(m_nItemIndex);

		if (pItemData->GetStack() > 0)
			str.PrintF("itemget %d 0 0 %d", m_nItemIndex, pItemData->GetStack());
		else
			str.PrintF("itemget %d", m_nItemIndex);

		_pNetwork->SendGMCommand( str.str_String );
#endif // VER_TEST
	}

private:
	int m_nItemIndex;
};

class CmdGoNpc : public Command
{
public:
	CmdGoNpc() : m_nNpcIndex(-1)	{}
	CmdGoNpc(int nNpcIndex) : m_nNpcIndex(nNpcIndex) {}
	void setData(int nNpcIndex)	{ m_nNpcIndex = nNpcIndex;	}
	void execute()
	{
#ifdef	VER_TEST
		if (m_nNpcIndex < 0)
			return;

		CTString str;
		str.PrintF("go_npc %d", m_nNpcIndex);
		_pNetwork->SendGMCommand( str.str_String );
#endif // VER_TEST
	}

private:
	int m_nNpcIndex;
};

#endif		// UI_COMMNAD_H_
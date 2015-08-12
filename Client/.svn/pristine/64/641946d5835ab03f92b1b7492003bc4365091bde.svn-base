#include "StdH.h"
#include <Engine/Interface/UIWindow.h>
#include "UIMsgBoxMgr.h"
#include <Engine/Interface/UIManager.h>

#define DEF_FAIL_ID 0

////////////////////// command //////////////////////////////
class CmdMsgEvent : public Command
{
public:
	CmdMsgEvent() : _MsgID(0), _bDestroy(true), _pCmd(NULL) {}
	~CmdMsgEvent() 
	{
		SAFE_DELETE(_pCmd);
	}
	void setData(UINT MsgID, bool bDestroy = true, Command* pCmd = NULL)	
	{ 
		_MsgID = MsgID;
		_bDestroy = bDestroy;
		_pCmd = pCmd;
	}
	void execute() 
	{
		if(_MsgID > 0)
		{
			if (_pCmd != NULL)
				_pCmd->execute();

			MSGBOXMGR()->Close(_MsgID, _bDestroy);
		}
	}
private:
	UINT _MsgID;
	bool _bDestroy;
	Command* _pCmd;
};
//-----------------------------------------------------------

class CmdMsgDelete : public Command
{
public:
	CmdMsgDelete() : _MsgID(0){}
	void setData(UINT MsgID)	{ _MsgID = MsgID; }
	void execute() {
		if(_MsgID > 0)
			MSGBOXMGR()->Delete(_MsgID);
	}
private:
	UINT _MsgID;
};
//-----------------------------------------------------------

MsgBoxMgr::MsgBoxMgr()
	: m_IDGenerator(0)
{
	_map.clear();

	for (int i = 0; i < eMSG_MAX; i++)
	{
		_pMsgDesign[i] = NULL;
	}

	m_csLock.cs_iIndex = -1;
}

MsgBoxMgr::~MsgBoxMgr()
{
	DestroyAll();
}

void MsgBoxMgr::DestroyAll()
{
	int i;
	for (i = 0; i < eMSG_MAX; i++)
	{
		SAFE_DELETE(_pMsgDesign[i]);
	}

	DeleteAll();
}

void MsgBoxMgr::DeleteAll()
{
	mapMsgBox_iter _iter = _map.begin();
	mapMsgBox_iter _eiter = _map.end();

	for (;_iter != _eiter; ++_iter)
	{
		SAFE_DELETE(_iter->second);
	}

	_map.clear();

	m_IDGenerator = 0;
}

void MsgBoxMgr::Initialize()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
		
	_pMsgDesign[eMSG_OK] = new CUIMsgBox;
	pUIMgr->LoadXML("MsgBox.xml", _pMsgDesign[eMSG_OK]);

	_pMsgDesign[eMSG_YESNO] = new CUIMsgBoxYesNo;
	pUIMgr->LoadXML("msgBox_yesno.xml", _pMsgDesign[eMSG_YESNO]);

	_pMsgDesign[eMSG_INPUT] = new CUIMsgBoxInput;
	pUIMgr->LoadXML("msgBox_input.xml", _pMsgDesign[eMSG_INPUT]);

	_pMsgDesign[eMSG_ICON] = new CUIMsgBoxICon;
	pUIMgr->LoadXML("msgBox_icon.xml", _pMsgDesign[eMSG_ICON]);

	_pMsgDesign[eMSG_TIMER] = new CUIMsgBoxTime;
	pUIMgr->LoadXML("msgBox_timer.xml", _pMsgDesign[eMSG_TIMER]);	
}

UINT MsgBoxMgr::CreateMsgBox( const char* strTitle, const char* strContent, eMSG_TYPE eType, bool bDestroy, Command* pCmdOk, Command* pCmdClose)
{
	if (eType < eMSG_OK || eType >= eMSG_MAX)
		return DEF_FAIL_ID;

	CUIManager* pUIManager = CUIManager::getSingleton();	
	
	int nX, nY;

	switch(eType)
	{
	case eMSG_OK:
		{
			CUIMsgBox* pMsgBox = (CUIMsgBox*)_pMsgDesign[eType]->Clone();

			if (pMsgBox != NULL)
			{
				m_IDGenerator++;

				nX = ( pUIManager->GetMaxI() - pUIManager->GetMinI() - pMsgBox->GetWidth() ) / 2;
				nY = ( pUIManager->GetMaxJ() - pUIManager->GetMinJ() - pMsgBox->GetHeight() ) / 5;

				pMsgBox->initialize();
				SetMsgBoxInfo(pMsgBox, m_IDGenerator, nX, nY, strTitle, strContent, pCmdOk, pCmdClose, bDestroy);
				
				_map.insert(std::make_pair(m_IDGenerator, pMsgBox));
			}
		}		
		break;
	case eMSG_YESNO:
		{
			CUIMsgBoxYesNo* pMsgBox = (CUIMsgBoxYesNo*)_pMsgDesign[eType]->Clone();

			if (pMsgBox != NULL)
			{
				m_IDGenerator++;

				nX = ( pUIManager->GetMaxI() - pUIManager->GetMinI() - pMsgBox->GetWidth() ) / 2;
				nY = ( pUIManager->GetMaxJ() - pUIManager->GetMinJ() - pMsgBox->GetHeight() ) / 5;

				pMsgBox->initialize();
				SetMsgBoxInfo((CUIMsgBox*)pMsgBox, m_IDGenerator, nX, nY, strTitle, strContent, pCmdOk, pCmdClose, bDestroy);

				_map.insert(std::make_pair(m_IDGenerator, (CUIMsgBox*)pMsgBox));
			}
		}		
		break;

	case eMSG_INPUT:
	case eMSG_ICON:
	case eMSG_TIMER:
		{
			sMsgBox_Element sElement;
			sElement.eType = eType;
			return CreateMsgBox(strTitle, strContent, sElement);
		}		
		break;
	}	

	if(Open(m_IDGenerator) == false)
	{
		LOG_DEBUG("MsgBoxMgr::CreateMsgBox :: Open fail %d Msg Box", m_IDGenerator );
	}

	return m_IDGenerator;
}

UINT MsgBoxMgr::CreateMsgBox( const char* strTitle, const char* strContent, sMsgBox_Element sElement )
{
	if (sElement.eType < eMSG_OK || sElement.eType >= eMSG_MAX)
		return DEF_FAIL_ID;

	CUIManager* pUIManager = CUIManager::getSingleton();

	int nX, nY;
	switch(sElement.eType)
	{
	case eMSG_OK:		
	case eMSG_YESNO:
		{			
			return CreateMsgBox(strTitle, strContent, sElement.eType);
		}		
		break;
	case eMSG_INPUT:
		{
			CUIMsgBoxInput* pMsgBox = (CUIMsgBoxInput*)_pMsgDesign[sElement.eType]->Clone();
			
			if (pMsgBox != NULL)
			{
				m_IDGenerator++;

				nX = ( pUIManager->GetMaxI() - pUIManager->GetMinI() - pMsgBox->GetWidth() ) / 2;
				nY = ( pUIManager->GetMaxJ() - pUIManager->GetMinJ() - pMsgBox->GetHeight() ) / 5;

				pMsgBox->initialize();
				SetMsgBoxInfo((CUIMsgBox*)pMsgBox, m_IDGenerator, nX, nY, strTitle, strContent);
				pMsgBox->SetInputBox(sElement.maxInputChar);
				_map.insert(std::make_pair(m_IDGenerator, (CUIMsgBox*)pMsgBox));
			}
		}		
		break;
	case eMSG_ICON:
		{
			CUIMsgBoxICon* pMsgBox = (CUIMsgBoxICon*)_pMsgDesign[sElement.eType]->Clone();

			if (pMsgBox != NULL)
			{
				m_IDGenerator++;

				nX = ( pUIManager->GetMaxI() - pUIManager->GetMinI() - pMsgBox->GetWidth() ) / 2;
				nY = ( pUIManager->GetMaxJ() - pUIManager->GetMinJ() - pMsgBox->GetHeight() ) / 5;

				pMsgBox->initialize();
				SetMsgBoxInfo((CUIMsgBox*)pMsgBox, m_IDGenerator, nX, nY, strTitle, strContent);

				_map.insert(std::make_pair(m_IDGenerator, (CUIMsgBox*)pMsgBox));
			}
		}		
		break;
	case eMSG_TIMER:
		{
			CUIMsgBoxTime* pMsgBox = (CUIMsgBoxTime*)_pMsgDesign[sElement.eType]->Clone();

			if (pMsgBox != NULL)
			{
				m_IDGenerator++;

				nX = ( pUIManager->GetMaxI() - pUIManager->GetMinI() - pMsgBox->GetWidth() ) / 2;
				nY = ( pUIManager->GetMaxJ() - pUIManager->GetMinJ() - pMsgBox->GetHeight() ) / 5;

				pMsgBox->initialize();
				SetMsgBoxInfo((CUIMsgBox*)pMsgBox, m_IDGenerator, nX, nY, strTitle, strContent);
				pMsgBox->SetTime(sElement.rmainTime);

				_map.insert(std::make_pair(m_IDGenerator, (CUIMsgBox*)pMsgBox));
			}
		}		
		break;
	}	

	if(Open(m_IDGenerator) == false)
	{
		LOG_DEBUG("MsgBoxMgr::CreateMsgBox :: Open fail %d Msg Box", m_IDGenerator );
	}

	return m_IDGenerator;
}

bool MsgBoxMgr::Open( UINT nID )
{
	if (nID == DEF_FAIL_ID || nID > m_IDGenerator)
		return false;

	mapMsgBox_iter _iter = _map.find(nID);
	
	if (_iter != _map.end())
	{
		_iter->second->Open();
		return true;
	}

	LOG_DEBUG("MsgBoxMgr::Open() :: Not find Msg ID : %d ", nID);
	return false;
}

bool MsgBoxMgr::Close( UINT nID, bool bDestroy /*= true*/ )
{
	if (nID == DEF_FAIL_ID || nID > m_IDGenerator)
		return false;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{
		_iter->second->Close();

		if (bDestroy == true)
		{
			CmdMsgDelete* pCmd = new CmdMsgDelete;
			pCmd->setData(nID);
			addQueue(pCmd);
		}
		return true;
	}

	LOG_DEBUG("MsgBoxMgr::Close() :: Not find");
	return false;
}

void MsgBoxMgr::Delete( UINT nID )
{
	if (nID == DEF_FAIL_ID || nID > m_IDGenerator)
		return;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{
		SAFE_DELETE(_iter->second);
		_map.erase(nID);
		LOG_DEBUG("MsgBoxMgr::Delete() :: Success");
		return;
	}

	LOG_DEBUG("MsgBoxMgr::Delete() :: Not find");
	return;
}


BOOL MsgBoxMgr::IsExist( UINT nID )
{
	if (nID == DEF_FAIL_ID || nID > m_IDGenerator)
		return FALSE;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{		
		return _iter->second->IsVisible();
	}
	return FALSE;
}

void MsgBoxMgr::Render( CDrawPort* pDraw )
{
	int Count = _map.size();
	mapMsgBox_iter _iter = _map.begin();

	for (int i = 0; i < Count; i++, _iter++)
	{
		if (_iter != _map.end())
			_iter->second->Render(pDraw);
	}
}

WMSG_RESULT MsgBoxMgr::MsgProc(MSG* pMsg)
{
	int Count = _map.size();
	mapMsgBox_reiter _iter = _map.rbegin();

	for (int i = 0; i < Count; i++, _iter++)
	{
		if (_iter != _map.rend())
		{
			if (_iter->second->MouseMessage(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
		}
	}

	return WMSG_FAIL;
}


WMSG_RESULT MsgBoxMgr::IMEProc( MSG* pMsg )
{
	int Count = _map.size();
	mapMsgBox_reiter _iter = _map.rbegin();
	
	for (int i = 0; i < Count; i++, _iter++)
	{
	    if (_iter != _map.rend())
	    {
	        if (_iter->second->IMEMessageProc(pMsg) != WMSG_FAIL)
	            return WMSG_SUCCESS;
	    }
	}
	return WMSG_FAIL;
}


WMSG_RESULT MsgBoxMgr::CharProc( MSG* pMsg )
{
	int Count = _map.size();
	mapMsgBox_reiter _iter = _map.rbegin();

	for (int i = 0; i < Count; i++, _iter++)
	{
		if (_iter != _map.rend())
		{
			if (_iter->second->CharMessageProc(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
		}
	}
	return WMSG_FAIL;
}

WMSG_RESULT MsgBoxMgr::KeyProc( MSG* pMsg )
{
	int Count = _map.size();
	mapMsgBox_reiter _iter = _map.rbegin();

	for (int i = 0; i < Count; i++, _iter++)
	{
		if (_iter != _map.rend())
		{
			if (_iter->second->KeyMessageProc(pMsg) != WMSG_FAIL)
				return WMSG_SUCCESS;
		}
	}
	return WMSG_FAIL;
}

void MsgBoxMgr::Update(float fDeltaTime, ULONG ElapsedTime)
{
	{
		int Count = _map.size();
		mapMsgBox_reiter _iter = _map.rbegin();

		for (int i = 0; i < Count; i++, _iter++)
		{
			if (_iter != _map.rend())
			{
				_iter->second->Update(fDeltaTime, ElapsedTime);
			}
		}
	}
	
	{
		CTSingleLock sl(&m_csLock, TRUE);

		while (m_queue.size())
		{
			Command* pCmd = m_queue.front();
			if (pCmd != NULL)
				pCmd->execute();

			SAFE_DELETE(pCmd);

			m_queue.pop();
		}
	}
}

void MsgBoxMgr::addQueue( Command* pCmd )
{
	CTSingleLock sl(&m_csLock, TRUE);

	m_queue.push(pCmd);
}

void MsgBoxMgr::SetMsgBoxInfo( CUIMsgBox* pMsgBox, UINT nID, int x, int y, const char* strTitle, const char* strContent, Command* pCmdOk /*= NULL*/, Command* pCmdClose /*= NULL*/, bool bDestroy /*= TRUE*/ )
{
	if (pMsgBox == NULL)
		return;

	pMsgBox->SetPos( x, y );
	pMsgBox->SetTitle(strTitle);
	pMsgBox->SetString(strContent);

	CmdMsgEvent* pCmdOkEvent = new CmdMsgEvent;
	pCmdOkEvent->setData(nID, bDestroy, pCmdOk);
	pMsgBox->SetOkCommand(pCmdOkEvent);

	CmdMsgEvent* pCmdCloseEvent = new CmdMsgEvent;
	pCmdCloseEvent->setData(nID, bDestroy, pCmdClose);
	pMsgBox->SetCloseCommand(pCmdCloseEvent);

	CmdMsgEvent* pCmdNoEvent = new CmdMsgEvent;
	pCmdNoEvent->setData(nID, bDestroy, pCmdClose);
	pMsgBox->SetNoCommand(pCmdNoEvent);
}

void MsgBoxMgr::SetCommandOK( UINT nID, Command* pCmd, bool bDestroy )
{
	if ( IsExist(nID) == FALSE)
		return;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{
		CmdMsgEvent* pCmdOkEvent = new CmdMsgEvent;
		pCmdOkEvent->setData(nID, bDestroy, pCmd);
		_iter->second->SetOkCommand(pCmdOkEvent);
	}
}

void MsgBoxMgr::SetCommandNo( UINT nID, Command* pCmd, bool bDestroy )
{
	if ( IsExist(nID) == FALSE)
		return;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{
		CmdMsgEvent* pCmdCloseEvent = new CmdMsgEvent;
		pCmdCloseEvent->setData(nID, bDestroy, pCmd);
		_iter->second->SetNoCommand(pCmdCloseEvent);
	}
}

void MsgBoxMgr::SetCommandClose( UINT nID, Command* pCmd, bool bDestroy )
{
	if ( IsExist(nID) == FALSE)
		return;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{
		CmdMsgEvent* pCmdCloseEvent = new CmdMsgEvent;
		pCmdCloseEvent->setData(nID, bDestroy, pCmd);
		_iter->second->SetCloseCommand(pCmdCloseEvent);
	}
}

void MsgBoxMgr::SetMsgBoxPos( UINT nID, int x, int y )
{
	if ( IsExist(nID) == FALSE)
		return;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{
		_iter->second->SetPos(x, y);
	}
}

CUIMsgBox* MsgBoxMgr::GetMsgBoxPtr( UINT nID )
{
	if ( IsExist(nID) == FALSE)
		return NULL;

	mapMsgBox_iter _iter = _map.find(nID);

	if (_iter != _map.end())
	{
		return _iter->second;
	}

	return NULL;
}

BOOL MsgBoxMgr::IsEditBoxFocused()
{
	int Count = _map.size();
	mapMsgBox_reiter _iter = _map.rbegin();

	for (int i = 0; i < Count; i++, _iter++)
	{
		if (_iter != _map.rend())
		{
			return _iter->second->IsEditBoxFocused();
		}
	}
	return FALSE;
}

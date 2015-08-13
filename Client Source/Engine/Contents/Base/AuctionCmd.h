#ifndef		_AUCTION_COMMAND_H_
#define		_AUCTION_COMMAND_H_

class Command;
//--------------------------------------------------------------
class CmdSearch : public Command
{
public:
	CmdSearch() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)	{ pUI_ = pUI; }
	void execute()	{
		if (pUI_)
			pUI_->CanSearch();
	}
private:
	CUIAuctionNew*	pUI_;
};
//--------------------------------------------------------------
class CmdBuyItem : public Command
{
public:
	CmdBuyItem() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)	{ pUI_ = pUI; }
	void execute()	{
		if (pUI_)
			pUI_->CanBuyItem();
	}
private:
	CUIAuctionNew*	pUI_;
};
//--------------------------------------------------------------
class CmdRegFavorite : public Command
{
public:
	CmdRegFavorite() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI, bool bReg)	{ pUI_ = pUI; bReg_ = bReg; }
	void execute()	{
		if (pUI_)
			pUI_->showMsgFavorite(bReg_);
	}

private:
	bool			bReg_;
	CUIAuctionNew*	pUI_;
};
//--------------------------------------------------------------
class CmdChangePage : public Command
{
public:
	CmdChangePage() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI, bool bNext)	{ pUI_ = pUI; bNext_ = bNext; }
	void execute()	{
		if (pUI_)
			pUI_->ChangePage(bNext_);
	}

private:
	bool			bNext_;
	CUIAuctionNew*	pUI_;
};
//--------------------------------------------------------------
class CmdAuctionClose : public Command
{
public:
	CmdAuctionClose() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->CloseAuction();
	}
private:
	CUIAuctionNew* pUI_;
};
//--------------------------------------------------------------
class CmdRegist : public Command
{
public:
	CmdRegist() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)	{ pUI_ = pUI; }
	void execute()	{
		if (pUI_)
			pUI_->CheckRegist();
	}
private:
	CUIAuctionNew*	pUI_;
};
//--------------------------------------------------------------
class CmdRegistCancel : public Command
{
public:
	CmdRegistCancel() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)	{ pUI_ = pUI; }
	void execute()	{
		if (pUI_)
			pUI_->showMsgRegCancel();
	}
private:
	CUIAuctionNew*	pUI_;
};
//--------------------------------------------------------------
class CmdCheckItemCount : public Command
{
public:
	CmdCheckItemCount() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->checkItemCount();
	}
private:
	CUIAuctionNew* pUI_;
};
//--------------------------------------------------------------
class CmdCheckPrice : public Command
{
public:
	CmdCheckPrice() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->checkPrice();
	}
private:
	CUIAuctionNew* pUI_;
};
//--------------------------------------------------------------
class CmdChangeMainTab : public Command
{
public:
	CmdChangeMainTab() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->changeMainTab();
	}
private:
	CUIAuctionNew* pUI_;
};
//--------------------------------------------------------------
class CmdChangeSearchTab : public Command
{
public:
	CmdChangeSearchTab() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->changeSearchnTab();
	}
private:
	CUIAuctionNew* pUI_;
};
//--------------------------------------------------------------
class CmdSelectListSearch : public Command
{
public:
	CmdSelectListSearch() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelListSearch();
	}
private:
	CUIAuctionNew* pUI_;
};
//--------------------------------------------------------------
class CmdSelectListReg : public Command
{
public:
	CmdSelectListReg() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelListRegist();
	}
private:
	CUIAuctionNew* pUI_;
};
//--------------------------------------------------------------
class CmdSelectListFav : public Command
{
public:
	CmdSelectListFav() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelListFavorite();
	}
private:
	CUIAuctionNew* pUI_;
}; 
//--------------------------------------------------------------

class CmdDblClickList : public Command
{
public:
	CmdDblClickList() : pUI_(NULL) {}
	void setData(CUIAuctionNew* pUI)		{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->CanBuyItem();
	}
private:
	CUIAuctionNew* pUI_;
};
#endif		// _AUCTION_COMMAND_H_
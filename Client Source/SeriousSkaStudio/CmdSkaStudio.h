#ifndef		CMD_SKA_STUDIO_H_
#define		CMD_SKA_STUDIO_H_

#include <Engine/Base/Command.h>

//------------------------------------------------------------------------

class CmdSkaSelectParticleEffect : public Command
{
public:
	void setData(WPARAM wp, LPARAM lp)	{ wp_ = wp; lp_ = lp; }
	void execute();

private:
	WPARAM wp_;
	LPARAM lp_;
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaTestEffect : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaMainFrmCreateTexture : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaNewFile : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------

class CmdSkaOpenFile : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------

class CmdSkaImportComvert : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaComvertRecursive : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaComvertTexRecursive : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaEffectFileOpen : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaEffectFileOpenAsAdd : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaEffectFileSave : public Command
{
public:
	void setData(const char* path)	{ strPath_ = path; }
	void execute();

private:
	std::string		strPath_;
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaExportTexture : public Command
{
public:
	void setData(const char* path)	{ strPath_ = path; }
	void execute();

private:
	std::string		strPath_;
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaExportEffectInfo : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdExecuteEffect : public Command
{
public:
	void setData(BOOL bResult)	{ bAutoSpell_ = bResult; }
	void execute();

private:
	BOOL		bAutoSpell_;
};

//------------------------------------------------------------------------

//------------------------------------------------------------------------

class CmdSkaCreateItemSmcData : public Command
{
public:
	void execute();
};

//------------------------------------------------------------------------


#endif		// CMD_SKA_STUDIO_H_
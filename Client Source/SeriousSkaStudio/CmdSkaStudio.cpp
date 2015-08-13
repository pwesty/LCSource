#include "stdafx.h"
#include "CmdSkaStudio.h"
#include "SeriousSkaStudio.h"
#include "MainFrm.h"


//------------------------------------------------------------------------

void CmdSkaSelectParticleEffect::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{

		pMain->m_dlgCEffectProperty.m_dlgCParticleEffectProperty.SelectEffect( wp_, lp_ );
	}
}

//------------------------------------------------------------------------

void CmdSkaTestEffect::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{		
		pMain->m_dlgEffectEditType.TestEffectSystem();
	}
}

void CmdSkaMainFrmCreateTexture::execute()
{
	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{		
		pMain->CreateTexture();
	}
}

void CmdSkaNewFile::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->FileNew();
	}
}

void CmdSkaOpenFile::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->FileOpen();
	}
}

void CmdSkaImportComvert::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ImportConvert();
	}
}

void CmdSkaComvertRecursive::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ConvertRecursive();
	}
}

void CmdSkaComvertTexRecursive::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ConvertTextureRecursive();
	}
}

void CmdSkaEffectFileOpen::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->EffectOpen();
	}
}

void CmdSkaEffectFileOpenAsAdd::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->EffectOpenAsAdd();
	}
}

void CmdSkaEffectFileSave::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->SaveEffect( strPath_.c_str() );
	}
}

void CmdSkaExportTexture::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ExportTexture( strPath_.c_str() );
	}
}

void CmdSkaExportEffectInfo::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ExportEffectInfo();
	}
}

void CmdExecuteEffect::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->ExecuteEffect( bAutoSpell_ );
	}
}

void CmdSkaCreateItemSmcData::execute()
{
	if ( CSeriousSkaStudioApp* pMain = (CSeriousSkaStudioApp *)AfxGetApp())
	{
		pMain->CreateItemSmcData();
	}
}

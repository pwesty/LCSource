// ----------------------------------------------------------------------------
//  File : JobInfo.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------
#ifndef	_LOGIN_JOBINFO_H_
#define	_LOGIN_JOBINFO_H_

#include <Engine/Base/CTString.h>
#include <Engine/Base/FileName.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/JobInfo.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/DesignClasses/Singleton/SingletonBase.h>
	
class ENGINE_API CLoginJobInfo : public CSingletonBase< CLoginJobInfo >
{
private:
	typedef struct _tagLoginJobInfo
	{
		CTString		strFileName;		// 파일명.
		CEntity*			pLogin_MI;			// 로그인 모델

		_tagLoginJobInfo()
		{
			pLogin_MI = NULL;
		}

	}sLoginJobInfo;

public:
	CLoginJobInfo();
	~CLoginJobInfo();

	void		InitLoginCharacterInfo();
	void		InitLoginModelInit();
	void		LoginModelCreate();
	void		LoginModelDelete();
	// Get Properties
	CTString	GetFileName( eJob Job );
	CEntity*	GetLoginModel( eJob Job);

protected:
	sLoginJobInfo		*m_pLoginJobInfo;
};

#endif // _LOGIN_JOBINFO_H_
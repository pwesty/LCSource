#ifndef _LOADING_IMAGE_H_
#define _LOADING_IMAGE_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <Engine/Interface/UIBase.h>

class CUIImage;
class CUITextBox;
class ENGINE_API CLoadingImage : public CUIBase, public CSingletonBase< CLoadingImage >
{
public:
	CLoadingImage();
	~CLoadingImage();

	enum ePROGRESS_STATE
	{
		ePS_START = 0,
		ePS_WARKING,
		ePS_END
	};

	void initialize();
	void InitLoadingData();

	void StopLoading();
	void SetProgress(float f);
	void UpdateProgress();
	ePROGRESS_STATE GetLoadingState()	{ return m_eState;	}
private:

	// 랜덤하게 로딩 팁을 가져오도록.
	CTString GetTip();

	std::vector<int> m_vecTip;
	CUIImage* m_pImgZone;
	CUIImage* m_pImgZoneName;
	CUIImage* m_pImgLoadingBar;
	CUITextBox* m_pTBTip;

	ePROGRESS_STATE m_eState;

	float	m_fLoadingStepMax;
	float	m_fLoadingUniStep;
	float	m_fStepCompleted;
	int		m_nCurStep;
	int		m_nOriginWidth;

	CTimerValue	m_tvLastUpdate;
};

#define SPLASH()	CLoadingImage::getSingleton()
#define CallProgressHook_t(x)	SPLASH()->SetProgress(x)

#endif // _LOADING_IMAGE_H_
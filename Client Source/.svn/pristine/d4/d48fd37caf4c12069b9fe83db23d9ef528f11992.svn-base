#pragma once

#ifndef __SMCINFO_H
#define __SMCINFO_H

#pragma warning(disable:4786)

#include <windows.h>
#include <vector>
#include <list>
#include <assert.h>

class CMeshInfo
{
public:
	typedef struct _TEXTURES
	{
		char*	_sTexName;	// 텍스처 이름
		char*	_sTexTFNM;	// 텍스처의 파일 경로

		_TEXTURES()
		{
			_sTexName = NULL;	_sTexTFNM = NULL;
		}

		_TEXTURES(const _TEXTURES& Tex)
		{
			(*this) = Tex;
		}

		~_TEXTURES()
		{
			if (_sTexName != NULL) { delete [] _sTexName; }
			if (_sTexTFNM != NULL) { delete [] _sTexTFNM; }
		}

		void operator = (const _TEXTURES& Tex)
		{
			if (Tex._sTexName)
			{
				_sTexName = new char[strlen(Tex._sTexName)+1];
				strcpy(_sTexName, Tex._sTexName);
			}

			if (Tex._sTexTFNM)
			{
				_sTexTFNM = new char[strlen(Tex._sTexTFNM)+1];
				strcpy(_sTexTFNM, Tex._sTexTFNM);
			}
		}
	}TEXTUREINFO;

	CMeshInfo() { Clear(); }
	CMeshInfo(const CMeshInfo& Mesh);
	virtual ~CMeshInfo() { Destroy(); }

	void operator = (const CMeshInfo& Mesh);

	void Clear();		// 변수 초기화
	void Destroy();		// 할당된 변수 초기화

	inline void SetMeshNumber(int num)
	{
		m_MeshNumber = num;
	}

	inline bool CreateMeshTFNM(const char* strTFNM)
	{
		m_MeshTFNM = new char[strlen(strTFNM)+1];
		if (!m_MeshTFNM)
			return false;
		strcpy(m_MeshTFNM, strTFNM);
		return true;
	}

	inline int GetMeshNumber() { return m_MeshNumber; }
	inline int GetTexInfoSize() { return m_vecTextures.size(); }
	inline const char* GetMeshTFNM() { return (const char*)m_MeshTFNM; }
	inline const char* GetTexInfoName(int iIdx) { return (const char*)m_vecTextures[iIdx]._sTexName; }
	inline const char* GetTexInfoTFNM(int iIdx) { return (const char*)m_vecTextures[iIdx]._sTexTFNM; }
	inline void AddTextureInfo(TEXTUREINFO TexInfo) { m_vecTextures.push_back(TexInfo); }

	bool AddTextureInfo(const char* strTexName, const char* strTexTFNM);
	
protected:
	int		m_MeshNumber; // 현재 smc파일의 메쉬 번호
	char*	m_MeshTFNM;	// 메쉬의 bm파일 경로
	
	std::vector<TEXTUREINFO>	m_vecTextures;	// 텍스처 정보 목록
};

typedef std::vector<CMeshInfo>::iterator	vecMeshInfoItor;
typedef std::vector<CMeshInfo>	vecMeshInfo;

class CSmcInfo
{
public:
	CSmcInfo() { Clear(); }
	CSmcInfo(const CSmcInfo& SmcInfo);
	virtual ~CSmcInfo() { Destroy(); }

	void operator = (const CSmcInfo& SmcInfo);

	void Clear();
	void Destroy();

	inline bool SetSmcFileName(const char* strName)
	{
		m_SmcFileName = new char[strlen(strName)+1];
		if (!m_SmcFileName)
			return false;
		strcpy(m_SmcFileName, strName);
		return true;
	}

	inline const char* GetSmcFileName() { return (const char*)m_SmcFileName; }
	inline void AddMeshInfo(CMeshInfo Mesh) { m_MeshInfoList.push_back(Mesh); }
	inline int GetMeshInfoListSize() { return m_MeshInfoList.size(); }
	inline CMeshInfo GetMeshInfo(int num) { return m_MeshInfoList[num]; }

	bool SaveSmcInfoText(std::string sFilename);	// Text파일로 저장

protected:
	char*	m_SmcFileName;		// smc 파일 이름
	vecMeshInfo	m_MeshInfoList; // smc파일 내의 메쉬 정보리스트
};

#endif
#include "stdh.h"
#include <string>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <Engine/Entities/SmcInfo.h>

using namespace std;

CMeshInfo::CMeshInfo(const CMeshInfo& Mesh)
{
	Clear();

	m_MeshNumber = Mesh.m_MeshNumber;

	m_MeshTFNM = new char[strlen(Mesh.m_MeshTFNM)+1];
	strcpy(m_MeshTFNM, Mesh.m_MeshTFNM);

	if (!Mesh.m_vecTextures.empty())
	{
		int loop = Mesh.m_vecTextures.size();
		int i;

		for(i=0; i<loop; i++)
		{
			m_vecTextures.push_back(TEXTUREINFO());
			m_vecTextures[i] = Mesh.m_vecTextures[i];
		}
	}
}

void CMeshInfo::Clear()
{
	m_MeshNumber = 0;
	m_MeshTFNM = NULL;
	m_vecTextures.clear();
}

void CMeshInfo::Destroy()
{
	if (m_MeshTFNM)
	{
		delete [] m_MeshTFNM;
		m_MeshTFNM = 0;
	}

	Clear();
}

void CMeshInfo::operator = (const CMeshInfo& Mesh)
{
	Destroy();

	m_MeshNumber = Mesh.m_MeshNumber;

	m_MeshTFNM = new char[strlen(Mesh.m_MeshTFNM)+1];
	strcpy(m_MeshTFNM, Mesh.m_MeshTFNM);

	if (!Mesh.m_vecTextures.empty())
	{
		int loop = Mesh.m_vecTextures.size();
		int i;

		for(i=0; i<loop; i++)
		{
			m_vecTextures.push_back(TEXTUREINFO());
			m_vecTextures[i] = Mesh.m_vecTextures[i];
		}
	}
}

bool CMeshInfo::AddTextureInfo(const char* strTexName, const char* strTexTFNM)
{
	TEXTUREINFO TexInfo;
	
	TexInfo._sTexName = new char[strlen(strTexName)+1];
	if (!TexInfo._sTexName)
		return false;
	TexInfo._sTexTFNM = new char[strlen(strTexTFNM)+1];
	if (!TexInfo._sTexTFNM)
	{
		delete [] TexInfo._sTexName;
		TexInfo._sTexName = NULL;
		return false;
	}

	strcpy(TexInfo._sTexName, strTexName);
	strcpy(TexInfo._sTexTFNM, strTexTFNM);

	TEXTUREINFO TexInfo2 = TexInfo;

	m_vecTextures.push_back(TexInfo);

	return true;
}

CSmcInfo::CSmcInfo(const CSmcInfo& SmcInfo)
{
	Clear();

	m_SmcFileName = new char[strlen(SmcInfo.m_SmcFileName)+1];
	strcpy(m_SmcFileName, SmcInfo.m_SmcFileName);

	if (!SmcInfo.m_MeshInfoList.empty())
	{
		int loop = SmcInfo.m_MeshInfoList.size();
		int i;

		for(i=0; i<loop; i++)
		{
			m_MeshInfoList.push_back(CMeshInfo());
			m_MeshInfoList[i] = SmcInfo.m_MeshInfoList[i];
		}
	}
}

void CSmcInfo::operator = (const CSmcInfo& SmcInfo)
{
	Destroy();

	if (SmcInfo.m_SmcFileName == NULL) return;

	m_SmcFileName = new char[strlen(SmcInfo.m_SmcFileName)+1];
	strcpy(m_SmcFileName, SmcInfo.m_SmcFileName);

	if (!SmcInfo.m_MeshInfoList.empty())
	{
		int loop = SmcInfo.m_MeshInfoList.size();
		int i;

		for(i=0; i<loop; i++)
		{
			m_MeshInfoList.push_back(SmcInfo.m_MeshInfoList[i]);
		}
	}
}

void CSmcInfo::Clear()
{
	m_SmcFileName = NULL;
	m_MeshInfoList.clear();
}

void CSmcInfo::Destroy()
{
	if (m_SmcFileName)
		delete [] m_SmcFileName;

	Clear();
}

bool CSmcInfo::SaveSmcInfoText(std::string sFilename)
{
	ofstream fout;
	int MeshInfoSize = m_MeshInfoList.size();
	int TexInfoSize = 0;
	int i, j;

	fout.open(sFilename.c_str(), ios_base::out);//|ios_base::app);

	fout << "SMC Name: " << m_SmcFileName << '\n';
	fout << "MESH Total NUM: " << MeshInfoSize << '\n'; // 메쉬 갯수

	for (i=0; i<MeshInfoSize; i++)
	{ // 메쉬 정보 저장
		fout << "Mesh Number: " << m_MeshInfoList[i].GetMeshNumber() << '\n';	// 메쉬 번호
		fout << "Mesh Path: " << m_MeshInfoList[i].GetMeshTFNM() << '\n';	// 메쉬 파일 경로
		TexInfoSize = m_MeshInfoList[i].GetTexInfoSize();
		fout << "Tex Total NUM: " << TexInfoSize << '\n'; // 텍스처 갯수

		for (j=0; j<TexInfoSize; j++)
		{
			fout << "Tex Name: " << m_MeshInfoList[i].GetTexInfoName(j) << '\n'; // Tex Name
			fout << "Tex File Path: " << m_MeshInfoList[i].GetTexInfoTFNM(j) << '\n'; // Tex FilePath
		}
	}

	fout << '\n'; // 끝

	fout.close();

	return true;
}

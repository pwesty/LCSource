#include "stdh.h"
#include <fstream>
#include <tchar.h>
#include <Engine/Base/FileName.h>
#include <Engine/Entities/SmcParser.h>
#include <Engine/Templates/StaticArray.cpp>

using namespace std;

#define MAX_LINE_LENGTH 1024
//#define WRITING_LOGINFO // 파싱 로그 정보 

extern CTFileName _fnmApplicationPath;

const char* g_pszOffSet = "OFFSET";
const char* g_pszName = "NAME";
const char* g_pszMesh = "MESH";
const char* g_pszTextures = "TEXTURES";
const char* g_pszAllFramesBBox = "ALLFRAMESBBOX";
const char* g_pszColision = "COLISION";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ByteQueueData::ByteQueueData()
{

}

ByteQueueData::~ByteQueueData()
{

}
//////////////////////////////////////////////////////////////////////
// Data Read / Write Function
//////////////////////////////////////////////////////////////////////
void ByteQueueData::WriteString(std::string sData)
{
	int i;
	size_t size = sData.length();
	WriteWord(sData.length());

	for (i=0; i<sData.length(); ++i)
		push(sData[i]);
}

void ByteQueueData::WriteDouble(double dData)
{
	BYTE *pByte = (BYTE*)&dData;
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
}

void ByteQueueData::WriteFloat(float floatData)
{
	BYTE *pByte = (BYTE*)&floatData;
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
}

void ByteQueueData::WriteDword(DWORD dwData)
{
	BYTE *pByte = (BYTE*)&dwData;
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
	push(*pByte++);
}

void ByteQueueData::WriteWord(WORD wData)
{
	BYTE *pByte = (BYTE*)&wData;
	push(*pByte++);
	push(*pByte++);
}

void ByteQueueData::WriteByte(BYTE byteData)
{
	push(byteData);
}

void ByteQueueData::WriteDocString(std::string sData)
{
	for (int i=0; i<sData.length(); ++i)
		push(sData[i]);
}

std::string ByteQueueData::ReadString()
{
	int i;
	char *pszData;
	WORD wSize = ReadWord();
	pszData = new char[(int)wSize+1];

	for (i=0; i<(int)wSize; ++i)
	{
		pszData[i] = front();	pop();
	}

	pszData[(int)wSize] = NULL;
	std::string sData(pszData);
	delete [] pszData;
	return sData;
}

double ByteQueueData::ReadDouble()
{
	double dData;
	PBYTE p = (PBYTE)&dData;
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	return dData;
}

float ByteQueueData::ReadFloat()
{
	float floatData;
	PBYTE p = (PBYTE)&floatData;
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	return floatData;
}

DWORD ByteQueueData::ReadDword()
{
	DWORD dwData;
	PBYTE p = (PBYTE)&dwData;
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	*p++ = front(); pop();
	return dwData;
}

WORD ByteQueueData::ReadWord()
{
	WORD wData;
	PBYTE p = (PBYTE)&wData;
	*p++ = front(); pop();
	*p++ = front(); pop();
	return wData;
}

BYTE ByteQueueData::ReadByte()
{
	BYTE byteData = front(); pop();
	return byteData;
}

void ByteQueueData::ClearQueue()
{
	while (!empty()) {	pop();	}
}

//////////////////////////////////////////////////////////////////////////

bool CSmcParser::LoadSmcParse(std::string sFilename)
{
	ifstream File;

	File.open(sFilename.c_str());
	if(!File.good())
		return HandleError("ERROR: file %s can't be opened for parsing.\n",(const char*)sFilename.c_str());

	if(!ProcessParse(File))
		return HandleError("Could not parse file %s.\n", (const char*)sFilename.c_str());

	return true;
}

bool CSmcParser::ProcessParse(std::istream& Input)
{
	char cLineBuffer[MAX_LINE_LENGTH];
	std::string sLineBuffer;

	if(!Input.good())
		return HandleError("ERROR: file can't be opened for parsing.(ProcessParse)");

	while (Input.good())
	{
		Input.getline(cLineBuffer, MAX_LINE_LENGTH);
		sLineBuffer = cLineBuffer;
		if (!ParseLine(sLineBuffer))
			return HandleError("ERROR: this line Fail of Parsing!");

		if (m_ParserType == NAME_END)
		{
			m_ParserType = PARSER_END;	
			return true; // Smc파일 정보를 모두 가져왔다.
		}
	}

	return true;
}

bool CSmcParser::ParseLine(const std::string& sLine)
{
	char cTempBuffer1[MAX_LINE_LENGTH];
	char cTempBuffer2[MAX_LINE_LENGTH];
	int iCurrentPosition = 0;
	int iStartPos = 0;
	int iEndPos = 0;
	int iNumToCopy = 0;

	if (m_ParserType == PARSER_NONE)
	{
		std::string strOffset = g_pszOffSet;
		iStartPos = sLine.find(strOffset);

		if (iStartPos == std::string::npos)
		{ // "OFFSET"을 못찾았다. // 이것은 OFFSET 정보가 없다.
			m_ParserType = PARSER_NAME;
		}
	}

	switch (m_ParserType)
	{
	case PARSER_NONE:
		{
			string strOffset = g_pszOffSet;
			iStartPos = sLine.find(strOffset);

			if (iStartPos != string::npos)
			{ // "OFFSET"을 찾았다. // OFFSET 정보는 저장 하지 않는다.
				m_ParserType = PARSER_NAME;
			}
			else
			{
				return false;
			}
		}
		break;
	case PARSER_NAME:
		{ // SMC 파일 이름
			iStartPos = sLine.find('\"')+1;
			iEndPos = sLine.find('\"', iStartPos);
			iNumToCopy = iEndPos - iStartPos;
			sLine.copy(cTempBuffer1, iNumToCopy, iStartPos);
			cTempBuffer1[iNumToCopy] = NULL; // SMC 파일 이름 = cTempBuffer
			SetSmcFileName(cTempBuffer1); // SMC파일 이름 저장
			m_ParserType = NAME_START;
		}
		break;
	case NAME_START:
			m_ParserType = MESH;
		break;
	case MESH:
		{ // 메쉬 정보 (bm 파일 경로)
			string strAllFramesBBox = g_pszAllFramesBBox;
			iStartPos = sLine.find(strAllFramesBBox);
			if (iStartPos != string::npos) // ALLFRAMESBBOX정보는 필요없다.
			{
				m_ParserType = COLISION;
				return true;
			}

			std::string strMesh = g_pszMesh;
			iStartPos = sLine.find(g_pszMesh);
			if (iStartPos == std::string::npos) // MESH정보가 아니다. 그외의 사항 예) SKELETON, ANIMSET 장비정보에서는 필요 없는 부분이다. 나중에 필요하게되면 읽자
				return true;

			// 메쉬정보 시작
			iStartPos = sLine.find('\"')+1;
			iEndPos = sLine.find('\"', iStartPos);
			iNumToCopy = iEndPos - iStartPos;
			sLine.copy(cTempBuffer1, iNumToCopy, iStartPos);
			cTempBuffer1[iNumToCopy] = NULL; // 메쉬 파일 경로 = cTempBuffer
			m_TempMeshInfo.CreateMeshTFNM(cTempBuffer1);	// 메쉬 파일 경로 저장
			m_TempMeshInfo.SetMeshNumber(++m_iMeshCount); // 메쉬 번호
			// 다음은 텍스처 정보
			m_ParserType = TEXTURES;
		}
		break;
	case TEXTURES:
		{
			string strTextures = g_pszTextures;
			iStartPos = sLine.find(strTextures);
			if (iStartPos == string::npos)
			{ // 텍스처 정보를 못찾았다.
				m_ParserType = MESH;
				return HandleError("ERROR: Not Find Texture Info!");
			}
			m_ParserType = TEXTURES_START;
		}
		break;
	case TEXTURES_START:
		{
			OutputDebugString("Finding Textrues Info!\n");
			
			iStartPos = sLine.find('{');
			if (iStartPos != string::npos)
				m_ParserType = TEXTURES_NAME;
			else
			{
				m_ParserType = MESH;
				return true;
			}
		}
		break;
	case TEXTURES_NAME:
		{
			iStartPos = sLine.find('}');
			if (iStartPos != string::npos)
			{ // 메쉬 정보 하나를 저장
				AddMeshInfo(m_TempMeshInfo);
				m_TempMeshInfo.Destroy();
				m_ParserType = MESH; // 메쉬정보가 더 있는지 확인 후 추가
				return true;
			}

			iStartPos = sLine.find('\"')+1;
			iEndPos = sLine.find('\"', iStartPos);
			iNumToCopy = iEndPos - iStartPos;
			sLine.copy(cTempBuffer1, iNumToCopy, iStartPos);
			cTempBuffer1[iNumToCopy] = NULL;	// 텍스처 이름

			iStartPos = sLine.find('\"', iEndPos+1)+1;
			iEndPos = sLine.find('\"', iStartPos);
			iNumToCopy = iEndPos - iStartPos;
			sLine.copy(cTempBuffer2, iNumToCopy, iStartPos);
			cTempBuffer2[iNumToCopy] = NULL;	// 텍스처 파일 경로

			m_TempMeshInfo.AddTextureInfo(cTempBuffer1, cTempBuffer2);
		}
		break;
	case COLISION:
		{
			m_ParserType = COLISION_START;
		}
	case COLISION_START:
		{
			iEndPos = sLine.find('}');
			if (iEndPos != string::npos)
			{
				m_ParserType = NAME_END;
				return true;
			}
		}
	}

	return true;
}

bool CSmcParser::HandleError(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	char szBuffer[512];
	if( lpszFormat )
	{
		nBuf = _vsntprintf(szBuffer, 512, lpszFormat, args);
		assert(nBuf >= 0);
	}
	else
		szBuffer[0] = NULL;
	OutputDebugString(szBuffer);
	m_sErrorMessage = szBuffer;

#ifdef WRITING_LOGINFO
	if (m_pStrloglist)
		m_pStrloglist->push_back(m_sErrorMessage);
#endif

	va_end(args);
	return false;	
}

bool CSmcParserList::SmcInfoWriteBin(std::ostream& Output)
{
	// clear the queue
	m_queBuffer.ClearQueue();

	// 여기서 저장할 데이터를 Queue에 담는다.
	WriteSmcInfo();
	
	while(!m_queBuffer.empty())
	{
		Output.put(m_queBuffer.front());
		m_queBuffer.pop();
	}

	Output.flush(); // 파일에 데이터 기록
	return true;
}

bool CSmcParserList::SmcInfoWriteBin(std::string sFilename)
{
	ofstream File;

	File.open(sFilename.c_str(), ios::out | ios::binary | ios::trunc);

	if(!File.good())
		return false; // 에러

	return SmcInfoWriteBin(File);
}

bool CSmcParserList::SmcParsingLogWritetxt(std::string sFilename)
{
	ofstream fout;
	std::vector<std::string>::iterator strItor;

	fout.open(sFilename.c_str(), ios_base::out | ios_base::trunc);//|ios_base::app);

	if (!fout.good())
		return false;

	for (strItor=m_strLogList.begin(); strItor!=m_strLogList.end(); strItor++)
	{
		std::string StrTemp = (*strItor);

		fout << StrTemp;
	}

	fout << '\n'; // 끝

	fout.close();

	return true;
}

bool CSmcParserList::SmcInfoReadBin(std::istream& Input)
{
	if(!Input.good())
		return false; //ERROR: file can't be opened for parsing.

	// clear the queue
	m_queBuffer.ClearQueue();

	int iData = Input.get();
	while(Input.good())
	{	//전체 파일의 내용을 큐에 담는다.
		m_queBuffer.push(BYTE(iData));
		iData = Input.get();
	}

	// Queue에 담긴 데이터를 읽는다.
	ReadSmcInfo();

	return true;
}

bool CSmcParserList::SmcInfoReadBin(std::string sFilename)
{
	ifstream File;

	File.open(sFilename.c_str(), ios::binary);

	if( !File.good() )
		return false; // 에러

	return SmcInfoReadBin(File);
}

void CSmcParserList::WriteBin(std::string sFilename)
{
	ofstream File;

	File.open(sFilename.c_str(), ios::out | ios::trunc);

	if(!File.good())
		return; // 에러

	while(!m_queBuffer.empty())
	{
		File.put(m_queBuffer.front());
		m_queBuffer.pop();
	}

	File.flush(); // 파일에 데이터 기록
}

void CSmcParserList::WriteSmcInfo()
{
	vec_SmcParserItor SmcItor;

	m_queBuffer.WriteDword((DWORD)(*this).size()); // Smc정보 갯수
	int iCount = 0;

	for (SmcItor=(*this).begin(); SmcItor!=(*this).end(); SmcItor++)
	{
		iCount++;
		CSmcParser SmcParser = (*SmcItor);
		if (SmcParser.GetSmcFileName() == NULL)
		{
			m_queBuffer.WriteDword(0);
			continue;
		}
		else
		{
			m_queBuffer.WriteDword(iCount);
		}

		m_queBuffer.WriteString(SmcParser.GetSmcFileName()); // Smc 파일 이름
		// 메쉬 정보 루프
		int i,j;
		int iSize = SmcParser.GetMeshInfoListSize();
		m_queBuffer.WriteDword((DWORD)iSize); // 메쉬 갯수

		for (i=0; i<iSize; i++)
		{
			CMeshInfo Mesh = SmcParser.GetMeshInfo(i);
			m_queBuffer.WriteDword((DWORD)Mesh.GetMeshNumber()); // 메쉬 번호
			m_queBuffer.WriteString(Mesh.GetMeshTFNM()); // 메쉬 파일 경로(bm파일)
			
			int iTexSize = Mesh.GetTexInfoSize();
			m_queBuffer.WriteDword((DWORD)iTexSize); // 테스처 갯수

			for (j=0; j<iTexSize; j++)
			{
				m_queBuffer.WriteString(Mesh.GetTexInfoName(j)); // 텍스처 이름
				m_queBuffer.WriteString(Mesh.GetTexInfoTFNM(j)); // 텍스처 파일 경로
			}
		}
	}
}

void CSmcParserList::ReadSmcInfo()
{
	(*this).clear(); // 리스트를 비운다.

#ifdef WRITING_LOGINFO
	ClearLogList(); // 로그 정보 초기화
#endif

	std::string strTemp, strTemp2;
	int iSmcSize;
	int i,j,k;
	int iCount;

	iSmcSize = (int)m_queBuffer.ReadDword(); // Smc정보 갯수

	for (i=0; i<iSmcSize; i++)
	{
		CSmcParser SmcParser;
		iCount = (int)m_queBuffer.ReadDword();

		if (iCount!=0)
		{
			strTemp = m_queBuffer.ReadString(); // Smc파일 이름
			SmcParser.SetSmcFileName(strTemp.c_str());

			int iMeshSize = (int)m_queBuffer.ReadDword(); // 메쉬 총 갯수
			
			for (j=0; j<iMeshSize; j++)
			{
				CMeshInfo Mesh;
				int iMeshNum = (int)m_queBuffer.ReadDword(); //메쉬 번호
				strTemp = m_queBuffer.ReadString(); // 메쉬 파일 경로(bm파일)
				Mesh.SetMeshNumber(iMeshNum);
				Mesh.CreateMeshTFNM(strTemp.c_str());
				
				int iTexSize = (int)m_queBuffer.ReadDword(); // 텍스처 갯수
				
				for (k=0; k<iTexSize; k++)
				{
					strTemp = m_queBuffer.ReadString(); // 텍스처 이름
					strTemp2 = m_queBuffer.ReadString(); // 텍스처 파일 경로

					Mesh.AddTextureInfo(strTemp.c_str(), strTemp2.c_str());
				}

				SmcParser.AddMeshInfo(Mesh); // 메쉬 리스트 추가
			}

			SmcParser.SetParserType(CSmcParser::PARSER_END);
		}

		(*this).push_back(SmcParser); // Smc 정보 추가
	}
}


bool CSmcParserList::CheckItemExist(const CStaticArray<CItemData>& arrItemList, const char* saveFilePath)
{
	int nIndexCount = 0;
	int nTotalCount = 0;
	int nMissingCount = 0;
	
	CSmcParserList& smcList = *this;
	const CStaticArray<CItemData>& itemList = arrItemList;
	std::list<CTString> missingList;
	
	CTString strFullPath = _fnmApplicationPath.FileDir();

	for (int i = 0; i < itemList.Count(); ++i)
	{
		/// 장비만
		if (itemList[i].GetType() != CItemData::ITEM_WEAPON &&
			itemList[i].GetType() != CItemData::ITEM_SHIELD)
			continue;
		
		int index = itemList[i].GetItemIndex();
		
		/// ItemData[i] == NULL
		if (index == -1)
			continue;
		
		++nIndexCount;
		
		int nMeshInfo = smcList[index].GetMeshInfoListSize();
		nTotalCount += nMeshInfo;
		
		for (int j = 0; j < nMeshInfo; ++j)
		{
			CMeshInfo& pInfo = smcList[index].GetMeshInfo(j);
			const char* strMeshPath = pInfo.GetMeshTFNM();
			
			CTString path = strFullPath + strMeshPath;
			
			WIN32_FIND_DATA wfd;
			if (FindFirstFile(path, &wfd) == INVALID_HANDLE_VALUE)
			{
				CTString log;
				log.PrintF("[%d] Missing Mesh\t : %s", index, strMeshPath);
				missingList.push_back(log);
				++nMissingCount;
			}
			
			int nTexture = pInfo.GetTexInfoSize();
			nTotalCount += nTexture;
			
			for (int k = 0; k < nTexture; ++k)
			{
				const char* strTexturePath = pInfo.GetTexInfoTFNM(k);
				CTString path2 = strFullPath + strTexturePath;
				
				if (FindFirstFile(path2, &wfd) == INVALID_HANDLE_VALUE)
				{
					CTString log;
					log.PrintF("[%d] Missing Texture\t : %s", index, strTexturePath);
					missingList.push_back(log);
					++nMissingCount;
				}
			}
		}
	}

 	std::ofstream file(saveFilePath, std::ios::trunc);
	file << "<< Item Resource : START Check ! >>" << endl << endl;
	file << "Total : " << nTotalCount << endl;
	file << "Missing : " << nMissingCount << endl << endl;
	
	std::list<CTString>::iterator iter = missingList.begin();
	while (iter != missingList.end())
	{
		file << iter->str_String << std::endl;
		++iter;
	}
	
	file << endl << "<< END >>" << endl;
	file.close();

	return true;
}

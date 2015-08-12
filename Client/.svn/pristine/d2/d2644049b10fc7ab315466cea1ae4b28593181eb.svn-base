#pragma once

#ifndef __SMCPARSER_H
#define __SMCPARSER_H

#include <string>
#include <queue>
#include <Engine/Entities/SmcInfo.h>
#include <Engine/Entities/ItemData.h>

struct DeleteEntity
{
	template<typename T>
	void operator()(const T* ptr) const
	{
		delete ptr;
	}
};
/*template<typename T>
struct DeleteEntity : public std::unary_function<const T*, void> 
{
	void operator()(const T* ptr) const
	{
		delete ptr;
	}
};*/

class CSmcParser : public CSmcInfo
{
public:
	enum PARSER_TYPE
	{
		PARSER_NONE = 0,
		OFFSET,				// OFFSET    	0,0,0,180,0,0;
		PARSER_NAME,				// NAME "ti_bd_00s";
		NAME_START,			// {
		MESH,				// MESH       	TFNM "Data\Item\shield\Titan\ti_bd_00s.bm";
		TEXTURES,			// TEXTURES
		TEXTURES_START,		// {
		TEXTURES_NAME,		// "ti_bd_00S_N"	TFNM "Data\Item\shield\Titan\Texture\ti_bd_00S_N.tex";
		TEXTURES_END,		// }
		ALLFRAMESBBOX,		// ALLFRAMESBBOX	-0.5,0,-0.5,0.5,2,0.5;
		COLISION,			// COLISION
		COLISION_START,		// {
		COLISION_END,		// }
		NAME_END,			// }
		PARSER_END,			// Parsing Complete!
	};

	CSmcParser() { Clear(); }
	CSmcParser(const CSmcParser& SmcParser)
	{
		Clear();
		m_ParserType = SmcParser.m_ParserType;
		*((CSmcInfo*)this) = *((CSmcInfo*)&SmcParser);		
	}

	virtual ~CSmcParser()
	{
		Destroy();
	}

	void Clear()
	{ 
		m_ParserType = PARSER_NONE;
		m_iMeshCount = 0; m_iTexCount =0;
		m_pStrloglist = NULL;
	}

	void Destroy()
	{
		m_TempMeshInfo.Destroy();
		Clear();
	}

	void operator = (const CSmcParser& SmcParser)
	{
		Destroy();
		m_ParserType = SmcParser.m_ParserType;
		*((CSmcInfo*)this) = *((CSmcInfo*)&SmcParser);
	}

	bool LoadSmcParse(std::string sFilename);
	bool ProcessParse(std::istream& Input);
	bool ParseLine(const std::string& sLine);

	bool HandleError(const char* lpszFormat, ...);
	void SetStrloglist(std::vector<std::string>* strLogList) { m_pStrloglist = strLogList; }
	void SetParserType(PARSER_TYPE psType) { m_ParserType = psType; }
	std::string GetErrorMessage() { return m_sErrorMessage; }
	PARSER_TYPE GetParserType(void) { return m_ParserType; }
	
protected:
	CMeshInfo		m_TempMeshInfo;
	PARSER_TYPE		m_ParserType;
	std::string		m_sErrorMessage;
	int				m_iMeshCount;
	int				m_iTexCount;
	std::vector<std::string>* m_pStrloglist; //
};

typedef std::vector<CSmcParser>::iterator	vec_SmcParserItor;
typedef std::vector<CSmcParser>		vec_SmcParser;
typedef std::queue<unsigned char>	ByteQueue;

class ENGINE_API ByteQueueData : public ByteQueue
{
public:
	ByteQueueData();
	virtual ~ByteQueueData();

	// Write Basics types
	void WriteString(std::string sData);
	void WriteDword(DWORD dwData);
	void WriteWord(WORD wData);
	void WriteByte(BYTE byteData);
	void WriteFloat(float floatData);
	void WriteDouble(double dData);
	
	// Read Basic types
	std::string ReadString(void);
	DWORD ReadDword(void);
	WORD ReadWord(void);
	BYTE ReadByte(void);
	float ReadFloat(void);
	double ReadDouble(void);

	void WriteDocString(std::string sData);

	void ClearQueue(void);
};

class CSmcParserList : public std::vector<CSmcParser>
{
public:
	bool SmcInfoWriteBin(std::string sFilename);	// 현재 목록의 모든 Smc정보 저장
	bool SmcInfoWriteBin(std::ostream& Output);
	bool SmcInfoReadBin(std::string sFilename);		// 파일에서 모든 Smc정보를 목록에 기록
	bool SmcInfoReadBin(std::istream& Input);

	bool SmcParsingLogWritetxt(std::string sFilename); // 파싱 로그 정보 기록

	bool CheckItemExist(const CStaticArray<CItemData>& arrItemList, const char* saveFileName);

	void ClearLogList() { m_strLogList.clear(); }
	std::vector<std::string>* GetstrLogList() { return &m_strLogList; }
	void AddString(std::string sData);
	void WriteBin(std::string sFilename);
	bool IsEmptyqueBuffer(void) { return m_queBuffer.empty(); }

protected:
	void WriteSmcInfo();
	void ReadSmcInfo();
	
	ByteQueueData	m_queBuffer;
	std::vector<std::string> m_strLogList; // 로그 정보 기록
};

#endif
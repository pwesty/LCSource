#ifndef __ANPCREGENINFO_H__
#define __ANPCREGENINFO_H__

class CNPCRegenInfo
{
public:
	CNPCRegenInfo();
	~CNPCRegenInfo();

	////////////////////////////////////////
	// NPC_REGEN_INFO 에서 읽기

	// DB 필드
	int		m_index;			// 디비 레코드 인덱스
	int		m_npcIdx;			// 몹 인덱스
	int		m_zoneNo;			// 존번호
	float	m_regenX;			// 리젠 위치 : x
	int		m_regenY;			// 리젠 위치 : yLayer
	float	m_regenZ;			// 리젠 위치 : z
	float	m_regenR;			// 리젠 위치 : r
	int		m_regenSec;			// 리젠 딜레이 : 초
	int		m_totalNum;			// 최대 생성 수 : 최대 리젠 되는 수

	// 서버내에서 관리를 위한 멤버
	bool	m_bAlive;			// 리젠 이후 살아 있는지 죽었는지
	int		m_lastDieTime;		// 최근 죽은 시간
	int		m_numRegen;			// 누적 생성 수

	// 리젠되는 NPC의 파라미터
	int		m_paramHP;			// HP를 증가

	bool	m_bRaidMoveRandom;	// RAID 몹 비전투 이동 여부

	bool		m_bRegen;			// npc 리제 여부 결정

	CNPCProto*	m_npcProto;

	void SetHPParam(int val)
	{
		m_paramHP = val;
	}
	void clean();
};

class CArea;

class CNPCRegenList
{
public:
	CNPCRegenList();
	~CNPCRegenList();

	int				m_type;		// 종류
	CNPCRegenInfo*	m_infoList;	// 목록
	int				m_nCount;	// 수
	int				m_wPos;		// 다음 쓰는 위치

	void Init();
	void SetCount(int n);
	void AddRegenInfo(int index, int npc_index, int regenSec, float regenX, int yLayer, float regenZ, float regenR, int totalNum, int zonenNo, bool bRegen = true, CNPCProto* pNpcProto = NULL);
//	void AddRegenInfo(CNPCRegenInfo* info);

	bool Find(int idx, int zoneNo, bool pdZone = false, float x = 0, int y = 0, float z = 0);

	bool DelRegenInfo(CNPCRegenInfo * p);

	// 퍼던용 : bw
	bool SetRegened(CArea* pArea, int protoindex, int zoneNo, float x = 0, int y = 0, float z = 0);
	/*
	{
		int i, j, index;
		if( !pArea ) return false;

		if( !pArea->m_regenList ) return false;

		for(i = 0; i < m_nCount; i++)
		{
			if( m_infoList[i].m_zoneNo == zoneNo )
			{
				for(j = 0; j < m_nCount; j++)
				{
					if( m_infoList[i].m_index == pArea->m_regenList[j] )
					{
						pArea->m_regenList[j] = -1;
						return true;
					}
				}
			}
		}

		return false;
	}
	*/

	/*
	bool Find(int idx, int zoneNo)
	{
		CNPCRegenInfo s;
		s.m_npcIdx = idx;
		s.m_zoneNo = zoneNo;

		CNPCRegenInfo* result = (CNPCRegenInfo*)bsearch(&s, m_infoList, m_nCount, sizeof(CNPCRegenInfo), CompIndex);

		if(result == NULL)
		{
			delete result;
			return false;
		}

		result = NULL;
		delete result;
		return true;
	}

	private:
	static int CompIndex(const void* p1, const void* p2)
	{
		CNPCRegenInfo * i1 = (CNPCRegenInfo*) p1;
		CNPCRegenInfo * i2 = (CNPCRegenInfo*) p2;

		if( (i1->m_npcIdx - i2->m_npcIdx) == 0 )
			return i1->m_zoneNo - i2->m_zoneNo;

		return i1->m_npcIdx - i2->m_npcIdx;
	}
	*/

	CNPCRegenInfo * GetNpcRegenInfo(int idx);	// 리스트에 저장된 인덱스로 CNPCRegenInfo 가져오기
};

#endif
//

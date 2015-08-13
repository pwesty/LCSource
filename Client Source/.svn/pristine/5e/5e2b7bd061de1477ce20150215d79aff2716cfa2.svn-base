
#ifndef __UIGUILD_ARRANGE_MEMBER_CONT_H__
#define __UIGUILD_ARRANGE_MEMBER_CONT_H__

#if		!defined(WORLD_EDITOR)

using namespace boost::multi_index;

class clsGuildMemberNew
{	
public:
	typedef boost::shared_ptr<clsGuildMemberNew>	SP;

	clsGuildMemberNew()
	{
		lIndex			= -1;
		eRanking		= 2;	// GUILD_MEMBER_MEMBER		
		bOnline			= FALSE;
		sPosName		= "";	
		iLevel			=0;				
		iJob			=-1;		
		iJob2			=-1;
		sLocName		="";
		iLocation		=-1;	
		iServDegree		=0;		
		iCumulPoint		=0;
		iExpServ		=0;	
		iFameServ		=0;	
		bStashPermission = 0;
		sRanking		= "";
		sJobName		="";	
	}	

	SLONG			lIndex;
	std::string		strMemberName;		// 멤버명?
	int				eRanking;			// 순위?
	int				nArrangeRank;		// 직위 정렬을 위해 추가.
	BOOL			bOnline;			// 접속중?
	// WSS_NEW_GUILD_SYSTEM 070705 ------------------------------------------->>
	// 추가 멤버 변수
	std::string		sRanking;			// 디폴트 직위명
	std::string		sPosName;			// 직위명
	INT				iLevel;				// 레벨
	std::string		sJobName;			// 클래스 명
	INT				iJob;				// 클래스
	INT				iJob2;				// 전직 클래스
	std::string		sLocName;			// 접속 위치
	INT				iLocation;			// 접속 위치
	INT				iServDegree;		// 기여도 
	INT				iCumulPoint;		// 누적 포인트
	INT				iExpServ;			// 경험치 상납도
	INT				iFameServ;			// 명성치 상납도
	BYTE			bStashPermission;
	// -----------------------------------------------------------------------<<
};

typedef boost::multi_index_container <
	clsGuildMemberNew::SP,
	indexed_by <
		ordered_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, std::string, strMemberName) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, int, nArrangeRank) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, INT, iLevel) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, std::string, sJobName) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, std::string, sLocName) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, INT, iCumulPoint) >
	>
> ContGuild;

#else	// WORLD_EDITOR

#include <set>

class clsGuildMemberNew
{	
public:
	static clsGuildMemberNew* SP(clsGuildMemberNew* ptr)	{
		return ptr;
	}
	
	clsGuildMemberNew()
	{
		lIndex			= -1;
		eRanking		= 2;	// GUILD_MEMBER_MEMBER		
		bOnline			= FALSE;
		sPosName		= "";	
		iLevel			=0;				
		iJob			=-1;		
		iJob2			=-1;
		sLocName		="";
		iLocation		=-1;	
		iServDegree		=0;		
		iCumulPoint		=0;
		iExpServ		=0;	
		iFameServ		=0;	
		bStashPermission = 0;
		sRanking		= "";
		sJobName		="";	
	}	
	
	SLONG			lIndex;
	std::string		strMemberName;		// 멤버명?
	int				eRanking;			// 순위?
	int				nArrangeRank;		// 직위 정렬을 위해 추가.
	BOOL			bOnline;			// 접속중?
	// WSS_NEW_GUILD_SYSTEM 070705 ------------------------------------------->>
	// 추가 멤버 변수
	std::string		sRanking;			// 디폴트 직위명
	std::string		sPosName;			// 직위명
	INT				iLevel;				// 레벨
	std::string		sJobName;			// 클래스 명
	INT				iJob;				// 클래스
	INT				iJob2;				// 전직 클래스
	std::string		sLocName;			// 접속 위치
	INT				iLocation;			// 접속 위치
	INT				iServDegree;		// 기여도 
	INT				iCumulPoint;		// 누적 포인트
	INT				iExpServ;			// 경험치 상납도
	INT				iFameServ;			// 명성치 상납도
	BYTE			bStashPermission;
	// -----------------------------------------------------------------------<<
};

typedef		std::set< clsGuildMemberNew* >	ContGuild;

#endif	// WORLD_EDITOR

#endif // __UIGUILD_ARRANGE_MEMBER_CONT_H__
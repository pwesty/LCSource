#ifndef __SKILL_TREE_H__
#define __SKILL_TREE_H__

#define DEF_SKILL_COL	(8)
struct stSkillTreeRow
{
	int index[DEF_SKILL_COL];

	stSkillTreeRow()
	{
		int i;
		for( i = 0; i < DEF_SKILL_COL; ++i )
			index[i] = -1;
	}
};

class CSkillTree
{
public:
	~CSkillTree();	

	static int LoadSkillTreeDataFromFile(const char* FileName);

	static stSkillTreeRow* m_SkillTree[9][3];
	static int m_nSkillCount[9][3];

	static void	Destroy();

private:
#ifdef G_THAI
	//----------------------------------------------->
	// skilltree.bin은 전로컬 공통데이타 입니다.
	// 태국의 경우 큐브스킬이 추가되지 않아서 코드상에서 제거작업이 진행 되었습니다.
	static void ChangeExclusionData();
	//-----------------------------------------------<
#endif // G_THAI

#ifdef VER_TEST	// ver_test일 경우에 잘못된 데이타 화면에 출력하기 위한 용도.
	static void ErrDataOutput();
#endif // VER_TEST
};

#endif // __SKILL_TREE_H__
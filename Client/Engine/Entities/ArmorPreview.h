#ifndef __ARMOR_PREVIEW_H__
#define __ARMOR_PREVIEW_H__

#define DEF_ARMOR_ROW	(5)
#define DEF_ARMOR_COL	(8) // WEAR_TOTAL 0~7번까지 사용 7번은 등장비
#define DEF_DEFAULT_COL  DEF_ARMOR_COL + 2 // 얼굴 머리 추가

struct stArmorInfo
{
	int index[DEF_ARMOR_ROW][DEF_ARMOR_COL];

	stArmorInfo()
	{
		int i, j;

		for( i = 0; i < DEF_ARMOR_ROW; ++i )
		{
			for ( j = 0; j < DEF_ARMOR_COL; ++j )
				index[i][j] = -1;
		}			
	}
};

struct stDefaultWearInfo
{
	int index[DEF_DEFAULT_COL];

	stDefaultWearInfo()
	{
		int i;

		for( i = 0; i < DEF_DEFAULT_COL; ++i )
			index[i] = -1;
	}
};

class CArmorPreview
{
public:
	~CArmorPreview();	

	static int LoadPreviewArmorDataFromFile(const char* FileName);

	static stArmorInfo* m_ArmorPreview[TOTAL_JOB];
	static stDefaultWearInfo* m_DefaultWearInfo[TOTAL_JOB];
	static void	Destroy();
	
};

#endif // __ARMOR_PREVIEW_H__
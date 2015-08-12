#ifndef __TEVENT_STRING_H__
#define __TEVENT_STRING_H__

#define DEF_TEVENT_ROW	(65)
#define DEF_TEVENT_COL	(24) // 0 Index, 1 Title 2~23 String

struct stTeventInfo
{
	int index[DEF_TEVENT_ROW][DEF_TEVENT_COL];

	stTeventInfo()
	{
		int i, j;

		for( i = 0; i < DEF_TEVENT_ROW; ++i )
		{
			for ( j = 0; j < DEF_TEVENT_COL; ++j )
				index[i][j] = -1;
		}			
	}
};

class CTEventString
{
public:
	~CTEventString();	

	static int LoadTEventStringDataFromFile(const char* FileName);

	static stTeventInfo* m_TEventString;
	static void	Destroy();
	
	static BOOL IsValidEvent(int nID);
	static int GetTEventName(int nID);
};

#endif // __ARMOR_PREVIEW_H__
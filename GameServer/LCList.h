#ifndef __LCLIST_H__
#define __LCLIST_H__

// DATATYPE을 LINKED LIST로 관리
template <class DATATYPE>
class CLCListNode
{
protected:
	int				m_nID;		// 데이터 ID
	DATATYPE		m_data;		// 데이터 내용
	CLCListNode*	m_prev;		// 이전 포인터
	CLCListNode*	m_next;		// 다음 포인터

public:
	CLCListNode(int nID, DATATYPE data)
		: m_data(data)
	{
		m_nID = nID;
		m_data = data;
		m_prev = m_next = NULL;
	}

	~CLCListNode()
	{
		if (m_prev)
			m_prev->Next(m_next);
		if (m_next)
			m_next->Prev(m_prev);
		m_prev = NULL;
		m_next = NULL;
	}

	void			Prev(CLCListNode* prev)
	{
		m_prev = prev;
	}
	void			Next(CLCListNode* next)
	{
		m_next = next;
	}
	CLCListNode*	Prev()
	{
		return m_prev;
	}
	CLCListNode*	Next()
	{
		return m_next;
	}
	DATATYPE		Data()
	{
		return m_data;
	}
	int				ID()
	{
		return m_nID;
	}
};

template <class DATATYPE>
class CLCList
{
protected:
	int						m_nID;		// 데이터 ID
	CLCListNode<DATATYPE>*	m_head;		// 헤더 포인터
	CLCListNode<DATATYPE>*	m_tail;		// 테일 포인터
	int						m_nCount;	// 데이터 수
	int (*m_fnComp)(DATATYPE, DATATYPE);	// 비교함수 포인터, strcmp와 같은 리턴값(같으면 0, 다를경우 순서에 따라 -, 또는 +)

public:
	CLCList(int (*fnComp)(DATATYPE, DATATYPE))
	{
		m_nID = rand();
		m_head = m_tail = NULL;
		m_nCount = 0;
		m_fnComp = fnComp;
	}

	~CLCList()
	{
		RemoveAll();
		m_nID = 0;
	}


	////////////////////
	// Function name	: AddToHead
	// Description	    : data를 처음에 추가
	void AddToHead(DATATYPE data)
	{
		if (m_head == NULL)
		{
			m_head = new CLCListNode<DATATYPE>(m_nID, data);
			m_tail = m_head;
		}
		else
		{
			CLCListNode<DATATYPE>* pNew = new CLCListNode<DATATYPE>(m_nID, data);
			pNew->Next(m_head);
			m_head->Prev(pNew);
			m_head = pNew;
		}
		m_nCount++;
	}


	////////////////////
	// Function name	: AddToTail
	// Description	    : data를 마지막에 추가
	void AddToTail(DATATYPE data)
	{
		if (m_tail == NULL)
		{
			m_head = new CLCListNode<DATATYPE>(m_nID, data);
			m_tail = m_head;
		}
		else
		{
			CLCListNode<DATATYPE>* pNew = new CLCListNode<DATATYPE>(m_nID, data);
			pNew->Prev(m_tail);
			m_tail->Next(pNew);
			m_tail = pNew;
		}
		m_nCount++;
	}


	////////////////////
	// Function name	: RemoveData
	// Description	    : 리스트에서 data 삭제, == 연산자 사용
	void RemoveData(DATATYPE data)
	{
		CLCListNode<DATATYPE>* p = NULL;
		CLCListNode<DATATYPE>* pNext = m_head;
		while ((p = pNext))
		{
			pNext = pNext->Next();
			if (m_fnComp == NULL)
			{
				if (p->Data() == data)
				{
					Remove(p);
				}
			}
			else
			{
				if (m_fnComp(p->Data(), data) == 0)
				{
					Remove(p);
				}
			}
		}
	}


	////////////////////
	// Function name	: Remove
	// Description	    : 리스트에서 p 노드를 삭제
	void Remove(void* p)
	{
		CLCListNode<DATATYPE>* pNode = (CLCListNode<DATATYPE>*)p;
		if (pNode == m_head)
			m_head = m_head->Next();
		if (pNode == m_tail)
			m_tail = m_tail->Prev();
		delete pNode;
		m_nCount--;
	}


	////////////////////
	// Function name	: RemoveAll
	// Description	    : 리스트 초기화
	void RemoveAll()
	{
		while (m_head)
		{
			CLCListNode<DATATYPE>* p = m_head;
			m_head = m_head->Next();
			delete p;
		}
		m_head = m_tail = NULL;
		m_nCount = 0;
	}


	////////////////////
	// Function name	: GetHead
	// Description	    : 헤더 포인터 반환
	void* GetHead()
	{
		return (void*)m_head;
	}

	////////////////////
	// Function name	: GetTail
	// Description	    : 테일 포인터 반환
	void* GetTail()
	{
		return (void*)m_tail;
	}

	////////////////////
	// Function name	: GetPrev
	// Description	    : pos 이전 노드 포인터 반환
	void* GetPrev(void* pos)
	{
		CLCListNode<DATATYPE>* p = (CLCListNode<DATATYPE>*)pos;
		return ((p->ID() == m_nID)) ? p->Prev() : NULL;
	}

	////////////////////
	// Function name	: GetNext
	// Description	    : pos 다음 노드 포인터 반환
	void* GetNext(void* pos)
	{
		CLCListNode<DATATYPE>* p = (CLCListNode<DATATYPE>*)pos;
		return ((p->ID() == m_nID)) ? p->Next() : NULL;
	}

	////////////////////
	// Function name	: GetData
	// Description	    : pos 노드의 데이터를 반환
	DATATYPE GetData(void* pos)
	{
		CLCListNode<DATATYPE>* p = (CLCListNode<DATATYPE>*)pos;
		if (pos != NULL && p->ID() == m_nID)
			return p->Data();
		else
		{
			char empty[sizeof(DATATYPE)];
			memset(empty, 0, sizeof(DATATYPE));
			return *((DATATYPE*)empty);
		}
	}

	////////////////////
	// Function name	: GetCount
	// Description	    : 리스트의 데이터 수를 반환
	int GetCount()
	{
		return m_nCount;
	}


	////////////////////
	// Function name	: FindDataWithCompareFunction
	// Description	    : 데이터 노드 찾기
	// Return type		: void*
	//                  : 찾은 노드 포인터, 없으면 NULL
	// Argument         : DATATYPE data
	//                  : 찾을 데이터
	// Argument         : int (*m_fnComp)(DATATYPE, DATATYPE)
	//                  : 찾기에 사용할 함수, NULL이면 == 연산자 사용
	void* FindDataWithCompareFunction(DATATYPE data, int (*fnComp)(DATATYPE, DATATYPE))
	{
		CLCListNode<DATATYPE>* p = NULL;
		CLCListNode<DATATYPE>* pNext = (CLCListNode<DATATYPE>*)GetHead();
		while ((p = pNext))
		{
			pNext = pNext->Next();
			if (fnComp == NULL)
			{
				if (p->Data() == data)
				{
					return (void*)p;
				}
			}
			else
			{
				if (fnComp(p->Data(), data) == 0)
				{
					return (void*)p;
				}
			}
		}

		return NULL;
	}

	////////////////////
	// Function name	: FindData
	// Description	    : 데이터 노드 찾기
	// Return type		: void*
	//                  : 찾은 노드 포인터, 없으면 NULL
	// Argument         : DATATYPE data
	//                  : 찾을 데이터
	void* FindData(DATATYPE data)
	{
		return FindDataWithCompareFunction(data, m_fnComp);
	}


	////////////////////
	// Function name	: GetPosition
	// Description	    : nIndex번째에 해당하는 데이터를 반환, 0 base
	// Return type		: void*
	//                  : 해당 노드 포인터
	// Argument         : int nIndex
	//                  : 찾고자 하는 노드의 위치
	void* GetPosition(int nIndex)
	{
		if (nIndex < 0)
			return NULL;
		if (nIndex >= GetCount())
			return NULL;
		void* ret = GetHead();
		while (nIndex > 0)
		{
			ret = GetNext(ret);
			if (!ret)
				return NULL;
			nIndex--;
		}

		return ret;
	}
};

#endif // __LCLIST_H__
//


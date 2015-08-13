/*
#include "stdafx.h"
#include <assert.h>
#include "TxtQueue.h"
*/
#include "stdh.h"
#include <Engine/Network/TxtQueue.h>

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

CTxtQ::CTxtQ()
{
	//m_pHead = new CTxtBlock;
	//m_pTail = new CTxtBlock;

	m_pHead = m_pTail = NULL;

}

CTxtQ::~CTxtQ()
{	
	char tmp[1024];
	while(GetFromQ(tmp)!=0){
	}

	SAFE_DELETE(m_pHead);
}

int CTxtQ::GetFromQ(char *dest)
{
	CTxtBlock *tmp;
	
	/* queue empty? */
	if (!m_pHead)
		return (0);
	
	tmp = m_pHead;
	strcpy(dest, m_pHead->m_Text);	
	m_pHead = m_pHead->m_pNext;
	
	delete[] tmp->m_Text;
	delete[] tmp;
	
	return (1);
}

void CTxtQ::WriteToQ(const char *txt)
{
	CTxtBlock *newt;
		
	newt = new CTxtBlock [1];
	newt->m_Text = StrDup(txt);	
	
	/* queue empty? */
	if (!m_pHead) {
		newt->m_pNext = NULL;
		m_pHead = m_pTail = newt;
	} else {
		m_pTail->m_pNext = newt;
		m_pTail = newt;
		newt->m_pNext = NULL;
	}

}

char* CTxtQ::StrDup(const char *source)
{
	char* new_z = new char[strlen(source)+1];
	
	return (strcpy(new_z, source));
}
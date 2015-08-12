
class CTxtBlock {
public:
   char	*m_Text;   
   CTxtBlock *m_pNext;
}; 

class CTxtQ {
public:

	CTxtQ();
	~CTxtQ();

	CTxtBlock	*m_pHead, *m_pTail;

	char *StrDup(const char *source);
	
	int GetFromQ(char *dest);
	void WriteToQ(const char *txt);

};
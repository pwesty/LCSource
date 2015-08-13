#ifndef __ROCKPAPER_SCISSOR_PRO_H__
#define __ROCKPAPER_SCISSOR_PRO_H__

class CRockPaperScissorsProb
{
public:
	CRockPaperScissorsProb(void);
	CRockPaperScissorsProb(unsigned char userProb, unsigned char serverProb, unsigned char drawProb = -1);
	~CRockPaperScissorsProb(void);

	void SetUserWinProb(unsigned char prob);
	const unsigned char GetUserWinProb(void) const;

	void SetServerWinProb(unsigned char prob);
	const unsigned char GetServerWinProb(void) const;

	void SetDrawProb(unsigned char prob);
	const unsigned char GetDrawProb(void) const;

private:
	unsigned char m_userWinProb;
	unsigned char m_serverWinProb;
	unsigned char m_drawProb;
};

#endif

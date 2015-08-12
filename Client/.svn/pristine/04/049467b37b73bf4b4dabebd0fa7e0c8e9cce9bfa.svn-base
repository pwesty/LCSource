#include "KRFReadLib.h"
#include "kseek.h"

inline kifstream::kifstream()
:	m_File(NULL)
,	m_State(std::ios_base::goodbit)
{
}

inline kifstream::kifstream(const char* filename, std::ios_base::openmode mode)
:	m_File(kopen(filename, NULL))
,	m_State(std::ios_base::goodbit)
{
}

inline kifstream::~kifstream()
{
	close();
}

inline kifstream& kifstream::read(char* s, long long n)
{
	if (!m_File)
		return *this;

	// n is always small enough to cast
	m_Count = kread(s, 1, (size_t)n, m_File);

	setState(m_Count == n);

	return *this;
}

inline long long kifstream::gcount() const
{
	return m_Count;
}

inline bool kifstream::is_open() const
{
	return m_File != NULL;
}

inline bool kifstream::open(const char* filename, std::ios_base::openmode mode)
{
	m_File = kopen(filename, NULL);
	m_State = std::ios_base::goodbit;
}

inline void kifstream::close()
{
	if (m_File != NULL)
	{
		kclose(m_File);
		m_File = NULL;
	}
}

inline void kifstream::seekg(long pos)
{
	int res = kseek(m_File, pos, SEEK_SET);
	setState(res == 0);
}

inline void kifstream::seekg(long off, std::ios_base::seekdir dir)
{
	int origin;
	switch(dir)
	{
	case std::ios_base::beg:	origin = SEEK_SET;	break;
	case std::ios_base::cur:	origin = SEEK_CUR;	break;
	case std::ios_base::end:	origin = SEEK_END;	break;
	default:					setState(false);	return;
	}

	int res = kseek(m_File, off, origin);
	setState(res == 0);
}

inline long long kifstream::tellg() const
{
	return ktell(m_File);
}

inline void kifstream::setState(bool error)
{
	if (!error)
	{
		m_State = std::ios_base::goodbit;
		return;
	}
	m_State = std::ios_base::failbit;

	if (keof(m_File))
		m_State |= std::ios_base::eofbit;
	else
		m_State |= std::ios_base::badbit;
}

inline bool kifstream::good() const
{
	return m_State == std::ios_base::goodbit;
}

inline bool kifstream::fail() const
{
	return (m_State & (std::ios_base::failbit | std::ios_base::badbit)) != 0;
}

inline bool kifstream::bad() const
{
	return (m_State & std::ios_base::badbit) != 0;
}

inline bool kifstream::eof() const
{
	return (m_State & std::ios_base::eofbit) != 0;
}

inline std::ios_base::iostate kifstream::rdstate() const
{
	return m_State;
}

inline void kifstream::clear(std::ios_base::iostate state)
{
	m_State = state;
}

inline void kifstream::setstate(std::ios_base::iostate state)
{
	clear(rdstate() | state);
}

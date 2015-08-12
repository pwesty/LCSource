#ifndef KIFSTREAM_H
#define KIFSTREAM_H

#include <string>
#include <istream>

#define KFILE void

class kifstream
{
public:
	/** 
	 * Default constructor
	 */
	inline kifstream();
	/** 
	 * Constructor that opens a file. WARNING: the openmode is ignored!
	 */
	explicit inline kifstream(const char* filename, std::ios_base::openmode mode);
	/** 
	 * Destructor
	 */
	inline ~kifstream();

	/**
	 * Closes a file
	 */
	inline bool is_open() const;

	/**
	 * Closes a file
	 */
	inline bool open(const char* filename, std::ios_base::openmode mode);

	/**
	 * Closes a file
	 */
	inline void close();

	/**
	 * Returns the next chunk in the given buffer.
	 * If something goes wrong will return false
	 * count will be set to the actual bytes read
	 * ensure that the buffer has at least size count!
	 */
	inline kifstream& read(char* s, long long n);

	/**
	 * Seeks to a position.
	 *
	 * Note that this is implemented using kread, ktell, 
	 * kreset and kgetSize, and can be slow!
	 */
	inline void seekg(long location);

	/**
	 * Seeks to a position offset from an absolute position.
	 *
	 * Note that this is implemented using kread, ktell, 
	 * kreset and kgetSize, and can be slow!
	 */
	inline void seekg(long off, std::ios_base::seekdir dir);

	/**
	 * Returns the current stream pointer.
	 */
	inline long long tellg() const;

	/**
	 * returns the number of read files.
	 */
	inline long long gcount() const;

	/** @name state functions
	 * Returns the state of the last operation.
	 */
	//@{
	inline bool good() const;
	inline bool fail() const;
	inline bool bad() const;
	inline bool eof() const;
	inline std::ios_base::iostate rdstate() const;
	inline void clear(std::ios_base::iostate state);
	inline void setstate(std::ios_base::iostate state);
	//@}
protected:
	/**
	 * Internal function for setting or clearing errors
	 */
	inline void setState(bool error);

	long long m_Count;
	std::ios_base::iostate m_State;
	KFILE* m_File;
};

#include "kifstream.inl"
#endif //KIFSTREAM_H

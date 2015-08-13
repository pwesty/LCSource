#ifndef SE_INCL_SERIAL_H
#define SE_INCL_SERIAL_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Changeable.h>
#include <Engine/Base/FileName.h>

#ifdef KALYDO
#include <deque>

typedef struct __streaming_load_support__
{
	__streaming_load_support__() : pTarget(NULL)	{															}
	~__streaming_load_support__()					{	pTarget = NULL; pTargetFilePath.Clear();				}

	CSerial*		pTarget;
	CTString		pTargetFilePath;
} SLS;

typedef std::deque<SLS*> deqSLS;

extern deqSLS g_deqLoadData;

#endif

 /*
 * Abstract base class for objects that can be saved and loaded.
 */
class ENGINE_API CSerial : public CChangeable {
public:
  INDEX ser_ctUsed;         // use count
  CTFileName ser_FileName;  // last file name loaded
public:
  /* Default constructor. */
  CSerial(void);
  /* Destructor. */
  virtual ~CSerial( void);

  /* Get the file name of this object. */
  inline const CTFileName &GetName(void) { return ser_FileName; };
  /* Get the description of this object. */
  virtual CTString GetDescription(void);
//안태훈 수정 시작	//(Bug FIx)(0.1)
  /* Load from file. */
#ifdef KALYDO
  virtual void Load_t( const CTFileName &fnFileName); // throw char *
  virtual void Load_Delay_t( const CTFileName &fnFileName)	{}					// only used streaming data load.
#else
  void Load_t( const CTFileName &fnFileName); // throw char *
#endif
  /* Save to file. */
  void Save_t( const CTFileName &fnFileName); // throw char *
//안태훈 수정 끝	//(Bug FIx)(0.1)
  /* Reload from file. */
#ifdef KALYDO
  virtual void Reload(void);
#else
  void Reload(void);
#endif
  /* Mark that object is used once more. */
  void MarkUsed(void);
  /* Mark that object is used once less. */
  void MarkUnused(void);
  /* Check if object is used at least once. */
  BOOL IsUsed(void);
  INDEX GetUsedCount(void);

  /* Clear the object. */
  virtual void Clear(void);
  /* Read from stream. */
  virtual void Read_t( CTStream *istrFile)=0; // throw char *
  /* Write to stream. */
  virtual void Write_t( CTStream *ostrFile)=0; // throw char *
  // check if this kind of objects is auto-freed
  virtual BOOL IsAutoFreed(void) { return TRUE; };
  // get amount of memory used by this object
  virtual SLONG GetUsedMemory(void) { return -1; };
  // gather the CRC of the file
  virtual void AddToCRCTable(void);
};


#endif  /* include-once check. */


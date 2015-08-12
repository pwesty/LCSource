#include "stdh.h"

#include <Engine/Base/Serial.h>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Base/Stream.h>
#include <Engine/Base/CRCTable.h>

#ifdef KALYDO
deqSLS g_deqLoadData;
#endif

/*
 * Default constructor.
 */
CSerial::CSerial( void) : ser_ctUsed(0) // not used initially
{
}

/*
 * Destructor.
 */
CSerial::~CSerial( void)
{
  // must not be used at all
  ASSERT(ser_ctUsed == 0);    // look at _strLastCleared for possible name
}

/*
 * Clear the object.
 */
void CSerial::Clear(void)
{
  // mark that you have changed
  MarkChanged();

  // clear the filename
  ser_FileName.Clear();
}

/* Get the description of this object. */
CTString CSerial::GetDescription(void)
{
  return "<no description>";
}

/*
 * Load from file.
 */
//안태훈 수정 시작	//(Bug FIx)(0.1)
void CSerial::Load_t(const CTFileName &fnFileName)  // throw char *
//안태훈 수정 끝	//(Bug FIx)(0.1)
{
  ASSERT(!IsUsed());
  // mark that you have changed
  MarkChanged();

  // open a stream
  CTFileStream istrFile;
  istrFile.Open_t(fnFileName);
  // read object from stream
  Read_t(&istrFile);
  // if still here (no exceptions raised)
  // remember filename
  ser_FileName = fnFileName;
}

/*
 * Reload from file.
 */
void CSerial::Reload(void)
{
  /* if not found, */
  TRACKMEM(Mem, strrchr((const char*)ser_FileName, '.'));

  // mark that you have changed
  MarkChanged();

  CTFileName fnmOldName = ser_FileName;
  Clear();
  // try to
  try {
    // open a stream
    CTFileStream istrFile;
    istrFile.Open_t(fnmOldName);
    // read object from stream
    Read_t(&istrFile);

  // if there is some error while reloading
  } catch (char *strError) {
    // quit the application with error explanation
    FatalError(TRANS("Cannot reload file '%s':\n%s"), (CTString&)fnmOldName, strError);
  }

  // if still here (no exceptions raised)
  // remember filename
  ser_FileName = fnmOldName;
}

/*
 * Save to file.
 */
//안태훈 수정 시작	//(Bug FIx)(0.1)
void CSerial::Save_t(const CTFileName &fnFileName)  // throw char *
//안태훈 수정 끝	//(Bug FIx)(0.1)
{
  // open a stream
  CTFileStream ostrFile;
  ostrFile.Create_t(fnFileName);
  // write object to stream
  Write_t(&ostrFile);
  // if still here (no exceptions raised)
  // remember new filename
  ser_FileName = fnFileName;

  ostrFile.Close();
}

/*
 * Mark that object is used once more.
 */
void CSerial::MarkUsed(void)
{
  // use count must not have dropped below zero
  ASSERT(ser_ctUsed>=0);
  // increment use count
  ser_ctUsed++;
}

/*
 * Mark that object is used once less.
 */
void CSerial::MarkUnused(void)
{
  // decrement use count
  ser_ctUsed--;
  // use count must not have dropped below zero
  ASSERT(ser_ctUsed>=0);
}

/*
 * Check if object is used at least once.
 */
BOOL CSerial::IsUsed(void)
{
  // use count must not have dropped below zero
  ASSERT(ser_ctUsed>=0);

  return ser_ctUsed>0;
}
INDEX CSerial::GetUsedCount(void)
{
  return ser_ctUsed;
}

// gather the CRC of the file
void CSerial::AddToCRCTable(void)
{
  // add the file to CRC table
  CRCT_AddFile_t(ser_FileName);
}

#ifndef __LIBHASHTBL_H___
#define __LIBHASHTBL_H___

/**********************************************************
   Author   : Elenoa
   Section  :
   SCCS ID  : %W%
   Date     : %G%
   Revision History :
    '07.  9. 13  Initial


   Description: Shared Memory, Indexed by Hash Table

   Copyright (c) Numensoft 2007
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <asm/atomic.h>
#include <sched.h>


/* Fixed Variables */
#define __RESERVED		8
#define __D_RESERVED	16
#define HASHED_TBL_DEFAULT_HASH_NUM			20000

/* Function Return Code */
#define HASHED_TBL_SUCCESS_OK					1
#define HASHED_TBL_SUCCESS_CREATED				1
#define HASHED_TBL_SUCCESS_EXIST				2
#define HASHED_TBL_SUCCESS_CONTINUE				3

/* ERROR: Init_hashed_tbl */
#define HASHED_TBL_ERR_NO_ENTITY_SIZE			-1
#define HASHED_TBL_ERR_NO_ENTITY_NUM			-2
#define HASHED_TBL_ERR_NO_SHMKEY				-3
#define HASHED_TBL_ERR_CANNOT_GET_SHMID_EXIST	-4
#define HASHED_TBL_ERR_CANNOT_GET_SHMID_CREATED	-5
#define HASHED_TBL_ERR_CANNOT_ATTACH			-6
#define HASHED_TBL_ERR_OVERFLOW					-7
#define HASHED_TBL_ERR_NOT_MATCH_ENTITY_SIZE	-8
#define HASHED_TBL_ERR_NOT_MATCH_ENTITY_NUM		-9
#define HASHED_TBL_ERR_NOT_MATCH_HASH_NUM		-10

/* ERROR: Get_hashed_tbl, New_hashed_tbl, Release_hashed_tbl */
/* ERROR: Delete_hashed_tbl, Purge_hashed_tbl */
#define HASHED_TBL_ERR_HASHKEY_OVER_LIMIT		-11
#define HASHED_TBL_ERR_NOT_FOUND				-12
#define HASHED_TBL_ERR_ILLEGAL_REQUEST			-13
#define HASHED_TBL_ERR_NOT_MATCH_KEY			-14
#define HASHED_TBL_ERR_NOT_FOUND_TO_DELETE		-15
#define HASHED_TBL_ERR_RMID_FAILED				-16
#define HASHED_TBL_ERR_ALREADY_EXIST			-17
#define HASHED_TBL_ERR_DATA_CORRUPTION			-18
#define HASHED_TBL_ERR_MEMORY_FULL				-19

/* NEW */
#define HASHED_TBL_ERR_GETLOCK					-20


typedef struct _st_HashedTblHEntity
		st_HashedTblHEntity, *pst_HashedTblHEntity;

typedef struct _st_HashedTblHeader
{
	unsigned int uiMapXLength;
	unsigned int uiMapYLength;
	unsigned int uiHashTotal;
	unsigned int uiEntityTotal;
	unsigned int uiEntityNext;
	atomic_t aiEntityUsed;
	unsigned int uiEntityDuple;
} st_HashedTblHeader, *pst_HashedTblHeader;

struct _st_HashedTblHEntity
{
	unsigned long long ullLocKey;
	unsigned long long ullEntityKey;
	unsigned short usType;
	unsigned short usReserved;
	unsigned int uiOffNext;
};

typedef struct _st_HashedTblTable
{
	unsigned long long ullLocKey;
	unsigned int uiChildNum;
	unsigned int uiOffEntityHeader;
} st_HashedTblTable, *pst_HashedTblTable;

typedef struct _st_HashedTblHandle
{
	/* Hash Table Related */
	unsigned int uiHashNum;
	unsigned int uiMapXLength;
	unsigned int uiMapYLength;
	pst_HashedTblTable pstHashTable;

	/* Data Related */
	unsigned int uiEntityNum;
	void *pEntityStartPos;

	/* Shared Memory Physical */
	int iShmId;
	key_t kShmKey;
	int iShmTotalSize;	/* Calculated by uiEntitySize, uiEntityNum */
	int iHashTotalSize;	/* Calculated by uiHashNum, sizeof(st_HashedTblTable) */
	void *pAttachAddr;
	pst_HashedTblHeader pstHeader;

	/* To Lock */
	int iLock;
	int iNumBlock;
	int iMaxBlock;

	/* To Browse */
	unsigned int uiOffEntityHeaderBrowse;
	int iEntityBrowseCurrent;
} st_HashedTblHandle, *pst_HashedTblHandle;


/* Function Prototypes */
const char *Msg_hashed_tbl(int iRetval);
int Init_hashed_tbl_handle(pst_HashedTblHandle pstHandle, unsigned int uiMapXLen, unsigned int uiMapYLen, unsigned int uiEntityNum, key_t kShmKey);
int Init_hashed_tbl(pst_HashedTblHandle pstHandle);
int Get_hashed_tbl(pst_HashedTblHandle pstHandle, unsigned long long ullKey, pst_HashedTblHEntity *ppstEntity);
int Get_hashed_tbl_next(pst_HashedTblHandle pstHandle, pst_HashedTblHEntity *ppstEntity);
int New_hashed_tbl(pst_HashedTblHandle pstHandle, unsigned long long ullKey, unsigned short usType, unsigned long long ullEntityKey);
int Release_hashed_tbl(pst_HashedTblHandle pstHandle, unsigned long long ullKey, unsigned long long ullEntityKey);
int Delete_hashed_tbl(pst_HashedTblHandle pstHandle);
int Purge_hashed_tbl(pst_HashedTblHandle pstHandle);
int Browse_hashed_tbl(pst_HashedTblHandle pstHandle, pst_HashedTblHEntity *ppstEntityHeaderBrowse);
int Build_hashed_tbl_key(pst_HashedTblHandle pstHandle, unsigned int uiXLoc, unsigned int uiYLoc, unsigned long long *pullBuilded);

#define Browse_hashed_tbl_clear(ph)	Browse_hashed_tbl((ph), NULL)

/* Macro for Shared Memory Segment */
#define SESSIONID_TO_HASHKEY(sid)	(sid)
#define HASHKEY_TO_SESSIONID(hk)	(kk)

#define KEY_BUILD_MASK				(0x000000000000FFFFLL)
#define EXCEPT_HASH_MASK			(0x0000FFFFFFFFFFFFLL)

#define IS_EMPTY_OFFSET(h, eh)	\
		(((unsigned long long)(eh) - (unsigned long long)((h)->pEntityStartPos)) == 0xFFFFFFFFL)

#define HASH_TO_OFFSET(h, hm)		\
		((((void *)(hm)) - ((void *)(h)->pstHashTable)) + 1)

#define OFFSET_TO_HASH(h, ho)		\
		(pst_HashedTblTable)(((void *)(h)->pstHashTable) + (ho - 1))

#define ENTITIES_TO_OFFSET(h, em)		\
		((((void *)(em)) - ((void *)(h)->pEntityStartPos)) + 1)

#define OFFSET_TO_ENTITIES(h, eo)		\
		(((void *)(h)->pEntityStartPos) + (eo - 1))

#define ETO(em)		ENTITIES_TO_OFFSET(pstHandle, em)
#define OTE(eo)		OFFSET_TO_ENTITIES(pstHandle, eo)

#define ONE_ENTITY_SIZE(h)			\
		(__RESERVED + sizeof(st_HashedTblHEntity))

#define ENTITY_HEADER(h, n)			\
		((h)->pEntityStartPos + (ONE_ENTITY_SIZE(h) * (n)))

#endif


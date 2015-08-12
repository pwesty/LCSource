#ifndef __LIBHASHSHM_H___
#define __LIBHASHSHM_H___

/**********************************************************
   Author   : Elenoa
   Section  :
   SCCS ID  : %W%
   Date     : %G%
   Revision History :
    '07.  9. 13  Initial
    '12.  3. 29 Extend Entity Number


   Description: Shared Memory, Indexed by Hash Table

   Copyright (c) Numensoft 2007
   Copyright (c) Barunsonsoft 2012
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
#define HASHED_SHM_DEFAULT_HASH_NUM			20000

/* Function Return Code */
#define HASHED_SHM_SUCCESS_OK					1
#define HASHED_SHM_SUCCESS_CREATED				1
#define HASHED_SHM_SUCCESS_EXIST				2
#define HASHED_SHM_SUCCESS_CONTINUE				3

/* ERROR: Init_hashed_shm */
#define HASHED_SHM_ERR_NO_ENTITY_SIZE			-1
#define HASHED_SHM_ERR_NO_ENTITY_NUM			-2
#define HASHED_SHM_ERR_NO_SHMKEY				-3
#define HASHED_SHM_ERR_CANNOT_GET_SHMID_EXIST	-4
#define HASHED_SHM_ERR_CANNOT_GET_SHMID_CREATED	-5
#define HASHED_SHM_ERR_CANNOT_ATTACH			-6
#define HASHED_SHM_ERR_OVERFLOW					-7
#define HASHED_SHM_ERR_NOT_MATCH_ENTITY_SIZE	-8
#define HASHED_SHM_ERR_NOT_MATCH_ENTITY_NUM		-9
#define HASHED_SHM_ERR_NOT_MATCH_HASH_NUM		-10

/* ERROR: Get_hashed_shm, New_hashed_shm, Release_hashed_shm */
/* ERROR: Delete_hashed_shm, Purge_hashed_shm */
#define HASHED_SHM_ERR_HASHKEY_OVER_LIMIT		-11
#define HASHED_SHM_ERR_NOT_FOUND				-12
#define HASHED_SHM_ERR_ILLEGAL_REQUEST			-13
#define HASHED_SHM_ERR_NOT_MATCH_KEY			-14
#define HASHED_SHM_ERR_NOT_FOUND_TO_DELETE		-15
#define HASHED_SHM_ERR_RMID_FAILED				-16
#define HASHED_SHM_ERR_ALREADY_EXIST			-17
#define HASHED_SHM_ERR_DATA_CORRUPTION			-18
#define HASHED_SHM_ERR_MEMORY_FULL				-19

/* NEW */
#define HASHED_SHM_ERR_GETLOCK					-20
#define HASHED_SHM_ERR_TYPE_NOT_MATCH			-21


#if 0
typedef struct _st_HashedShmEntityHeader
		st_HashedShmEntityHeader, *pst_HashedShmEntityHeader;
#endif

typedef struct _st_HashedShmHeader
{
	unsigned int uiHashType;
	unsigned int uiReserved;
	unsigned int uiHashTotal;
	unsigned int uiEntitySize;
	unsigned int uiEntityTotal;
	unsigned int uiEntityNext;
	atomic_t aiEntityUsed;
	unsigned int uiEntityDuple;
} st_HashedShmHeader, *pst_HashedShmHeader;

typedef struct _st_HashedShmEntityHeader
{
	unsigned long long ullKey;
	unsigned int uiOffNext;
	unsigned int uiReserved;
} st_HashedShmEntityHeader, *pst_HashedShmEntityHeader;

typedef struct _st_HashedShmKey
{
	unsigned long long __t;
	unsigned long long __b;
} st_HashedShmKey, *pst_HashedShmKey;

typedef struct _st_HashedShmEntityHeader4B
{
	st_HashedShmKey stKey;
	unsigned int uiOffNext;
	unsigned int uiReserved;
} st_HashedShmEntityHeader4B, *pst_HashedShmEntityHeader4B;

typedef struct _st_HashedShmTable
{
	unsigned int uiChildNum;
	unsigned int uiOffEntityHeader;
} st_HashedShmTable, *pst_HashedShmTable;

#define HASHED_SHM_ORIGINAL		1
#define HASHED_SHM_4B			2

typedef struct _st_HashedShmHandle
{
	/* Hash Table Related */
	unsigned int uiHashType;
	unsigned int uiHashNum;
	pst_HashedShmTable pstHashTable;

	/* Data Related */
	unsigned int uiEntitySize;
	unsigned int uiEntityNum;
	void *pEntityStartPos;

	/* Shared Memory Physical */
	int iShmId;
	key_t kShmKey;
	int iShmTotalSize;	/* Calculated by uiEntitySize, uiEntityNum */
	int iHashTotalSize;	/* Calculated by uiHashNum, sizeof(st_HashedShmTable) */
	void *pAttachAddr;
	pst_HashedShmHeader pstHeader;

	/* To Lock */
	int iLock;
	int iNumBlock;
	int iMaxBlock;

	/* To Browse */
	unsigned int uiOffEntityHeaderBrowse;
	int iEntityBrowseCurrent;
} st_HashedShmHandle, *pst_HashedShmHandle;


/* Function Prototypes */
const char *Msg_hashed_shm(int iRetval);
int Init_hashed_shm_handle(pst_HashedShmHandle pstHandle, unsigned int uiHashNum, unsigned int uiEntitySize, unsigned int uiEntityNum, key_t kShmKey);
int Init_hashed_shm(pst_HashedShmHandle pstHandle);
int Get_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long ullKey, void **ppEntity);
int New_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long ullKey, void **ppEntity);
int Release_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long ullKey, void *pEntity);
int Delete_hashed_shm(pst_HashedShmHandle pstHandle);
int Purge_hashed_shm(pst_HashedShmHandle pstHandle);
int Browse_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long *pullKey, void **ppEntity);
int Build_hashed_shm_key(pst_HashedShmHandle pstHandle, unsigned long long ullVToKey, unsigned long long ullVAno, unsigned long long *pullBuilded);
int Build_hashed_shm_key_seed(pst_HashedShmHandle pstHandle, unsigned long long ullVToKey, unsigned long long ullVAno, int iNum, int iMax, unsigned long long *pullBuilded);

/* Function Prototypes: 4B */
const char *Msg_hashed_shm_4b(int iRetval);
int Init_hashed_shm_handle_4b(pst_HashedShmHandle pstHandle, unsigned int uiHashNum, unsigned int uiEntitySize, unsigned int uiEntityNum, key_t kShmKey);
int Init_hashed_shm_4b(pst_HashedShmHandle pstHandle);
int Get_hashed_shm_4b(pst_HashedShmHandle pstHandle, pst_HashedShmKey pstKey, void **ppEntity);
int New_hashed_shm_4b(pst_HashedShmHandle pstHandle, pst_HashedShmKey pstKey, void **ppEntity);
int Release_hashed_shm_4b(pst_HashedShmHandle pstHandle, pst_HashedShmKey pstKey, void *pEntity);
int Delete_hashed_shm_4b(pst_HashedShmHandle pstHandle);
int Purge_hashed_shm_4b(pst_HashedShmHandle pstHandle);
int Browse_hashed_shm_4b(pst_HashedShmHandle pstHandle, pst_HashedShmKey pstKey, void **ppEntity);
int Build_hashed_shm_key_4b(pst_HashedShmHandle pstHandle, unsigned long long ullVToKey, unsigned long long ullVAno, pst_HashedShmKey pstKey);
int Build_hashed_shm_key_seed_4b(pst_HashedShmHandle pstHandle, unsigned long long ullVToKey, unsigned long long ullVAno, int iNum, int iMax, pst_HashedShmKey pstKey);

#define Browse_hashed_shm_clear(ph)		Browse_hashed_shm((ph), NULL, NULL)
#define Browse_hashed_shm_clear_4b(ph)	Browse_hashed_shm_4b((ph), NULL, NULL)

#define MY_BLOCK(h, p)	((h)->iMaxBlock ? (((p) / (h)->iMaxBlock * (h)->iMaxBlock) + (h)->iNumBlock) : (p))
#define NEXT_BLOCK(h, p)	((p) + ((h)->iMaxBlock ? (h)->iMaxBlock : 1))

/* Macro for Shared Memory Segment */
#define SESSIONID_TO_HASHKEY(sid)	(*(((unsigned short *)&(sid)) + 3))
#define HASHKEY_TO_SESSIONID(hk)	\
		(((unsigned long long)((unsigned short)(hk))) << 24)

#define KEY_BUILD_MASK				(0x000000000000FFFFLL)
#define KEY_BUILD_MASK_4B			(0x00000000FFFFFFFFLL)
#define EXCEPT_HASH_MASK			(0x0000FFFFFFFFFFFFLL)

#define SESSIONID_TO_HASHKEY_4B(sid)	(sid->__t & KEY_BUILD_MASK_4B)
//#define HASHKEY_TO_SESSIONID_4B(hk)	it's not need

#define IS_EMPTY_OFFSET(h, eh)	\
		(((unsigned long long)(eh) - (unsigned long long)((h)->pEntityStartPos)) == 0xFFFFFFFFL)

#define HASH_TO_OFFSET(h, hm)		\
		((((void *)(hm)) - ((void *)(h)->pstHashTable)) + 1)

#define OFFSET_TO_HASH(h, ho)		\
		(pst_HashedShmTable)(((void *)(h)->pstHashTable) + (ho - 1))

#define ENTITIES_TO_OFFSET(h, em)		\
		((((void *)(em)) - ((void *)(h)->pEntityStartPos)) + 1)

#define OFFSET_TO_ENTITIES(h, eo)		\
		(((void *)(h)->pEntityStartPos) + (eo - 1))

#define ETO(em)		ENTITIES_TO_OFFSET(pstHandle, em)
#define OTE(eo)		OFFSET_TO_ENTITIES(pstHandle, eo)

/* Normal Entity Control */
#define ONE_ENTITY_SIZE(h)			\
		((h)->uiEntitySize + __D_RESERVED + sizeof(st_HashedShmEntityHeader))

#define ENTITY(h, n)				\
		((h)->pEntityStartPos + sizeof(st_HashedShmEntityHeader) + \
				__RESERVED + (ONE_ENTITY_SIZE(h) * (n)))

#define ENTITY_HEADER(h, n)			\
		((h)->pEntityStartPos + (ONE_ENTITY_SIZE(h) * (n)))

#define ENTITY_HEADER_TO_ENTITY(eh)	\
		(((void *)(eh)) + sizeof(st_HashedShmEntityHeader) + __RESERVED)

#define ENTITY_TO_ENTITY_HEADER(e)	\
		(((void *)(e)) - sizeof(st_HashedShmEntityHeader) - __RESERVED)

/* 4B Entity Control */
#define ONE_ENTITY4B_SIZE(h)			\
		((h)->uiEntitySize + __D_RESERVED + sizeof(st_HashedShmEntityHeader4B))

#define ENTITY4B(h, n)				\
		((h)->pEntityStartPos + sizeof(st_HashedShmEntityHeader4B) + \
				__RESERVED + (ONE_ENTITY4B_SIZE(h) * (n)))

#define ENTITY4B_HEADER(h, n)			\
		((h)->pEntityStartPos + (ONE_ENTITY4B_SIZE(h) * (n)))

#define ENTITY4B_HEADER_TO_ENTITY4B(eh)	\
		(((void *)(eh)) + sizeof(st_HashedShmEntityHeader4B) + __RESERVED)

#define ENTITY4B_TO_ENTITY4B_HEADER(e)	\
		(((void *)(e)) - sizeof(st_HashedShmEntityHeader4B) - __RESERVED)


/* utility functions */
int prevent_cpu(int iNum1, int iNum2);
int occupy_cpu(int iNum);

#endif


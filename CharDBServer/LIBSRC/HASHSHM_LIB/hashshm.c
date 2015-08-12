/* vim:set ts=4 sw=4
 */
/******************************************************************************
   Author	: Elenoa
   Section	: 
   SCCS ID	: %W%
   Date		: %G%
   Revision History :
   	'07.  9. 13	Initial


   Description : Shared Memory, Indexed by Hash Table

   Copyright (c) Numensoft 2007
******************************************************************************/

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
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>


/* Private Library Headers */
#include <libhashshm.h>
#include <utillib.h>
//#include "libhashshm_work.h"


//#define __HASHSHM_LOCK__
//#define __DEBUG_HASHSHM_INIT__
//#define __DEBUG_HASHSHM_GET__
//#define __DEBUG_HASHSHM_NEW__
//#define __DEBUG_HASHSHM_REMOVE__
//#define __DEBUG_HASHSHM_BROWSE__

/* Internal Functions */

int inline get_hash_entry(pst_HashedShmHandle pstHandle, unsigned long long ullKey, pst_HashedShmTable *ppstHashTable)
{
	unsigned short usHashKey = SESSIONID_TO_HASHKEY(ullKey);
	pst_HashedShmTable pstHashTable;

	/* check hash key (limit) */
	if (usHashKey > pstHandle->uiHashNum) {
		return HASHED_SHM_ERR_HASHKEY_OVER_LIMIT;
	}

	/* get hash entry by hashkey */
	*ppstHashTable = ((pstHandle->pstHashTable) + usHashKey);

#ifdef __DEBUG_HASHSHM_GET__
	fprintf(stderr, "HASHKEY %d TABLE %x\n", usHashKey, (*ppstHashTable));
#endif

	return HASHED_SHM_SUCCESS_OK;
}


//int inline browse_next_entry(pst_HashedShmHandle pstHandle, unsigned int *puiOffNextEntityHeader)
int inline browse_next_entry(pst_HashedShmHandle pstHandle, pst_HashedShmEntityHeader *ppstNextEntityHeader)
{
	int i;
	pst_HashedShmEntityHeader pstEntityHeader;

	for (i = pstHandle->iEntityBrowseCurrent;
		 i < pstHandle->pstHeader->uiEntityTotal; i++) {

		pstEntityHeader = ENTITY_HEADER(pstHandle, i);
		//fprintf(stderr, "%5d %llx\n", i, pstEntityHeader);
		if (pstEntityHeader && pstEntityHeader->ullKey > 0) {
			*ppstNextEntityHeader = pstEntityHeader;
			pstHandle->iEntityBrowseCurrent = i + 1;
			return HASHED_SHM_SUCCESS_OK;
		}
	}
	*ppstNextEntityHeader = NULL;
	pstHandle->iEntityBrowseCurrent = 0;

	return HASHED_SHM_ERR_NOT_FOUND;
}


/* file lock */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int init_lock(char *lockname)
{
    int fd = open(lockname, O_CREAT | O_RDWR);

    return fd;
}

int file_lock(int fd)
{
    struct flock _lock;

    memset((char *)&_lock, 0, sizeof(struct flock));
    _lock.l_type = F_WRLCK;
    _lock.l_start = 0;
    _lock.l_whence = SEEK_SET;
    _lock.l_len = 0;

    return fcntl(fd, F_SETLKW, &_lock);
}

int file_unlock(int fd)
{
    struct flock _lock;

    memset((char *)&_lock, 0, sizeof(struct flock));
    _lock.l_type = F_UNLCK;
    _lock.l_start = 0;
    _lock.l_whence = SEEK_SET;
    _lock.l_len = 0;

    return fcntl(fd, F_SETLK, &_lock);
}


/* External Functions */

int Init_hashed_shm_handle(pst_HashedShmHandle pstHandle, unsigned int uiHashNum, unsigned int uiEntitySize, unsigned int uiEntityNum, key_t kShmKey)
{
	memset((char *)pstHandle, 0, sizeof(st_HashedShmHandle));

	pstHandle->uiHashType = HASHED_SHM_ORIGINAL;
	pstHandle->uiHashNum = uiHashNum;
	pstHandle->uiEntitySize = uiEntitySize;
	pstHandle->uiEntityNum = uiEntityNum;
	pstHandle->kShmKey = kShmKey;

	return HASHED_SHM_SUCCESS_OK;
}

int Build_hashed_shm_key(pst_HashedShmHandle pstHandle, unsigned long long ullVToKey, unsigned long long ullVAno, unsigned long long *pullBuilded)
{
	unsigned long long ullTemp = 0;

	//ullVToKey = 0x1234567890123456;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	ullTemp = (ullVToKey & KEY_BUILD_MASK) ^
			  ((ullVToKey >> 16) & KEY_BUILD_MASK) ^
			  ((ullVToKey >> 32) & KEY_BUILD_MASK) ^
			  ((ullVToKey >> 48) & KEY_BUILD_MASK);

	/*
	fprintf(stderr, "parse 1 %x : %04x,%04x,%04x,%04x\n",
			ullTemp,
			(ullVToKey & KEY_BUILD_MASK),
			((ullVToKey >> 16) & KEY_BUILD_MASK),
			((ullVToKey >> 32) & KEY_BUILD_MASK),
			((ullVToKey >> 48) & KEY_BUILD_MASK));
	*/
	ullTemp = (ullTemp % pstHandle->uiHashNum);
	//fprintf(stderr, "parse 2 %x\n", ullTemp);
	ullTemp <<= 48;
	//fprintf(stderr, "parse 3 %lx\n", ullTemp);

	*pullBuilded = (ullTemp) | (ullVAno & EXCEPT_HASH_MASK);
	//fprintf(stderr, "end parse %lx\n", *pullBuilded);

	return HASHED_SHM_SUCCESS_OK;
}

int inline __Build_hashed_shm_key(pst_HashedShmHandle pstHandle, unsigned long long ullVToKey, unsigned long long ullVAno, int iNum, int iMax, unsigned long long *pullBuilded)
{
	unsigned long long ullTemp = 0;
	int iSeed;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	//ullVToKey = 0x1234567890123456;

	ullTemp = (ullVToKey & KEY_BUILD_MASK) ^
			  ((ullVToKey >> 16) & KEY_BUILD_MASK) ^
			  ((ullVToKey >> 32) & KEY_BUILD_MASK) ^
			  ((ullVToKey >> 48) & KEY_BUILD_MASK);

	/*
	fprintf(stderr, "parse 1 %x : %04x,%04x,%04x,%04x\n",
			ullTemp,
			(ullVToKey & KEY_BUILD_MASK),
			((ullVToKey >> 16) & KEY_BUILD_MASK),
			((ullVToKey >> 32) & KEY_BUILD_MASK),
			((ullVToKey >> 48) & KEY_BUILD_MASK));
	*/
	ullTemp = (ullTemp % pstHandle->uiHashNum);
	//fprintf(stderr, "parse 2 %x max %d\n", ullTemp, iMax);
	iSeed = (ullTemp % iMax);
	//fprintf(stderr, "parse 2-1 %x seed %d num %d\n", ullTemp, iSeed, iNum);
	ullTemp = ullTemp - iSeed + iNum;
	//fprintf(stderr, "parse 2-2 %x\n", ullTemp);
	ullTemp <<= 48;
	//fprintf(stderr, "parse 3 %lx\n", ullTemp);

	*pullBuilded = (ullTemp) | (ullVAno & EXCEPT_HASH_MASK);
	//fprintf(stderr, "end parse %lx\n", *pullBuilded);

	return HASHED_SHM_SUCCESS_OK;
}

int Build_hashed_shm_key_seed(pst_HashedShmHandle pstHandle, unsigned long long ullVToKey, unsigned long long ullVAno, int iNum, int iMax, unsigned long long *pullBuilded)
{
	int iSeed;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	__Build_hashed_shm_key(pstHandle, ullVToKey, ullVAno, iNum, iMax, pullBuilded);

	//fprintf(stderr, "Builded %lx\n", *pullBuilded);

	iSeed = ((*pullBuilded) % iMax);
	(*pullBuilded) = (*pullBuilded) - iSeed + iNum;

	pstHandle->iNumBlock = iNum;
	pstHandle->iMaxBlock = iMax;

	//fprintf(stderr, "Seed %d Max %d Num %d Fixed %lx\n", \
			iSeed, iMax, iNum, *pullBuilded);

	return HASHED_SHM_SUCCESS_OK;
}

int Init_hashed_shm(pst_HashedShmHandle pstHandle)
{
	int iRetval;
	int iGrandTotal = 0;
	unsigned long long ullOverflowChecker;
	char szLockKey[128];

	errno = 0;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	/* Validate */
	if (pstHandle->uiHashNum == 0)
		pstHandle->uiHashNum = HASHED_SHM_DEFAULT_HASH_NUM;

	if (pstHandle->uiEntitySize == 0)
		return HASHED_SHM_ERR_NO_ENTITY_SIZE;

	if (pstHandle->uiEntityNum == 0)
		return HASHED_SHM_ERR_NO_ENTITY_NUM;

	if (pstHandle->kShmKey == 0)
		return HASHED_SHM_ERR_NO_SHMKEY;

	/* Calculate Memory */
	pstHandle->iShmTotalSize =
			ONE_ENTITY_SIZE(pstHandle) * pstHandle->uiEntityNum;
	ullOverflowChecker = 
			ONE_ENTITY_SIZE(pstHandle) * pstHandle->uiEntityNum;
	if (pstHandle->iShmTotalSize != ullOverflowChecker)
		return HASHED_SHM_ERR_OVERFLOW;

	pstHandle->iHashTotalSize = (sizeof(st_HashedShmTable) + __RESERVED) *
			pstHandle->uiHashNum;
	ullOverflowChecker = (sizeof(st_HashedShmTable) + __RESERVED) *
			pstHandle->uiHashNum;
	if (pstHandle->iHashTotalSize != ullOverflowChecker)
		return HASHED_SHM_ERR_OVERFLOW;

	iGrandTotal = sizeof(st_HashedShmHeader) + __RESERVED +
			pstHandle->iShmTotalSize + pstHandle->iHashTotalSize;
	ullOverflowChecker = sizeof(st_HashedShmHeader) + __RESERVED +
			pstHandle->iShmTotalSize + pstHandle->iHashTotalSize;
	if (iGrandTotal != ullOverflowChecker)
		return HASHED_SHM_ERR_OVERFLOW;

	/* Make Shared Memory */
	pstHandle->iShmId = shmget(pstHandle->kShmKey,
			iGrandTotal, SHMPERM | IPC_CREAT | IPC_EXCL);

	if (pstHandle->iShmId < 0) {
		if (errno == EEXIST) {
			errno = 0;

#ifdef __DEBUG_HASHSHM_INIT__
			fprintf(stderr, "EEXIST #1\n");
#endif

			pstHandle->iShmId = shmget(pstHandle->kShmKey,
					iGrandTotal, SHMPERM | IPC_CREAT);
	
			if (pstHandle->iShmId < 0)
				return HASHED_SHM_ERR_CANNOT_GET_SHMID_EXIST;

#ifdef __DEBUG_HASHSHM_INIT__
			fprintf(stderr, "EEXIST #2\n");
#endif
			iRetval = HASHED_SHM_SUCCESS_EXIST;
		} else {
#ifdef __DEBUG_HASHSHM_INIT__
			fprintf(stderr, "NO EEXIST\n");
#endif
			return HASHED_SHM_ERR_CANNOT_GET_SHMID_CREATED;
		}
	} else {
#ifdef __DEBUG_HASHSHM_INIT__
		fprintf(stderr, "CREATED\n");
#endif
		iRetval = HASHED_SHM_SUCCESS_CREATED;
	}

	pstHandle->pAttachAddr = shmat(pstHandle->iShmId, 0, 0);

	/* Make Semaphore Lock */
#ifdef __HASHSHM_LOCK__
	sprintf(szLockKey, "/tmp/lock.%x", pstHandle->kShmKey);
	pstHandle->iLock = init_lock(szLockKey);
	if (pstHandle->iLock <= 0)
		return HASHED_SHM_ERR_GETLOCK;
#endif

	if (pstHandle->pAttachAddr == (void *)-1) {
#ifdef __DEBUG_HASHSHM_INIT__
		fprintf(stderr, "SHMAT\n");
#endif
		return HASHED_SHM_ERR_CANNOT_ATTACH;
	}

	/* New Memory, Must be Clear */
	if (iRetval == HASHED_SHM_SUCCESS_CREATED) {
#ifdef __DEBUG_HASHSHM_INIT__
		fprintf(stderr, "NEW AND CLEAR\n");
#endif
		memset(pstHandle->pAttachAddr, 0, iGrandTotal);
	}

	/* Validate, from Previous Data */
	pstHandle->pstHeader = (pst_HashedShmHeader)pstHandle->pAttachAddr;
	
	if (iRetval == HASHED_SHM_SUCCESS_EXIST) {
		if (pstHandle->pstHeader->uiHashTotal != pstHandle->uiHashNum) {
			shmdt(pstHandle->pAttachAddr);
			pstHandle->pAttachAddr = NULL;
			return HASHED_SHM_ERR_NOT_MATCH_HASH_NUM;
		}

		if (pstHandle->pstHeader->uiEntitySize != pstHandle->uiEntitySize) {
			shmdt(pstHandle->pAttachAddr);
			pstHandle->pAttachAddr = NULL;
			return HASHED_SHM_ERR_NOT_MATCH_ENTITY_SIZE;
		}

		if (pstHandle->pstHeader->uiEntityTotal != pstHandle->uiEntityNum) {
			shmdt(pstHandle->pAttachAddr);
			pstHandle->pAttachAddr = NULL;
			return HASHED_SHM_ERR_NOT_MATCH_ENTITY_NUM;
		}
#ifdef __DEBUG_HASHSHM_INIT__
		fprintf(stderr, "OLD AND MATCH\n");
#endif
	} else if (iRetval == HASHED_SHM_SUCCESS_CREATED) {
		/* Fill Handle Header */
#ifdef __DEBUG_HASHSHM_INIT__
		fprintf(stderr, "NEW AND CLEAR 2\n");
#endif
		pstHandle->pstHeader->uiHashTotal = pstHandle->uiHashNum;
		pstHandle->pstHeader->uiEntitySize = pstHandle->uiEntitySize;
		pstHandle->pstHeader->uiEntityTotal = pstHandle->uiEntityNum;
		//pstHandle->pstHeader->uiEntityUsed = 0;
		atomic_set(&pstHandle->pstHeader->aiEntityUsed, 0);
	}

	/* Fill Handle */
	pstHandle->pstHashTable =
			(pstHandle->pAttachAddr + sizeof(st_HashedShmHeader) + __RESERVED);
	pstHandle->pEntityStartPos =
			(((char *)pstHandle->pstHashTable) + pstHandle->iHashTotalSize);

#ifdef __DEBUG_HASHSHM_INIT__
	fprintf(stderr, "ATTACH %llx\nHASH   %llx(%016lld)\nENTITY %llx(%016lld)\n",
			pstHandle->pAttachAddr,
			pstHandle->pstHashTable,
			*((unsigned long long *)(pstHandle->pstHashTable)),
			pstHandle->pEntityStartPos,
			*((unsigned long long *)(pstHandle->pEntityStartPos)));

	fprintf(stderr, "HASHTOTAL %d ENTITYTOTAL %d GRANDTOTAL %d\n",
			pstHandle->iHashTotalSize, pstHandle->iShmTotalSize, iGrandTotal);
#endif

	return iRetval;
}


int Get_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long ullKey, void **ppEntity)
{
	int iRetval;
	unsigned short usHashKey = SESSIONID_TO_HASHKEY(ullKey);
	pst_HashedShmTable pstHashTable;
	pst_HashedShmEntityHeader pstEntityHeader;
	unsigned int uiOffNext = 0;

	if (pstHandle == NULL || ullKey == 0 || ppEntity == NULL)
		return HASHED_SHM_ERR_ILLEGAL_REQUEST;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	/* get hash entry */
	iRetval = get_hash_entry(pstHandle, ullKey, &pstHashTable);
	if (iRetval == HASHED_SHM_SUCCESS_OK) {
		/* search child, get entry */
#ifdef __DEBUG_HASHSHM_GET__
			fprintf(stderr, "KEY %llx HASH %llx OFFEH %d\n",
						ullKey, pstHashTable, pstHashTable->uiOffEntityHeader);
#endif
		if (pstHashTable->uiOffEntityHeader == 0) {
			return HASHED_SHM_ERR_NOT_FOUND;
		}

		for (pstEntityHeader = OTE(pstHashTable->uiOffEntityHeader);
			 pstEntityHeader;
			 pstEntityHeader = OTE(pstEntityHeader->uiOffNext)) {

			if (IS_EMPTY_OFFSET(pstHandle, pstEntityHeader)) {
				return HASHED_SHM_ERR_NOT_FOUND;
			}
#if 0
			if (SESSIONID_TO_HASHKEY(pstEntityHeader->ullKey) !=
				SESSIONID_TO_HASHKEY(ullKey)) {

				fprintf(stderr, "== KIKU GET HeaderKey %llx ullKey %llx\n",
						pstEntityHeader->ullKey, ullKey);
			}
#endif
#ifdef __DEBUG_HASHSHM_GET__
			fprintf(stderr, "EH %llx HASH %llx(offset %llx)\n"
							"HeaderKey %llx ullKey %llx\n",
						pstEntityHeader, pstHashTable,
						pstHashTable - pstHandle->pstHashTable,
						pstEntityHeader->ullKey, ullKey);

			if (uiOffNext != 0 && uiOffNext == pstEntityHeader->uiOffNext)
				exit(-1);
#else
			if (uiOffNext != 0 && uiOffNext == pstEntityHeader->uiOffNext)
				return HASHED_SHM_ERR_DATA_CORRUPTION;
#endif

			if (pstEntityHeader->ullKey == ullKey) {
				*ppEntity = ENTITY_HEADER_TO_ENTITY(pstEntityHeader);
				return HASHED_SHM_SUCCESS_OK;
			}
			if (pstEntityHeader->uiOffNext == 0) {
				break;
			}

			uiOffNext = pstEntityHeader->uiOffNext;
		}
	} else {
		return iRetval;
	}
	
	return HASHED_SHM_ERR_NOT_FOUND;
}


int New_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long ullKey, void **ppEntity)
{
	int iRetval;
	//unsigned int uiEntityNext = pstHandle->pstHeader->uiEntityNext;
	unsigned int uiEntityNext =
			MY_BLOCK(pstHandle, pstHandle->pstHeader->uiEntityNext);
	unsigned int uiPrevEntityNext = uiEntityNext;
	pst_HashedShmEntityHeader pstEntityHeader, pstHashedEntityHeader;
	pst_HashedShmTable pstHashRoot;
	unsigned int uiOffNext = 0;
#ifdef __DEBUG_HASHSHM_NEW__
	int depth = 0;
#endif

	if (pstHandle == NULL || ullKey == 0 || ppEntity == NULL)
		return HASHED_SHM_ERR_ILLEGAL_REQUEST;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	/* initialize */
	*ppEntity = NULL;

	/* get hash entry */
	iRetval = get_hash_entry(pstHandle, ullKey, &pstHashRoot);
	if (iRetval != HASHED_SHM_SUCCESS_OK) return iRetval;

	/* search empty entity */
	if (uiEntityNext >= pstHandle->uiEntityNum) {
		uiPrevEntityNext = uiEntityNext = MY_BLOCK(pstHandle, 0);
	}

#ifdef __HASHSHM_LOCK__
	file_lock(pstHandle->iLock);
#endif
	pstEntityHeader = ENTITY_HEADER(pstHandle, uiEntityNext);
#ifdef __DEBUG_HASHSHM_NEW__
    fprintf(stderr, "First EntityHeader %x\n", pstEntityHeader);
#endif
	//usleep(100000);
	while (pstEntityHeader->ullKey != 0) {
		//uiEntityNext++;
		uiEntityNext = NEXT_BLOCK(pstHandle, uiEntityNext);

		if (uiEntityNext >= pstHandle->uiEntityNum)
			uiEntityNext = MY_BLOCK(pstHandle, 0);

		if (uiPrevEntityNext == uiEntityNext) {
#if 0
//#ifdef __DEBUG_HASHSHM_NEW__
			fprintf(stderr, "FULL? PREV %d, ENEXT %d, NUM %d\n",
				uiPrevEntityNext, uiEntityNext, pstHandle->uiEntityNum);
#endif
			return HASHED_SHM_ERR_MEMORY_FULL;
		}

#ifdef __DEBUG_HASHSHM_NEW__
//#if 1
		fprintf(stderr, "Empty LOOP %u NEXT %u\n",
				uiPrevEntityNext, uiEntityNext);
#endif

		pstEntityHeader = ENTITY_HEADER(pstHandle, uiEntityNext);
	}
	pstEntityHeader->ullKey = ullKey;
#ifdef __HASHSHM_LOCK__
	file_unlock(pstHandle->iLock);
#endif

//#ifdef __DEBUG_HASHSHM_NEW__
#if 0
	fprintf(stderr, "PN %d EN %d TOTAL ENTITY %d "
					"pstHashRoot %llx OffEntityHeader %u\n",
			uiPrevEntityNext, uiEntityNext, pstHandle->uiEntityNum,
			pstHashRoot, pstHashRoot->uiOffEntityHeader);
#endif

	/* try insert hash entry */
	if (pstHashRoot->uiOffEntityHeader == 0) {
		pstHashRoot->uiOffEntityHeader = ETO(pstEntityHeader);
#ifdef __DEBUG_HASHSHM_NEW__
		fprintf(stderr, "Header 1st\n");
#endif
	} else {
		/* duplicate check *, no need first entry
		if (pstHashRoot->uiOffEntityHeader->ullKey == ullKey)
			return HASHED_SHM_ERR_ALREADY_EXIST;
		*/
		pstHandle->pstHeader->uiEntityDuple++;

		for (pstHashedEntityHeader = OTE(pstHashRoot->uiOffEntityHeader);
			 pstHashedEntityHeader->uiOffNext;
			 pstHashedEntityHeader = OTE(pstHashedEntityHeader->uiOffNext)) {

#if 1
			if (SESSIONID_TO_HASHKEY(pstHashedEntityHeader->ullKey) !=
				SESSIONID_TO_HASHKEY(ullKey)) {

				fprintf(stderr, "== KIKU NEW HeaderKey %llx ullKey %llx\n",
						pstHashedEntityHeader->ullKey, ullKey);
			}
#endif
#ifdef __DEBUG_HASHSHM_NEW__
			fprintf(stderr, "Header LOOP:%d %llx NEXT %u\n", depth,
				pstHashedEntityHeader, pstHashedEntityHeader->uiOffNext);

			depth++;

			if (uiOffNext == pstHashedEntityHeader->uiOffNext)
				exit(-1);
#else
			if (uiOffNext == pstHashedEntityHeader->uiOffNext) {
				pstEntityHeader->ullKey = 0;
				return HASHED_SHM_ERR_DATA_CORRUPTION;
			}
#endif

			/* cannot be this case, must be */
			if (pstHashedEntityHeader == NULL) {
				pstEntityHeader->ullKey = 0;
				return HASHED_SHM_ERR_DATA_CORRUPTION;
			}

			/* duplicate check, middle entry */
			if (pstHashedEntityHeader->ullKey == ullKey) {
				pstEntityHeader->ullKey = 0;
				return HASHED_SHM_ERR_ALREADY_EXIST;
			}

			uiOffNext = pstHashedEntityHeader->uiOffNext;
		}

		/* duplicate check, last entry */
		if (pstHashedEntityHeader->ullKey == ullKey) {
			pstEntityHeader->ullKey = 0;
			return HASHED_SHM_ERR_ALREADY_EXIST;
		}

		pstHashedEntityHeader->uiOffNext = ETO(pstEntityHeader);
	}

	/* insert done. we have fixed data */
	/* fill data */
	pstEntityHeader->ullKey = ullKey;
	*ppEntity = ENTITY(pstHandle, uiEntityNext);

	/* clear assigned memory */
	memset(*ppEntity, 0, pstHandle->uiEntitySize);

	/* Increase Counter */
	uiEntityNext++;

	pstHashRoot->uiChildNum ++;
	//pstHandle->pstHeader->uiEntityUsed ++;
	atomic_inc(&pstHandle->pstHeader->aiEntityUsed);
	pstHandle->pstHeader->uiEntityNext =
			(uiEntityNext >= pstHandle->uiEntityNum ? 0 : uiEntityNext);

#ifdef __DEBUG_HASHSHM_NEW__
	fprintf(stderr, "NUMBER %d MEMORY %x-%x SIZE OE %d ES %d\n",
			uiEntityNext, pstHandle->pAttachAddr, *ppEntity,
			ONE_ENTITY_SIZE(pstHandle), pstHandle->uiEntitySize);
#endif

	return HASHED_SHM_SUCCESS_OK;
}


int Release_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long ullKey, void *pEntity)
{
	int iRetval;
	pst_HashedShmEntityHeader pstEntityHeader = NULL;
	pst_HashedShmEntityHeader pstHashedEntityHeader, pstEntityHeaderPrev = NULL;
	pst_HashedShmTable pstHashRoot;
	unsigned int uiOffNext = 0;

	if (pstHandle == NULL)
		return HASHED_SHM_ERR_ILLEGAL_REQUEST;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	/* get hash entry */
	iRetval = get_hash_entry(pstHandle, ullKey, &pstHashRoot);
	if (iRetval != HASHED_SHM_SUCCESS_OK)
		return iRetval;

	/* ullKey, pEntity both unique, both cat reach st_HashedShmEntityHeader */
#if 0
	if (pEntity != NULL) {
		/* pEntity, more easy (low cost) */
#ifdef __DEBUG_HASHSHM_REMOVE__
		fprintf(stderr, "Release by Entity: %llx\n", pEntity);
#endif
		pstEntityHeader = ENTITY_TO_ENTITY_HEADER(pEntity);

		if (ullKey > 0 && pstEntityHeader->ullKey != ullKey)
			return HASHED_SHM_ERR_NOT_MATCH_KEY;
	} else
#endif
	if (ullKey > 0) {
#ifdef __DEBUG_HASHSHM_REMOVE__
		int n = 0;
		fprintf(stderr, "Release by Key: %llx\n", ullKey);
#endif
		if (pstHashRoot->uiOffEntityHeader == 0)
			return HASHED_SHM_ERR_NOT_FOUND;

		pstEntityHeader = NULL;
		/* ullKey, must search hash table and linked list */
		for (pstHashedEntityHeader = OTE(pstHashRoot->uiOffEntityHeader);
			 pstHashedEntityHeader;
			 pstHashedEntityHeader = OTE(pstHashedEntityHeader->uiOffNext)) {

#if 1
			if (SESSIONID_TO_HASHKEY(pstHashedEntityHeader->ullKey) !=
				SESSIONID_TO_HASHKEY(ullKey)) {

				fprintf(stderr, "== KIKU REMOVE HeaderKey %llx ullKey %llx\n",
						pstHashedEntityHeader->ullKey, ullKey);
			}
#endif
#ifdef __DEBUG_HASHSHM_REMOVE__
			fprintf(stderr, "HEADER %x H-KEY %llx\n",
					pstHashedEntityHeader, pstHashedEntityHeader->ullKey);
			n++;
			if (n > 20) {
				fprintf(stderr, "OVER LIMIT!!\n");
				return HASHED_SHM_ERR_NOT_FOUND;
			}
#endif
			if (uiOffNext != 0 && uiOffNext == pstHashedEntityHeader->uiOffNext)
				return HASHED_SHM_ERR_DATA_CORRUPTION;

			if (pstHashedEntityHeader->ullKey == ullKey) {
				pstEntityHeader = pstHashedEntityHeader;
				break;
			}

			if (pstHashedEntityHeader->uiOffNext) {
				pstEntityHeaderPrev = pstHashedEntityHeader;
			} else if (pstHashedEntityHeader->uiOffNext == 0) {
				pstEntityHeader = NULL;
				break;
			}

			uiOffNext = pstHashedEntityHeader->uiOffNext;
		}

		if (pstEntityHeader == NULL)
			return HASHED_SHM_ERR_NOT_FOUND;

		pEntity = ENTITY_HEADER_TO_ENTITY(pstEntityHeader);
	} else {
		/* noway to found EntityHeader */
		return HASHED_SHM_ERR_ILLEGAL_REQUEST;
	}

	/* remove data, first from linked list */
	if (pstHashRoot->uiOffEntityHeader == ETO(pstEntityHeader)) {
#ifdef __DEBUG_HASHSHM_REMOVE__
		fprintf(stderr, "ROOT\n");
#endif
		pstHashRoot->uiOffEntityHeader = pstEntityHeader->uiOffNext;
	} else {
		if (pstEntityHeaderPrev &&
			pstEntityHeaderPrev->uiOffNext == ETO(pstEntityHeader)) {

#ifdef __DEBUG_HASHSHM_REMOVE__
			fprintf(stderr, "PREV %x, Right to Next\n", pstEntityHeaderPrev);
#endif
			pstEntityHeaderPrev->uiOffNext = pstEntityHeader->uiOffNext;
		} else {
			uiOffNext = 0;
			for (pstEntityHeaderPrev = OTE(pstHashRoot->uiOffEntityHeader);
				 pstEntityHeaderPrev &&
					pstEntityHeaderPrev->uiOffNext != ETO(pstEntityHeader); 
				pstEntityHeaderPrev = OTE(pstEntityHeaderPrev->uiOffNext)) {

#ifdef __DEBUG_HASHSHM_REMOVE__
				fprintf(stderr, "PREV %x(%d), Searching..\n",
						pstEntityHeaderPrev, pstHashRoot->uiOffEntityHeader);
#endif
				if (uiOffNext != 0 &&
					uiOffNext == pstEntityHeaderPrev->uiOffNext)
					return HASHED_SHM_ERR_DATA_CORRUPTION;

				uiOffNext = pstEntityHeaderPrev->uiOffNext;
			}

			if (pstEntityHeaderPrev->uiOffNext == ETO(pstEntityHeader)) {
				pstEntityHeaderPrev->uiOffNext = pstEntityHeader->uiOffNext;
			} else {
				return HASHED_SHM_ERR_NOT_FOUND_TO_DELETE;
			}
		}
	}	

	/* remove data, clear EntityHeader */
	pstEntityHeader->ullKey = 0;
	pstEntityHeader->uiOffNext = 0;

	/* clear used entity */
	memset(pEntity, 0, pstHandle->uiEntitySize);

	/* Decrease Counter */
	if (pstHashRoot->uiChildNum > 0) {
		pstHashRoot->uiChildNum --;
	}
	atomic_dec(&(pstHandle->pstHeader->aiEntityUsed));
#if 0
	if (pstHandle->pstHeader->uiEntityUsed > 0) {
		pstHandle->pstHeader->uiEntityUsed --;
	}
#endif

	return HASHED_SHM_SUCCESS_OK;
}

int Browse_hashed_shm(pst_HashedShmHandle pstHandle, unsigned long long *pullKey, void **ppEntity)
{
	pst_HashedShmEntityHeader pstEntityHeaderBrowse;
	pst_HashedShmTable pstHashRoot;
	int iRetval;
	//int iEntityBrowsePrev;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	/* if clear signal */
	if (pullKey == NULL || ppEntity == NULL) {
		pstHandle->uiOffEntityHeaderBrowse = 0;
		pstHandle->iEntityBrowseCurrent = 0;

		return HASHED_SHM_SUCCESS_OK;
	}
	/* step 1: Find Current Entry */
	/* step 1-1: First Time */
	if (pstHandle->uiOffEntityHeaderBrowse == 0) {
		iRetval =
			browse_next_entry(pstHandle, &pstEntityHeaderBrowse);

		if (iRetval < 0)
			return iRetval;

		pstHandle->uiOffEntityHeaderBrowse = ETO(pstEntityHeaderBrowse);
	} else {
		pstEntityHeaderBrowse = OTE(pstHandle->uiOffEntityHeaderBrowse);
	}

	/* step 1-2: Fill data to return */
	*ppEntity = ENTITY_HEADER_TO_ENTITY(pstEntityHeaderBrowse);
	*pullKey = pstEntityHeaderBrowse->ullKey;

	/* step 2: Find Next Entry */
	//iEntityBrowsePrev = pstHandle->iEntityBrowseCurrent;

	iRetval = browse_next_entry(pstHandle, &pstEntityHeaderBrowse);
	if (pstEntityHeaderBrowse)
		pstHandle->uiOffEntityHeaderBrowse = ETO(pstEntityHeaderBrowse);

	if (iRetval < 0) {
		if (iRetval == HASHED_SHM_ERR_NOT_FOUND)
			return HASHED_SHM_SUCCESS_OK;
		else
			return iRetval;
	} else
		return HASHED_SHM_SUCCESS_CONTINUE;
	
	/* WARNING: cannot reach to here */
	return HASHED_SHM_ERR_DATA_CORRUPTION;
}

int Delete_hashed_shm(pst_HashedShmHandle pstHandle)
{
	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	/* Detech */
	shmdt(pstHandle->pAttachAddr);

	/* clear data */	
	pstHandle->pstHashTable = NULL;
	pstHandle->pEntityStartPos = NULL;
	pstHandle->pAttachAddr = NULL;
	pstHandle->pstHeader = NULL;

	//pstHandle->iShmId = 0;
	pstHandle->iShmTotalSize = 0;
	pstHandle->iHashTotalSize = 0;

	return HASHED_SHM_SUCCESS_OK;
}

int Purge_hashed_shm(pst_HashedShmHandle pstHandle)
{
	int iRetval;

	if (pstHandle->uiHashType != HASHED_SHM_ORIGINAL)
		return HASHED_SHM_ERR_TYPE_NOT_MATCH;

	if (pstHandle->pAttachAddr)
		Delete_hashed_shm(pstHandle);

	if (pstHandle->iShmId <= 0) {
		return HASHED_SHM_ERR_ILLEGAL_REQUEST;
	}

	iRetval = shmctl(pstHandle->iShmId, IPC_RMID, NULL);
	if (iRetval < 0) {
		return HASHED_SHM_ERR_RMID_FAILED;
		//perror("Purge_hashed_shm");
	}

	return HASHED_SHM_SUCCESS_OK;
}

const char *pszHashedShmMsg[] = {
	"(0) *ZERO*",
	/* 1 */
	"(1) Entity Size not Inputed",
	"(2) Entity Number not Inputed",
	"(3) Shared Memory Key not Inputed",
	"(4) shmget failed: with IPC_EXCL",
	/* 5 */
	"(5) shmget failed: without IPC_EXCL",
	"(6) shmat failed",
	"(7) Overflow",
	"(8) Detail maybe changed: Entity Size",
	"(9) Detail maybe changed: Entity Number",
	/* 10 */
	"(10) Detail maybe changed: Hash Table Number",
	"(11) Hash Table Number over Limit",
	"(12) Not Found",
	"(13) Illegal Request",
	"(14) Key not Match",
	/* 15 */
	"(15) Not Found, to Delete",
	"(16) shmctl IPC_RMID failed",
	"(17) Already Exist",
	"(18) Data Corruption",
	"(19) Memory Full",
	/* 20 */
	"(20) Error While Locking Operation",
	"(21) 4B Library Header Inputed"
};

const char *Msg_hashed_shm(int iRetval)
{
	int n = sizeof(pszHashedShmMsg) / sizeof(const char *);
	int i;

	if (iRetval < 0) iRetval = -iRetval;

	if (iRetval >= n)
		return NULL;

	return pszHashedShmMsg[iRetval];
}


#if 0
#define __MAX_CPU       16

int prevent_cpu(int iNum1, int iNum2)
{
	cpu_set_t cmask;
	unsigned long len = sizeof(cmask);
	int i;

	__CPU_ZERO(&cmask);

	for (i = 0; i < __MAX_CPU; i++) {
		if ((iNum1 >= 0 && iNum1 != i) && (iNum2 >= 0 && iNum2 != i)) {
			__CPU_SET(i, &cmask);
		}
	}

	if (sched_setaffinity(0, len, &cmask)) {
		return -1;
	}

	return 0;
}


int occupy_cpu(int iNum)
{
	cpu_set_t cmask;
	unsigned long len = sizeof(cmask);
	int i;

	__CPU_ZERO(&cmask);
	__CPU_SET(iNum, &cmask);

	if (sched_setaffinity(0, len, &cmask)) {
		return -1;
	}

	return 0;
}
#endif

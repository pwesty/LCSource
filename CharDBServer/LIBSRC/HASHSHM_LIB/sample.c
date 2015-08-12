#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Private Library Headers */
#include <libhashshm.h>
#include <utillib.h>
#include <server_sock.h>
#include <server_define.h>


#define SAMPLE_KEY			50000
//#define SAMPLE_KEY		20
#define SAME_KEY			5
#define SAMPLE_NUM			(SAMPLE_KEY * SAME_KEY)
#define MY_SHARED_KEY		0x10406700
#define MY_SHARED_KEY_4B	0x10406701

#define FAIL_KEY_START		10
#define FAIL_KEY_END		15
#define FAIL_SUB_START		2
#define FAIL_SUB_END		4
#define VECTOR				1000000

//#define __TEST_SHM_ALL_STEP
#define __TEST_4BSHM_ALL_STEP


struct sample_data {
	unsigned long long key;
	unsigned long long data;
	char reserved[202];
};

struct sample_data_4b {
	st_HashedShmKey stKey;
	unsigned long long data;
	char reserved[20];
};


//#define OUTPUT_STEP_1
//#define OUTPUT_STEP_2
//#define OUTPUT_STEP_3
//#define OUTPUT_STEP_4
//#define OUTPUT_STEP_5

main()
{
	st_HashedShmHandle stHandle;		
	int retval, i, j;
	int success, success_nf, failed;
	//unsigned long long ullKey = 4839117799609631318;
	//unsigned long long ullKey = 0x4328000000008270LL;
	unsigned long long ullKey = 0x432800000000826cLL;
	pst_SOCK_INFO   pstSOCKINFO;
	struct timeval tv;
	int ip = 0x4328;
	short port = 23481;

#ifdef __TEST_SHM_ALL_STEP
	Init_hashed_shm_handle(&stHandle, 20000, sizeof(struct sample_data),
			20000, MY_SHARED_KEY);
#endif
#ifdef __TEST_4BSHM_ALL_STEP
	Init_hashed_shm_handle_4b(&stHandle, 500000, sizeof(struct sample_data_4b),
			500000, MY_SHARED_KEY_4B);
#endif
#if 0
	Init_hashed_shm_handle(&stHandle, MAX_SOCKSHM_CNT, sizeof(st_SOCK_INFO),
            MAX_SOCKSHM_CNT, S_SSHM_HASHLIB);
#endif
#if 0
	Init_hashed_shm_handle(&stHandle, MAX_CONN_NUM,
			sizeof(st_CONN_INFO),
			MAX_CONN_NUM, S_SSHM_CLIENT_INFO);
#endif

#ifdef __TEST_SHM_ALL_STEP
	retval = Init_hashed_shm(&stHandle);
	if (retval < 0) {
		fprintf(stderr, "Init_hashed_shm: %d\n", retval);
		exit(-1);
	}
#endif
#ifdef __TEST_4BSHM_ALL_STEP
	retval = Init_hashed_shm_4b(&stHandle);
	if (retval < 0) {
		fprintf(stderr, "Init_hashed_shm: %d\n", retval);
		exit(-1);
	}
#endif

#if 0
	Build_hashed_shm_key_seed(&stHandle, 0x432800000000826cLL, 0x1234567812345678LL, 0, 10, &ullKey);
	Build_hashed_shm_key_seed(&stHandle, 0x432800000000826cLL, 0x1234567812345678LL, 1, 10, &ullKey);

	gettimeofday(&tv, NULL);

	Build_hashed_shm_key_seed(&stHandle,
		(unsigned long long)ip ^ (unsigned long long)port ^ tv.tv_usec,
		((unsigned long long)port ^ (unsigned long long)tv.tv_sec) ^ (((unsigned long long)ip ^ (unsigned long long)tv.tv_usec) << 32),
		1, 10, &ullKey);
	
	fprintf(stderr, "KEY %lx, HASH %lx(%d) - %d\n\n", ullKey,
			SESSIONID_TO_HASHKEY(ullKey),
			SESSIONID_TO_HASHKEY(ullKey), SESSIONID_TO_HASHKEY(ullKey) % 10);

	port += 10;
	gettimeofday(&tv, NULL);

	Build_hashed_shm_key_seed(&stHandle,
		(unsigned long long)ip ^ (unsigned long long)port ^ tv.tv_usec,
		(unsigned long long)port,
		5, 10, &ullKey);

	fprintf(stderr, "KEY %lx, HASH %lx(%d) - %d\n", ullKey,
			SESSIONID_TO_HASHKEY(ullKey),
			SESSIONID_TO_HASHKEY(ullKey), SESSIONID_TO_HASHKEY(ullKey) % 10);

#endif

#ifdef __TEST_SHM_ALL_STEP
	/* step 1: create test */
	fprintf(stderr, "STEP 1 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * (SAME_KEY / 2), 0, SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;

	for (i = 0; i < SAMPLE_KEY; i++) {
		struct sample_data *entry;
		
		for (j = 1; j <= (SAME_KEY / 2); j++) {
			//unsigned long long key = HASHKEY_TO_SESSIONID(i) | j;
			unsigned long long key;
			Build_hashed_shm_key(&stHandle, i, j, &key);

			retval = New_hashed_shm(&stHandle, key, (void **)&entry);
			if (retval < 0) {
#ifdef OUTPUT_STEP_1
				fprintf(stderr, "INPUTED:%5d,%3d(%016llx) New_hashed_shm: %d\n",
						i, j, key, retval);
#endif
				failed++;
			} else {
				//fprintf(stderr, "INPUTED:%5d (%x)\n", i, (void *)entry);

				entry->key = key;
				entry->data = i;

#ifdef OUTPUT_STEP_1
				fprintf(stderr, "INPUTED:%5d,%3d(%016llx) (%x) %08x:%08x\n",
						i, j, key, (void *)entry, entry->key, entry->data);
#endif

				success++;
			}
		}
#if 0
		if (i > 2) {
		Purge_hashed_shm(&stHandle);
		exit(-1);
		}
#endif
	}
	fprintf(stderr, "STEP 1 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));

	/* step 2: get test */
	fprintf(stderr, "STEP 2 GOAL   : Success %d Failed %d Entity %d\n",
			0,
			(FAIL_KEY_END - FAIL_KEY_START) * (FAIL_SUB_END - FAIL_SUB_START),
			SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	for (i = FAIL_KEY_START; i < FAIL_KEY_END; i++) {
	//for (i = 10; i < 15; i++) {
		for (j = FAIL_SUB_START; j < FAIL_SUB_END; j++) {
			//unsigned long long key = HASHKEY_TO_SESSIONID(i) | j;
			unsigned long long key;
			Build_hashed_shm_key(&stHandle, i + VECTOR, j + VECTOR, &key);

			retval = Release_hashed_shm(&stHandle, key, NULL);
			if (retval < 0) {
#ifdef OUTPUT_STEP_2
				fprintf(stderr, "Release_hashed_shm: %d\n", retval);
#endif
				failed++;
			} else {
				success++;
			}
		}
	}
	fprintf(stderr, "STEP 2 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));


#if 0
	Purge_hashed_shm(&stHandle);
	exit(-1);
#endif

	/* step 3: additional create test */
	/* target: all success */
	fprintf(stderr, "STEP 3 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * ((SAME_KEY / 2) + 1), 0,
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	for (i = 0; i < SAMPLE_KEY; i++) {
		struct sample_data *entry;
		
		for (j = (SAME_KEY / 2) + 1; j <= SAME_KEY; j++) {
			//unsigned long long key = HASHKEY_TO_SESSIONID(i) | j;
			unsigned long long key;
			Build_hashed_shm_key(&stHandle, i + VECTOR, j + VECTOR, &key);

			retval = New_hashed_shm(&stHandle, key, (void **)&entry);
			if (retval < 0) {
#ifdef OUTPUT_STEP_3
				fprintf(stderr, "New_hashed_shm: %d\n", retval);
#endif
				failed++;
			} else {
				//fprintf(stderr, "INPUTED:%5d (%x)\n", i, (void *)entry);

				entry->key = key;
				entry->data = i;

#ifdef OUTPUT_STEP_3
				fprintf(stderr, "INPUTED:%5d,%3d(%016lx) (%x) %08x:%08x\n",
						i, j, key, (void *)entry, entry->key, entry->data);
#endif
				success++;
			}
		}
	}
	fprintf(stderr, "STEP 3 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));
	//fprintf(stderr, "STEP 3 SUMMERY: Success %d Failed %d\n", success, failed);
	//Purge_hashed_shm(&stHandle);
	//exit(-1);

	/* step 4: get test */
	/* target: all success */
	fprintf(stderr, "STEP 4 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * ((SAME_KEY / 2) + 1), SAMPLE_KEY * (SAME_KEY / 2),
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	for (i = 0; i < SAMPLE_KEY; i++) {
		struct sample_data *entry;

		for (j = 1; j <= SAME_KEY; j++) {
			//unsigned long long key = HASHKEY_TO_SESSIONID(i) | j;
			unsigned long long key;
			Build_hashed_shm_key(&stHandle, i + VECTOR, j + VECTOR, &key);

			retval = Get_hashed_shm(&stHandle, key, (void **)&entry);
			if (retval < 0) {
#ifdef OUTPUT_STEP_4
				fprintf(stderr, "Get_hashed_shm: %d, %s\n", retval,
						Msg_hashed_shm(retval));
#endif
				failed++;
			} else {
#ifdef OUTPUT_STEP_4
				fprintf(stderr, "GETTED: %5d,%3d(%016lx) (%x) %08x:%08x\n",
						i, j, key, (void *)entry, entry->key, entry->data);
#endif
				success++;
			}
		}
	}
	fprintf(stderr, "STEP 4 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));
	//fprintf(stderr, "STEP 4 SUMMERY: Success %d Failed %d\n", success, failed);

	/* step 5: browse test */
	fprintf(stderr, "STEP 5 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * SAME_KEY / 2, 0,
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * SAME_KEY / 2);

	success = 0;
	failed = 0;
	retval = 0;
	Browse_hashed_shm_clear(&stHandle);
	while (retval >= 0 && retval != HASHED_SHM_SUCCESS_OK) {
		struct sample_data *entry;
		unsigned long long key;

		retval = Browse_hashed_shm(&stHandle, &key, (void **)&entry);
		if (retval < 0) {
#ifdef OUTPUT_STEP_5
			fprintf(stderr, "Browse_hashed_shm: %d\n", retval);
#endif
			failed++;
		} else {
#ifdef OUTPUT_STEP_5
			fprintf(stderr, "BROWSE: %016lx (%x) %08x:%08x\n",
					key, (void *)entry, entry->key, entry->data);
#endif
			success++;
		}
	}
	fprintf(stderr, "STEP 5 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));
	//fprintf(stderr, "STEP 5 SUMMERY: Success %d Failed %d\n", success, failed);

	Purge_hashed_shm(&stHandle);
#endif // __TEST_SHM_STEP_14

#if 0
	/* step 5 */
	success = 0; success_nf = 0;
	failed = 0;
	retval = 0;
	Browse_hashed_shm_clear(&stHandle);
	while (retval >= 0 && retval != HASHED_SHM_SUCCESS_OK) {
		struct sample_data *entry;
		unsigned long long key;

		retval = Browse_hashed_shm(&stHandle, &key, (void **)&entry);
		if (retval < 0) {
#ifdef OUTPUT_STEP_5
			fprintf(stderr, "Browse_hashed_shm: %d\n", retval);
#endif
			failed++;
		} else {
			int subret;
			subret = Get_hashed_shm(&stHandle, key, (void **)&entry);
			if (subret < 0) {
				fprintf(stderr, "==== NOT FOUND: BROWSE: %016lx\n", key);
				success_nf++;
			} else {
#ifdef OUTPUT_STEP_5
				fprintf(stderr, "BROWSE: %016lx (%x) %08x:%08x\n",
						key, (void *)entry, entry->key, entry->data);
#endif
				;
			}
			success++;
		}
	}
	fprintf(stderr, "STEP 5 SUMMERY: Success %d/%d Failed %d Entity %d\n",
			success_nf, success, failed,
			atomic_read(&(stHandle.pstHeader->aiEntityUsed)));
#endif

#if 0
	for (i = 0; i < 21; i++) {
		fprintf(stderr, "ERR SAMPLE : %s, %s\n",
				Msg_hashed_shm(i), Msg_hashed_shm(-i));
	}
#endif

#if 0
	retval = New_hashed_shm(&stHandle, ullKey, (void **)&pstSOCKINFO);
	fprintf(stderr, "New_hashed_shm: %d\n", retval);
#endif

	//Release_hashed_shm(&stHandle, ullKey, NULL);

#ifdef __TEST_4BSHM_ALL_STEP
	/* step 1: create test */
	fprintf(stderr, "STEP 1 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * (SAME_KEY / 2), 0, SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	for (i = 0; i < SAMPLE_KEY; i++) {
		struct sample_data_4b *entry;
		
		for (j = 1; j <= (SAME_KEY / 2); j++) {
			st_HashedShmKey stKey;

			fprintf(stderr, "\rKey %d Duple %d\r", i, j);
			Build_hashed_shm_key_4b(&stHandle, i, j, &stKey);
			retval = New_hashed_shm_4b(&stHandle, &stKey, (void **)&entry);
			if (retval < 0) {
#ifdef OUTPUT_STEP_1
				fprintf(stderr, "INPUTED:%5d,%3d(%016llx:%016llx) "
						"New_hashed_shm: %d\n",
						i, j, stKey.__t, stKey.__b, retval);
#endif
				failed++;
			} else {
				entry->stKey = stKey;
				entry->data = i+1;

#ifdef OUTPUT_STEP_1
				fprintf(stderr, "INPUTED:%5d,%3d(%016llx:%016llx) "
						"(%x) (%016llx:%016llx) %08x\n",
						i, j, stKey.__t, stKey.__b,
						(void *)entry,
						entry->stKey.__t, entry->stKey.__b,
						entry->data);
#endif

				success++;
			}
		}
	}
	fprintf(stderr, "STEP 1 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));

	/* step 2: get test */
	fprintf(stderr, "STEP 2 GOAL   : Success %d Failed %d Entity %d\n",
			0,
			(FAIL_KEY_END - FAIL_KEY_START) * (FAIL_SUB_END - FAIL_SUB_START),
			SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	for (i = FAIL_KEY_START; i < FAIL_KEY_END; i++) {
		for (j = FAIL_SUB_START; j < FAIL_SUB_END; j++) {
			st_HashedShmKey stKey;

			Build_hashed_shm_key_4b(&stHandle, i + VECTOR, j + VECTOR, &stKey);
			retval = Release_hashed_shm_4b(&stHandle, &stKey, NULL);
			if (retval < 0) {
				failed++;
			} else {
				success++;
			}
		}
	}
	fprintf(stderr, "STEP 2 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));

	/* step 3: additional create test */
	/* target: all success */
	fprintf(stderr, "STEP 3 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * ((SAME_KEY / 2) + 1), 0,
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	for (i = 0; i < SAMPLE_KEY; i++) {
		struct sample_data_4b *entry;
		
		for (j = (SAME_KEY / 2) + 1; j <= SAME_KEY; j++) {
			st_HashedShmKey stKey;
			Build_hashed_shm_key_4b(&stHandle, i + VECTOR, j + VECTOR, &stKey);

			fprintf(stderr, "\rKey %d Duple %d retval %d\r", i, j, retval);
			retval = New_hashed_shm_4b(&stHandle, &stKey, (void **)&entry);
			if (retval < 0) {
#ifdef OUTPUT_STEP_3
				fprintf(stderr, "New_hashed_shm: %d\n", retval);
#endif
				failed++;
			} else {
				entry->stKey = stKey;
				entry->data = i;

#ifdef OUTPUT_STEP_3
				fprintf(stderr, "INPUTED:%5d,%3d(%016llx:%016llx) "
						"(%x) (%016llx:%016llx) %08x\n",
						i, j, stKey.__t, stKey.__b,
						(void *)entry,
						entry->stKey.__t, entry->stKey.__b,
						entry->data);
#endif
				success++;
			}
		}
	}
	fprintf(stderr, "STEP 3 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));

	/* step 4: get test */
	/* target: all success */
	fprintf(stderr, "STEP 4 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * ((SAME_KEY / 2) + 1), SAMPLE_KEY * (SAME_KEY / 2),
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	for (i = 0; i < SAMPLE_KEY; i++) {
		struct sample_data_4b *entry;

		for (j = 1; j <= SAME_KEY; j++) {
			st_HashedShmKey stKey;
			Build_hashed_shm_key_4b(&stHandle, i + VECTOR, j + VECTOR, &stKey);

			retval = Get_hashed_shm_4b(&stHandle, &stKey, (void **)&entry);
			if (retval < 0) {
				failed++;
#ifdef OUTPUT_STEP_4
				fprintf(stderr, "FAILED: %5d,%3d(%016lx:%016lx)\n",
						i, j,
						stKey.__t, stKey.__b);
#endif
			} else {
				success++;
#ifdef OUTPUT_STEP_4
				fprintf(stderr, "GETTED: %5d,%3d(%016lx:%016lx) "
						"%x,%5d(%016lx:%016lx)\n",
						i, j, stKey.__t, stKey.__b,
						(void *)entry,
						entry->data,
						entry->stKey.__t,
						entry->stKey.__b);
#endif
			}
		}
	}
	fprintf(stderr, "STEP 4 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));

	/* step 5 */
	/* step 5: browse test */
	fprintf(stderr, "STEP 5 GOAL   : Success %d Failed %d Entity %d\n",
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * (SAME_KEY / 2), 0,
			SAMPLE_KEY * ((SAME_KEY / 2) + 1) + SAMPLE_KEY * (SAME_KEY / 2));

	success = 0;
	failed = 0;
	retval = 0;
	Browse_hashed_shm_clear(&stHandle);
	while (retval >= 0 && retval != HASHED_SHM_SUCCESS_OK) {
		struct sample_data_4b *entry;
		st_HashedShmKey stKey;

		retval = Browse_hashed_shm_4b(&stHandle, &stKey, (void **)&entry);
		if (retval < 0) {
#ifdef OUTPUT_STEP_5
			fprintf(stderr, "Browse_hashed_shm: %d\n", retval);
#endif
			failed++;
		} else {
#ifdef OUTPUT_STEP_5
			fprintf(stderr, "BROWSE: %016lx:%016lx (%x) %016lx:%016lx %08x\n",
					stKey.__t,
					stKey.__b,
					(void *)entry,
					entry->stKey.__t,
					entry->stKey.__b,
					entry->data);
#endif
			success++;
		}
	}
	fprintf(stderr, "STEP 5 SUMMERY: Success %d Failed %d Entity %d\n",
			success, failed, atomic_read(&(stHandle.pstHeader->aiEntityUsed)));
	//fprintf(stderr, "STEP 5 SUMMERY: Success %d Failed %d\n", success, failed);

	Purge_hashed_shm_4b(&stHandle);
#endif //__TEST_4BSHM_STEP_14

#if 0
	ullKey = 0x279d00000000835bLL;
	retval = Get_hashed_shm(&stHandle, ullKey, (void **)&pstSOCKINFO);
	fprintf(stderr, "Get_hashed_shm: %d\n", retval);
#endif
	//Purge_hashed_shm(&stHandle);
}

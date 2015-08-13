/* 마운트빌 소스코드 UTF-8 Encode */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <errno.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <utillib.h>
#include <server_sock.h>
#include <server_define.h>
#include <server_shm.h>
#include <server_names.h>
#include <init_shm.h>
#include <libhashshm.h>



int JiSTOPFlag, Finishflag;

int g_dMyQid;

st_HashedShmHandle	stClientHandle;


void UserControlledSignal(int sign)
{
    JiSTOPFlag = 0;
    Finishflag = sign;
} /* end of UserControlledSignal */

void FinishProgram()
{
    dAppLog(LOG_CRI, "FinishProgram : PROGRAM IS NORMALLY TERMINATED, Cause = %d", Finishflag);
    exit(0);
} /* end of FinishProgram */

void IgnoreSignal(int sign)
{
    if (sign != SIGALRM)
        dAppLog(LOG_CRI,
        "IgnoreSignal : UNWANTED SIGNAL IS RECEIVED, signal = %d", sign);
    signal(sign, IgnoreSignal);
} /* end of IgnoreSignal */


void SetUpSignal()
{
    JiSTOPFlag = 1;

    /* WANTED SIGNALS   */
    signal(SIGTERM, UserControlledSignal);
    signal(SIGINT,  UserControlledSignal);
    signal(SIGQUIT, UserControlledSignal);

    /* UNWANTED SIGNALS */
    signal(SIGHUP,  IgnoreSignal);
    signal(SIGALRM, IgnoreSignal);
    signal(SIGPIPE, IgnoreSignal);
    signal(SIGPOLL, IgnoreSignal);
    signal(SIGPROF, IgnoreSignal);
    signal(SIGUSR1, IgnoreSignal);
    signal(SIGUSR2, IgnoreSignal);
    signal(SIGVTALRM, IgnoreSignal);
    signal(SIGCLD, SIG_IGN);

    dAppLog(LOG_DEBUG, "SetUpSignal : SIGNAL HANDLER WAS INSTALLED[%d]", JiSTOPFlag);

    return;
} /* end of SetUpSignal */

int dInit_IPC()
{
	int dRet, i;

	Init_hashed_shm_handle(&stClientHandle, MAX_CLIENT_CNT, sizeof(st_CLIENT_INFO),
            MAX_CLIENT_CNT, S_SSHM_CHARDBSSOCK);

    dRet = Init_hashed_shm(&stClientHandle);
    if (dRet < 0) {
        dAppLog(LOG_CRI, "[FAIL] Init_hashed_shm(SI): %s", Msg_hashed_shm(dRet));
		return -1;
    }

	if((g_dMyQid = msgget(S_MSGQ_CDB_STSOC, 0666|IPC_CREAT)) < 0) {
        dAppLog(LOG_CRI, "[init_ipcs] [FAIL:%d] MSGGET S_MSGQ_CDB_STSOC : [%s]\n",
                    errno, strerror(errno) );
        return -5;
	}



	return 0;
}

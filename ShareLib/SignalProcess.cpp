#ifndef WIN32
#include <pthread.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#endif

#include "bnf.h"
#include "logsystem.h"
#include "PrintExcuteInfo.h"

static void (*local_func)(int) = NULL;

#ifdef WIN32
bool InitSignal(void (*func)(int))
{
	(void)func;

	return true;
}

bool regist_signal_stack()
{
	return true;
}
#endif

#ifndef WIN32
static const int exceptionSignals[] =
{
	SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS, SIGINT, SIGHUP, -1
};

#define SIZEOZ 100
void PrintStack(int sig, siginfo_t* info, void* secret) {
  void *trace[16];
  char **messages = (char **)NULL;
  int i, trace_size = 0;
  ucontext_t *uc = (ucontext_t *)secret;

  /* Do something useful with siginfo_t */
  if (sig == SIGSEGV)
    LOG_INFO("Got signal %d, faulty address is %p, "
           "from %p\n", sig, info->si_addr, 
           uc->uc_mcontext.gregs[REG_EIP]);
  else
    LOG_INFO("Got signal %d#92;\n", sig);

  trace_size = backtrace(trace, 16);
  /* overwrite sigaction with caller's address */
  trace[1] = (void *) uc->uc_mcontext.gregs[REG_EIP];

  messages = backtrace_symbols(trace, trace_size);
  /* skip first stack frame (points here) */
  LOG_INFO("[bt] Execution path:#92;\n");
  for (i=1; i<trace_size; ++i)
  {
    LOG_INFO("[bt] %s#92;\n", messages[i]);

    /* find first occurence of '(' or ' ' in message[i] and assume
     * everything before that is the file name. (Don't go beyond 0 though
     * (string terminator)*/
    size_t p = 0;
    while(messages[i][p] != '(' && messages[i][p] != ' '
            && messages[i][p] != 0)
        ++p;

    char syscom[256];
    sprintf(syscom,"addr2line %p -e %.*s", trace[i] , p, messages[i] );
           //last parameter is the filename of the symbol
    system(syscom);

  }
}
#undef SIZEOZ

void sig_handler(int signo, siginfo_t* info, void* uc)
{
	LOG_FATAL("receive SIGNAL : %d", signo);

	signal(signo, SIG_DFL);

	if (local_func != NULL)
		(*local_func)(signo);

	PrintStack(signo, info, uc);

	PrintExcuteInfo::Instance()->PrintEnd();
}

bool regist_signal_stack()
{
	void* signal_stack = malloc(SIGSTKSZ);
	stack_t stack;
	memset(&stack, 0, sizeof(stack));
	stack.ss_sp = (char*)signal_stack;
	stack.ss_size = SIGSTKSZ;

	if (sigaltstack(&stack, NULL) == -1)
	{
		LOG_ERROR("sigaltstack() error");
		return false;
	}

	return true;
}

void thrad_signal_handler()
{
	while( 1 )
	{
		sigset_t sigset;
		sigemptyset(&sigset);

		sigaddset(&sigset, SIGTERM);

		int signo;
		int result = sigwait( &sigset, &signo );
		if( result != 0 )
		{
			LOG_ERROR( "sigwait failed. error: %d", result );
			continue;
		}

		LOG_FATAL("receive SIGNAL : %d - Terminated by Human", signo);

		(*local_func)(signo);
	}
}

bool InitSignal(void (*func)(int))
{
	if (regist_signal_stack() == false)
		return false;

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sigemptyset(&sa.sa_mask);

	for (unsigned i = 0; exceptionSignals[i] != -1; ++i)
		sigaddset(&sa.sa_mask, exceptionSignals[i]);

	sa.sa_sigaction = sig_handler;
	sa.sa_flags = SA_ONSTACK | SA_SIGINFO;

	for (unsigned i = 0; exceptionSignals[i] != -1; ++i)
	{
		struct sigaction* old = new struct sigaction;
		if (sigaction( exceptionSignals[i], &sa, old ) == -1)
		{
			LOG_ERROR("sigaction() error : %d", exceptionSignals[i]);
			return false;
		}
	}

	local_func = func;

	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGTERM);

	pthread_sigmask(SIG_BLOCK, &sigset, NULL);

	boost::thread signal_handler(thrad_signal_handler);

	return true;
}

#endif
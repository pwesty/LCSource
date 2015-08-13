#ifndef __SIGNAL_PROCESS_H__
#define __SIGNAL_PROCESS_H__

bool InitSignal(void (*func)(int));
bool regist_signal_stack();

#endif

#ifndef __STDHDRS_H__
#define __STDHDRS_H__

#define __BILLING_SERVER__

#include "Conf.h"
#include "Config.h"

#ifdef  PROC_BILLING
#	undef PROC_BILLING
#else
#	define PROC_BILLING	0
#endif

#include "../ShareLib/Conf.h"
#include "Sysdep.h"
#include "../ShareLib/Config.h"

#include "../ShareLib/logsystem.h"

#include "Utils.h"

#endif

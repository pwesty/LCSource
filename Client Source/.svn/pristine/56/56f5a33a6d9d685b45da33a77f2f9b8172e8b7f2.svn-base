/*
Copyright (c) 1999,2000  X Dimension Software

Project:
            3D Exploration
Module Name:
            e3call.h
Author:
            Alexander Shelemekhov
            alex@xdsoft.com

            3D Exploration call type

THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.

http://www.xdsoft.com/explorer
info@xdsoft.com

*/
#ifndef _15238E60_14A8_11D4_9F76_AC94217A99AA_
#define _15238E60_14A8_11D4_9F76_AC94217A99AA_

#define _E3_CALLTYPE __stdcall

#ifdef E3INTERNAL
#define _E3_METHOD(_return,_func) virtual _return _E3_CALLTYPE _func;
#define __E3_METHOD(_func) virtual _E3_CALLTYPE _func;
#else
#define _E3_METHOD(_return,_func) virtual _return _E3_CALLTYPE _func=0;
#define __E3_METHOD(_func) virtual _E3_CALLTYPE _func=0;
#endif

#define E3_INTERFACE class
#define E3_ACCESS    public:
#define E3_METHOD(_return,_func) virtual _return _E3_CALLTYPE _func=0;
#endif

#ifndef _TIFB_API_H_
#define _TIFB_API_H_


int is_registered_block( char *blkname );
char *mtime_tifb();
int GetProcessID( char *name );
int GetMcInitInfo();
int GetYorN();
int dGetUserPermission();

#endif

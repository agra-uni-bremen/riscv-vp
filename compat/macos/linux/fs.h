#ifndef FS_H
#define FS_H

#define BLKGETSIZE64 DKIOCGETBLOCKSIZE
#define lseek64 lseek
#define open64 open
#include <sys/disk.h>

#endif
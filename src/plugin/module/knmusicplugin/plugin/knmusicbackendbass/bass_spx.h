#ifndef BASSSPX_H
#define BASSSPX_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSSPXDEF
#define BASSSPXDEF(f) WINAPI f
#endif

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_SPX	0x10c00


HSTREAM BASSSPXDEF(BASS_SPX_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSSPXDEF(BASS_SPX_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif

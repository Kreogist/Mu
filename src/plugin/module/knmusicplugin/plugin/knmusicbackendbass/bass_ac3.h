#ifndef BASSAC3_H
#define BASSAC3_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSAC3DEF
#define BASSAC3DEF(f) WINAPI f
#endif

// BASS_Set/GetConfig options
#define BASS_CONFIG_AC3_DYNRNG		0x10001

// Additional BASS_AC3_StreamCreateFile/User/URL flags
#define BASS_AC3_DYNAMIC_RANGE	0x800	// enable dynamic range compression

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_AC3	0x11000


HSTREAM BASSAC3DEF(BASS_AC3_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSAC3DEF(BASS_AC3_StreamCreateURL)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user);
HSTREAM BASSAC3DEF(BASS_AC3_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif
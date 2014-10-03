#ifndef BASSMPC_H
#define BASSMPC_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSMPCDEF
#define BASSMPCDEF(f) WINAPI f
#endif

// Additional tags available from BASS_StreamGetTags
#define BASS_TAG_APE		6	// APE tags

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_MPC	0x10a00


HSTREAM BASSMPCDEF(BASS_MPC_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSMPCDEF(BASS_MPC_StreamCreateURL)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user);
HSTREAM BASSMPCDEF(BASS_MPC_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *proc, void *user);

#ifdef __cplusplus
}
#endif

#endif

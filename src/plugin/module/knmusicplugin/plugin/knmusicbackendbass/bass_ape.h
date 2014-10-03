#ifndef BASSAPE_H
#define BASSAPE_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSAPEDEF
#define BASSAPEDEF(f) WINAPI f
#endif

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_APE	0x10700


HSTREAM BASSAPEDEF(BASS_APE_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSAPEDEF(BASS_APE_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif

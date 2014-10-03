#ifndef BASSTTA_H
#define BASSTTA_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSTTADEF
#define BASSTTADEF(f) WINAPI f
#endif


// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_TTA	0x10f00


HSTREAM BASSTTADEF(BASS_TTA_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSTTADEF(BASS_TTA_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif

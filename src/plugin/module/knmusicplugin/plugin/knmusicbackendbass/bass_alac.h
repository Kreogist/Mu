#ifndef BASSALAC_H
#define BASSALAC_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSALACDEF
#define BASSALACDEF(f) WINAPI f
#endif

#define BASS_TAG_MP4 		7	// iTunes/MP4 metadata

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_ALAC	0x10e00


HSTREAM BASSALACDEF(BASS_ALAC_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSALACDEF(BASS_ALAC_StreamCreateURL)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user);
HSTREAM BASSALACDEF(BASS_ALAC_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif

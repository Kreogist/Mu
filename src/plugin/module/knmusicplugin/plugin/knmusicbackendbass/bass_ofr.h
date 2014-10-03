#ifndef BASSOFR_H
#define BASSOFR_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSOFRDEF
#define BASSOFRDEF(f) WINAPI f
#endif

// Additional tags available from BASS_StreamGetTags
#define BASS_TAG_APE		6	// APE tags

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_OFR	0x10600


HSTREAM BASSOFRDEF(BASS_OFR_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSOFRDEF(BASS_OFR_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif

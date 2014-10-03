//Zavyalov Alexey Alexandrovich (RUS) (Завьялов Алексей Александрович)
//for more information please write on e-mail _ff_alex_ff_@mail.ru (RUS) (Все вопросы писать на e-mail _ff_alex_ff_@mail.ru)

#ifndef BASSAIX_H
#define BASSAIX_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSAIXDEF
#define BASSAIXDEF(f) WINAPI f
#endif

// No TAG data

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_AIX	0x1F001 // Only for ADX of all versions (with AIXP support)


HSTREAM BASSAIXDEF(BASS_AIX_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSAIXDEF(BASS_AIX_StreamCreateURL)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user);
HSTREAM BASSAIXDEF(BASS_AIX_StreamCreateFileUser)(DWORD system, DWORD flags, BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif

 
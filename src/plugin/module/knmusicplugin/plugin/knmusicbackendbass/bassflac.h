/*
	BASSFLAC 2.4 C/C++ header file
	Copyright (c) 2004-2013 Un4seen Developments Ltd.

	See the BASSFLAC.CHM file for more detailed documentation
*/

#ifndef BASSFLAC_H
#define BASSFLAC_H

#include "bass.h"

#if BASSVERSION!=0x204
#error conflicting BASS and BASSFLAC versions
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSFLACDEF
#define BASSFLACDEF(f) WINAPI f
#endif

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_FLAC		0x10900
#define BASS_CTYPE_STREAM_FLAC_OGG	0x10901

// Additional tag types
#define BASS_TAG_FLAC_CUE			12		// cuesheet : TAG_FLAC_CUE structure
#define BASS_TAG_FLAC_PICTURE		0x12000	// + index #, picture : TAG_FLAC_PICTURE structure

typedef struct {
	DWORD apic;			// ID3v2 "APIC" picture type
	const char *mime;	// mime type
	const char *desc;	// description
	DWORD width;
	DWORD height;
	DWORD depth;
	DWORD colors;
	DWORD length;		// data length
	const void *data;
} TAG_FLAC_PICTURE;

typedef struct {
	QWORD offset;			// index offset relative to track offset (samples)
	DWORD number;			// index number
} TAG_FLAC_CUE_TRACK_INDEX;

typedef struct {
	QWORD offset;			// track offset (samples)
	DWORD number;			// track number
	const char *isrc;		// ISRC
	DWORD flags;
	DWORD nindexes;			// number of indexes
	const TAG_FLAC_CUE_TRACK_INDEX *indexes; // the indexes
} TAG_FLAC_CUE_TRACK;

typedef struct {
	const char *catalog;	// media catalog number
	DWORD leadin;			// lead-in (samples)
	BOOL iscd;				// a CD?
	DWORD ntracks;			// number of tracks
	const TAG_FLAC_CUE_TRACK *tracks; // the tracks
} TAG_FLAC_CUE;

// TAG_FLAC_CUE_TRACK flags
#define TAG_FLAC_CUE_TRACK_DATA	1	// data track
#define TAG_FLAC_CUE_TRACK_PRE	2	// pre-emphasis

HSTREAM BASSFLACDEF(BASS_FLAC_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSFLACDEF(BASS_FLAC_StreamCreateURL)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user);
HSTREAM BASSFLACDEF(BASS_FLAC_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}

#ifdef _WIN32
static inline HSTREAM BASS_FLAC_StreamCreateFile(BOOL mem, const WCHAR *file, QWORD offset, QWORD length, DWORD flags)
{
	return BASS_FLAC_StreamCreateFile(mem, (const void*)file, offset, length, flags|BASS_UNICODE);
}

static inline HSTREAM BASS_FLAC_StreamCreateURL(const WCHAR *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user)
{
	return BASS_FLAC_StreamCreateURL((const char*)url, offset, flags|BASS_UNICODE, proc, user);
}
#endif
#endif

#endif

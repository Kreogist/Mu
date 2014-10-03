
/*****************************************************************************
 *  BASS_VST C/C++ Header and Documentation
 *****************************************************************************
 *
 *  BASS_VST allows the usage of VST plugins in BASS.  BASS_VST was
 *  written to work with Silverjuke(R).  Any other usage is for your own risk -
 *  but you're welcome and I'm sure it will work :-)
 *
 *  If you use BASS_VST in your projects, you must not charge anything for this
 *  feature.  I would also be happy to hear from you if you use BASS_VST.  A
 *  note like "BASS_VST by Bjoern Petersen @ www.silverjuke.net" in the about
 *  box would be nice, however, I'm not angry if you decide not to do so.
 *
 *  BASS_VST is a VST host compatible up to the VST 2.4 implementation and
 *  (hopefully) implements all needed features - incl. the new double precision
 *  processing.
 *
 *  While the normal usage is very easy (only one function -
 *  BASS_VST_ChannelSetDSP() - is really needed), BASS_VST also allows advanced
 *  usage of the VST features - incl. embedding an editor or subclassing
 *  the whole library.  The latter may require the VST SDK which can be
 *  obtained for free from Steinberg.  Moreover, BASS_VST also supports
 *  VST instruments (VSTi plugins).
 *
 *  (C) Bjoern Petersen Software Design'n'Development
 *  VST PlugIn Interface Technology by Steinberg Media Technologies GmbH
 *
 *  Contact: drsilver@silverjuke.net - http://www.silverjuke.net
 *
 *****************************************************************************
 *
 *  Version History:
 *
 *  Version 2.4.5.0 (9/16/2013)
 *  First version on BASS_VST for OS X
 *  Ported by BaseHead, LLC   contact support@baseheadinc.com
 *****************************************************************************/




#ifndef BASS_VST_H
#define BASS_VST_H

#ifndef BASS_H
#include "bass.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif




/* If you load the DLL using LoadLibrary() instead of using bass_vst.lib,
 * you can define the functions as pointers by setting BASS_VSTDEF(f) to
 * "(WINAPI *f)".  As you should do this only once, you can define
 * BASS_VSTSCOPE to "extern" for subsequent includes.  Of course, all this must
 * be done before including bass_vst.h!
 */
#ifndef BASS_VSTDEF
#define BASS_VSTDEF(f) WINAPI f
#endif

#ifndef BASS_VSTSCOPE
#define BASS_VSTSCOPE
#endif




/*****************************************************************************
 *  Assigning VST effects to BASS channels
 *****************************************************************************/




/* BASS_VST_ChannelSetDSP() sets any VST effect plugin (defined by a DLL file
 * name) to any channel handle.  VST instrument plugins cannot be used with
 * this function.  Flags:
 *
 * BASS_UNICODE         Treat the dllFile pointer as UNICODE instead of ANSI
 *                      (0x80000000)
 *
 * BASS_VST_KEEP_CHANS  By default, mono effects assigned to stereo channels
 *                      are mixed down before processing and converted back
 *                      to stereo afterwards. Set this flag to avoid this
 *                      behaviour in which case only the first channel is
 *                      affected by processing (0x00000001)
 *
 * The priority parameter has the same meaning as for BASS_ChannelSetDSP() -
 * DSPs with higher priority are called before those with lower.
 *
 * On success, the function returns the new vstHandle that must be given to
 * the other functions.  For errors, 0 is returned and BASS_ErrorGetCode()
 * will specify the reason.
 *
 * For testing if a DLL is a valid VST effect, you can set chHandle to 0 -
 * however, do not forget to call BASS_VST_ChannelRemoveDSP() even in this
 * case.
 *
 * You may safely assign the same DLL to different channels at the same time -
 * the library makes sure, every channel is processed indepeningly.  But take
 * care to use the correct vstHandles in this case.
 *
 * Finally, you can use any number of VST effects on a channel.
 */
BASS_VSTSCOPE DWORD BASS_VSTDEF(BASS_VST_ChannelSetDSP)
    (DWORD chHandle, const void* dllFile, DWORD flags, int priority);

#define BASS_VST_KEEP_CHANS 0x00000001 /* flag that may be used for BASS_VST_ChannelSetDSP(), see the comments above */




/* BASS_VST_ChannelRemoveDSP() removes a VST effect from a channel and destroys
 * the VST instance. vstHandle is the value returned by BASS_VST_ChannelSetDSP()
 * and is no longer valid after calling this function.
 *
 * If you do not call BASS_VST_ChannelRemoveDSP() explicitly and you have
 * assigned a channel to the effect, the effect is removed automatically when
 * the channel handle is deleted.
 *
 * For various reasons, the underlying DLL is unloaded from memory with a
 * little delay, however, this has also the advantage that subsequent
 * adding/removing of DLLs to channels has no bad performance impact.
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_ChannelRemoveDSP)
    (DWORD chHandle, DWORD vstHandle);




/*****************************************************************************
 * Create BASS streams using VST instruments (VSTi plugins)
 *****************************************************************************/



/* BASS_VST_ChannelCreate() creates a new BASS stream based on any VST
 * instrument plugin - if you are only interested in VST effect plugins, you
 * can ignore this function.  Flags:
 *
 * BASS_UNICODE         Treat the dllFile pointer as UNICODE instead of ANSI
 *                      (0x80000000)
 *
 * BASS_SPEAKER_xxx     These flags will just work in the same way as they
 * BASS_SAMPLE_FLOAT    work for other streams
 * BASS_SAMPLE_SOFTWARE .
 * BASS_SAMPLE_3D       .
 * BASS_SAMPLE_FX       .
 * BASS_STREAM_DECODE
 *
 * On success, the function returns the new vstHandle that must be given to
 * the other functions.  The returned VST handle can also be given to the
 * typical BASS_Channel*(). For errors, 0 is returned and BASS_ErrorGetCode()
 * will specify the reason.
 */
BASS_VSTSCOPE DWORD BASS_VSTDEF(BASS_VST_ChannelCreate)
    (DWORD freq, DWORD chans, const void* dllFile, DWORD flags);



/* BASS_VST_ChannelFree deletes a VST instrument channel created by
 * BASS_VST_ChannelCreate().  Note, that you cannot delete effects assigned to
 * channels this way; for this purpose, please use BASS_VST_ChannelRemoveDSP().
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_ChannelFree)
    (DWORD vstHandle);



/*****************************************************************************
 *  VST Parameter Handling
 *****************************************************************************/




/* BASS_VST_GetParamCount() returns the number of editable parameters for
 * the VST plugin.  If the plugin has no editable parameters, 0 is returned.
 */
BASS_VSTSCOPE int BASS_VSTDEF(BASS_VST_GetParamCount)
    (DWORD vstHandle);




/* Get/Set the value of a single parameter.  All parameters are in the
 * range 0.0 to 1.0, however, from the view of a VST plugin, they may
 * represent completely different values.  You can use BASS_VST_GetParamInfo()
 * to get further information about a single parameter.
 *
 * paramIndex must be smaller than BASS_VST_GetParamCount().
 */
BASS_VSTSCOPE float BASS_VSTDEF(BASS_VST_GetParam)
    (DWORD vstHandle, int paramIndex);

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetParam)
    (DWORD vstHandle, int paramIndex, float value);




/* Get some common information about an editable parameter to a
 * BASS_VST_PARAMINFO structure.
 *
 * paramIndex must be smaller than BASS_VST_GetParamCount().
 */
typedef struct
{
    char    name[16];               /* examples: Time, Gain, RoomType */
    char    unit[16];               /* examples: sec, dB, type */
    char    display[16];            /* the current value in a readable format, examples: 0.5, -3, PLATE */
    float   defaultValue;           /* the default value - this is the value used by the VST plugin just after creation */
} BASS_VST_PARAM_INFO;

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_GetParamInfo)
    (DWORD vstHandle, int paramIndex, BASS_VST_PARAM_INFO* ret);




/*****************************************************************************
 *  VST Program Handling
 *****************************************************************************/



/* BASS_VST_GetProgramCount() returns the number of editable programs for
 * the VST plugin.  Many (not all!) plugins have more than one "program" that
 * can hold a complete set of parameters each.  Moreover, some of these
 * programs may be initialized to some useful "factory defaults".
 */
BASS_VSTSCOPE int BASS_VSTDEF(BASS_VST_GetProgramCount)
    (DWORD vstHandle);




/* BASS_VST_GetProgram() returns the currently selected program.  Valid
 * program numbers are between 0 and BASS_VST_GetProgramCount() minus 1.
 * After construction, always the first program (0) is selected.
 *
 * With BASS_VST_SetProgram() you can change the selected program.  Functions
 * as as BASS_VST_SetParam() will always change the selected program's settings.
 */
BASS_VSTSCOPE int BASS_VSTDEF(BASS_VST_GetProgram)
    (DWORD vstHandle);

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetProgram)
    (DWORD vstHandle, int programIndex);




/* With BASS_VST_GetProgramParam() you can query the parameters of any program.
 * The parameters of the currently selected program can also be queried by
 * BASS_VST_GetParam().  The function returns the parameters as a pointer to
 * an array of floats.  The pointer is valid until you call this function again
 * for the same vstHandle or until you delete the plugin.  The number of
 * elements in the returned array is equal to BASS_VST_GetParamCount().
 *
 * programIndex must be smaller than BASS_VST_GetProgramCount().  If you set
 * programIndex to -1, you can retrieve the plugin's default values (these are
 * the same values as returned by BASS_VST_GetParamInfo()).  This function does
 * not change the selected program.
 */
BASS_VSTSCOPE const float* BASS_VSTDEF(BASS_VST_GetProgramParam)
    (DWORD vstHandle, int programIndex);




/* With BASS_VST_SetProgramParam() you can set the parameters of any program.
 * The parameters of the currently selected program can also be set using
 * BASS_VST_SetParam().  The parameters must be given as a pointer to an
 * array of floats.  The function expects the array to have as many elements
 * as defined by BASS_VST_GetParamCount().  When the function returns, the
 * given pointer is no longer needed by BASS_VST.
 *
 * programIndex must be smaller than BASS_VST_GetProgramCount().  This function
 * does not change the selected program.
 *
 * If you use BASS_VST_SetCallback(), the BASS_VST_PARAM_CHANGED event is only
 * posted if you select a program with parameters different from the prior.
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetProgramParam)
    (DWORD vstHandle, int programIndex, const float* param);




/* With BASS_VST_GetProgramName() and BASS_VST_SetProgramName() you can get/set
 * the name of any program.  For BASS_VST_GetProgramName(), the returned pointer
 * is valid until you call this function again for the same vstHandle or until
 * you delete the plugin.  The names are limited to 24 characters plus a
 * terminating null-byte; BASS_VST truncates the names, if needed.
 *
 * programIndex must be smaller than BASS_VST_GetProgramCount().  These
 * functions do not change the selected program.
 */
BASS_VSTSCOPE const char* BASS_VSTDEF(BASS_VST_GetProgramName)
    (DWORD vstHandle, int programIndex);

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetProgramName)
    (DWORD vstHandle, int programIndex, const char* name);




/*****************************************************************************
 *  Misc.
 *****************************************************************************/




/* Call BASS_VST_Resume() after playback position changes or sth. like that.
 * This will reset the internal VST buffers which may remember some "old" data.
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_Resume)
    (DWORD vstHandle);




/* With BASS_VST_SetBypass() you can bypass the VST plugin processing
 * (state=TRUE) or switch back to normal processing (state=FALSE). By default,
 * bypassing is off and the VST plugin will be processed normally.
 * BASS_VST_GetBypass() returns the current state.
 *
 * Note, that the bypassing is completely done by BASS_VST, we're not using
 * the so-called "soft" bypass that is implemented by some VST plugins. This is
 * for the following reasons:
 *
 * - Soft-bypassing is not supported by all VST plugins
 * - The state of soft-bypassing cannot be queried safely
 * - Soft-bypassing would not be a real bypass as some channel transformations
 *   may still be needed
 * - Performance reasons - soft-bypassing would require still most of the
 *   needed BASS_VST transformations
 * - Finally, I do not see any advantages of the soft-bypassing
 *
 * If you really need the soft bypassing, you can do the following using the
 * VST SDK:
 *
 *      #include <aeffectx.h> // you can get this file from Steinberg
 *
 *      BASS_VST_INFO info;
 *      BASS_VST_GetInfo(vstHandle, &info);
 *
 *      if( info.aeffect->dispatcher(info.aeffect, effCanDo, 0, 0, "bypass", 0.0) )
 *      {
 *          info.aeffect->dispatcher(info.aeffect, effSetBypass, 0, (BOOL)softBypassState, NULL, 0.0);
 *      }
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetBypass)
    (DWORD vstHandle, BOOL state);

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_GetBypass)
    (DWORD vstHandle);




/* BASS_VST_GetInfo() writes some information about a vstHandle to a
 * BASS_VST_INFO structure.
 *
 * Some words to the number of input/output channels:
 *
 * VST plugins that have no input channels (so called "instruments") are not
 * loaded by BASS_VST.  You can assume chansIn and chansOut to be at least 1.
 *
 * Multi-channel streams should work correctly, if supported by a VST plugin.
 * If not, only the first chansIn channels are processed by the plugin, the
 * other ones stay unaffected.  The opposite, eg. assigning multi-channel
 * VST plugins to stereo channels, should be no problem at all.
 *
 * If mono plugins are assigned to stereo channels, the result will be mono,
 * expanded to both channels. This behaviour can be switched of using the
 * BASS_VST_KEEP_CHANS in BASS_VST_ChannelSetDSP().
 */
typedef struct
{
    DWORD    channelHandle;         /* the channelHandle as given to BASS_VST_ChannelSetDSP() or returned by BASS_VST_ChannelCreate; 0 if no channel was assigned to the VST plugin */
    DWORD    uniqueID;              /* a unique ID for the VST plugin (the IDs are registered at Steinberg) */
    char     effectName[80];        /* the plugin's name */
    DWORD    effectVersion;         /* the plugin's version */
    DWORD    effectVstVersion;      /* the VST version, the plugin was written for */
    DWORD    hostVstVersion;        /* the VST version supported by BASS_VST, currently 2.4 */
    char     productName[80];       /* the product name, may be empty */
    char     vendorName[80];        /* the vendor name, may be empty */
    DWORD    vendorVersion;         /* vendor-specific version number */
    DWORD    chansIn;               /* max. number of possible input channels */
    DWORD    chansOut;              /* max. number of possible output channels */
    DWORD    initialDelay;          /* for algorithms which need input in the first place, in samples */
    DWORD    hasEditor;             /* can the BASS_VST_EmbedEditor() function be called? */
    DWORD    editorWidth;           /* initial/current width of the editor, also note BASS_VST_EDITOR_RESIZED; if the editor is not yet opened, this value may be 0 for some (very few) plugins! */
    DWORD    editorHeight;          /* same for the height */
#ifdef __aeffect__
    AEffect* aeffect;               /* the underlying AEffect object (see aeffectx.h in the VST SDK) */
#else
    void*    aeffect;
#endif
    DWORD    isInstrument;          /* 1=the VST plugin is an instrument, 0=the VST plugin is an effect */
    DWORD    dspHandle;             /* the internal DSP handle */
} BASS_VST_INFO;

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_GetInfo)
    (DWORD vstHandle, BASS_VST_INFO* ret);




/* Many VST plugins come along with an graphical parameters editor; with the
 * following function, you can embed these editors to your user interface.
 *
 * To embed the editor to another window, call this function with parentWindow
 * set to the HWND of the parent window.  To check, if an plugin has an editor,
 * see the hasEditor flag set by BASS_VST_GetInfo(). Example:
 *
 *      BASS_VST_INFO info;
 *      BASS_VST_GetInfo(vstHandle, &info);
 *
 *      if( info.hasEditor )
 *      {
 *          HWND parentWindow = CreateWindow(...);
 *          BASS_VST_EmbedEditor(vstHandle, parentWindow);
 *      }
 *
 * To "unembed" the editor, call this function with parentWindow set to NULL.
 * Example:
 *
 *      BASS_VST_EmbedEditor(vstHandle, NULL);
 *
 * If you create the editor window independently of a real channel (eg. by
 * skipping the channel parameter when calling BASS_VST_ChannelSetDSP()) and
 * the editor displays any spectrums, VU-meters or such, the data for this come
 * from the most recent channel with the same plugin and the same scope; the
 * scope can be set by BASS_VST_SetScope() to any ID, the default is 0.
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_EmbedEditor)
#ifdef _WIN32
    (DWORD vstHandle, HWND parentWindow);
#else
    (DWORD vstHandle, void* parentWindow);
#endif

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetScope)
    (DWORD vstHandle, DWORD scope);



/* With BASS_VST_SetCallback() you can assign a callback function of the type
 * VSTPROC* to a vstHandle.  The callback function is called with the BASS_VST_*
 * actions defined below then.  Unless defined otherwise, the callback function
 * should always return 0. The "user" parameter given to BASS_VST_SetCallback()
 * is just forwarded to the callback.
 *
 * Every vstHandle can have only one callback function; subsequent calls to
 * BASS_VST_SetCallback() for the same vstHandle will just change the callback
 * function.  You can remove a callback function from a vstHandle with
 * BASS_VST_SetCallback(vstHandle, NULL, 0);  however, this is not needed from
 * the view of BASS_VST.
 */
typedef DWORD (CALLBACK VSTPROC)(DWORD vstHandle, DWORD action, DWORD param1, DWORD param2, void* user);
#define BASS_VST_PARAM_CHANGED  1   /* some parameters are changed by the editor opened by BASS_VST_EmbedEditor(), NOT posted if you call BASS_VST_SetParam() */
#define BASS_VST_EDITOR_RESIZED 2   /* the embedded editor window should be resized, the new width/height can be found in param1/param2 and in BASS_VST_GetInfo() */
#define BASS_VST_AUDIO_MASTER   3   /* can be used to subclass the audioMaster callback, param1 is a pointer to a BASS_VST_AUDIO_MASTER_PARAM structure defined below */

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetCallback)
    (DWORD vstHandle, VSTPROC*, void* user);




/* Subclassing BASS_VST: By using BASS_VST_SetCallback() and catching the
 * BASS_VST_AUDIO_MASTER events, you can subclass the communication between the
 * plugin and BASS_VST.  You'll find all needed parameters in a
 * BASS_VST_AUDIO_MASTER_PARAM structure given in param1 to the callback
 * function.  To avoid the BASS_VST default processing on some opcodes, just
 * set the "doDefault" member to 0 - in this case BASS_VST just forwards your
 * return value to the plugin and does nothing else.  Initially, "doDefault"
 * is always set to 1 and BASS_VST will do what's needed. For a documentation
 * about the possible requests, see the VST SDK from Steinberg.
 *
 * Example for replacing the BASS_VST file selector:
 *
 *      #include <aeffectx.h> // you can get this file from Steinberg
 *
 *      DWORD openMyFileSelector(VstFileSelect* fileSelect)
 *      {
 *          // do what to do here ...
 *      }
 *
 *      DWORD myCallback(DWORD vstHandle, DWORD action, DWORD param1, DWORD param2, void* user)
 *      {
 *          if( action == BASS_VST_AUDIO_MASTER )
 *          {
 *              BASS_VST_AUDIO_MASTER_PARAM* audioMaster = (BASS_VST_AUDIO_MASTER_PARAM*)param1;
 *              if( audioMaster->opcode == audioMasterOpenFileSelector )
 *              {
 *                  openMyFileSelector((VstFileSelect*)audioMaster->ptr);
 *                  audioMaster->doDefault = 0;
 *                  return 1;
 *              }
 *          }
 *          return 0;
 *      }
 *
 *      BASS_VST_SetCallback(vstHandle, myCallback, 0);
 */
typedef struct
{
#ifdef __aeffect__
    AEffect* aeffect;
#else
    void*    aeffect;
#endif
    long     opcode;
    long     index;
    long     value;
    void*    ptr;
    float    opt;
    long     doDefault;
} BASS_VST_AUDIO_MASTER_PARAM;




/* Some VST plugins come along localized.  With this function you can set the
 * desired language as ISO 639.1 - eg. "en" for english, "de" for german, "es"
 * for spanish and so on.  The default language is english.
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_SetLanguage)
    (const char* lang);




/* With BASS_VST_ProcessEvent() you can send MIDI events to the plugin similar
 * to BASS_MIDI_StreamEvent().
 *
 * With BASS_VST_ProcessEventRaw() you can send raw SysEx- or MIDI-Messages to
 * the plugin.  For SysEx-Messages, let "event" point to the data to send and
 * set "length" to the number of bytes to send.  For MIDI-Message set length to
 * 0 and encode "event" as 0x00xxyyzz with xx=MIDI command, yy=MIDI databyte #1,
 * zz=MIDI databyte #2.
 *
 * Example:
 *
 *      #include <bassmidi.h>
 *
 *      // press note #60 (middle C) on channel #1
 *      BASS_VST_ProcessEvent(vstHandle, 0, MIDI_EVENT_NOTE, MAKEWORD(60, 100));
 *
 *      // sending a raw SysEx-Nessage
 *      char sysex[] = {0xF0,0x7E,0x7F,0x09,0x01,0xF7};
 *      BASS_VST_ProcessEventRaw(vstHandle, (void*)sysex, 6);
 *
 *      // sending a raw MIDI-Message
 *      BASS_VST_ProcessEventRaw(vstHandle, (void*)0x903C64, 0);
 */
BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_ProcessEvent)
    (DWORD vstHandle, DWORD midiCh, DWORD event, DWORD param);

BASS_VSTSCOPE BOOL BASS_VSTDEF(BASS_VST_ProcessEventRaw)
    (DWORD vstHandle, const void* event, DWORD length);




/* If any BASS_VST function fails, you can use BASS_ErrorGetCode() to obtain
 * the reason for failure.  The error codes are the one from bass.h plus the
 * error codes below.  If a function succeeded, BASS_ErrorGetCode() returns
 * BASS_OK.
 */
#define BASS_VST_ERROR_NOINPUTS     3000 /* the given VST plugin has no inputs and is probably a VST instrument and no effect */
#define BASS_VST_ERROR_NOOUTPUTS    3001 /* the given VST plugin has no outputs */
#define BASS_VST_ERROR_NOREALTIME   3002 /* the given VST plugin does not support realtime processing */




#ifdef __cplusplus
}
#endif

#endif /* BASS_VST_H */

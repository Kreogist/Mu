/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNVERSION
#define KNVERSION

//Change the version here.
#define MAJOR_VERSION 0    //((QTAV_VERSION&0xff0000)>>16)
#define MINOR_VERSION 5    //((QTAV_VERSION&0xff00)>>8)
#define PATCH_VERSION 0    //(QTAV_VERSION&0xff)

//----DON'T TOUCH BELOW---
//The following data will automatically generate according to MAJOR_VERSION,
//MINOR_VERSION and PATCH_VERSION.
// Features:
//  * Support 8-bit (maximum 255) for major and minor, 16=bit number (maximum
//    65535) for patch.

//Get version from application version.
#define GET_MAJOR_VERSION(V) ((V & 0xff00000000) >> 24)
#define GET_MINOR_VERSION(V) ((V & 0xff0000) >> 16)
#define GET_PATCH_VERSION(V) (V & 0xffff)

//Generate application version.
#define GENERATE_VERSION(major, minor, patch) \
    (((major&0xff)<<24) | ((minor&0xff)<<16) | (patch&0xffff))
#define APP_VERSION \
    GENERATE_VERSION(MAJOR_VERSION, MINOR_VERSION, PATCH_VERSION)

//Generate string for version.
//MAGIC, DON'T TOUCH
// Stringify \a x.
#define _TOSTR(x)   #x
// Stringify \a x, perform macro expansion.
#define TOSTR(x)  _TOSTR(x)

/* the following are compile time version */
/* C++11 requires a space between literal and identifier */
#define APP_VERSION_STR \
    TOSTR(MAJOR_VERSION) "." TOSTR(MINOR_VERSION) "." TOSTR(PATCH_VERSION)

//----DON'T TOUCH ABOVE---

#endif // KNVERSION

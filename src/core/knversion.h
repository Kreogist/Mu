/*
 * Copyright (C) Kreogist Dev Team <kreogistdevteam@126.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef KNVERSION
#define KNVERSION

//Change the version here.
#define MAJOR_VERSION 0
#define MINOR_VERSION 8
#define PATCH_VERSION 56

//----DON'T TOUCH BELOW---
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

/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#ifndef KNVERSION
#define KNVERSION

/*
 * This file is only a place for application version settings.
 * You should change the version here, and all the version information should be
 * synced from here.
 */

//Change the version here.
#define MAJOR_VERSION 0
#define MINOR_VERSION 9
#define PATCH_VERSION 2

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

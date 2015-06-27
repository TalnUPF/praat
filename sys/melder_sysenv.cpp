/* melder_sysenv.cpp
 *
 * Copyright (C) 1992-2011,2015 Paul Boersma
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * pb 2004/10/14 made Cygwin-compatible
 * Eric Carlson & Paul Boersma 2005/05/19 made MinGW-compatible
 * pb 2006/10/28 erased MacOS 9 stuff
 * pb 2011/04/05 C++
 */

/*
 * This is a replacement for the CodeWarrior routines getenv and system,
 * into which many bugs were introduced in the year 2000.
 */

#if defined (_WIN32)
	#if ! defined (__CYGWIN__) && ! defined (__MINGW32__)
		#include <crtl.h>
	#endif
	#include <windows.h>
	#include <errno.h>
	#include <stdlib.h>
#endif
#include "melder.h"

char32 * Melder_getenv (const char32 *variableName) {
	#if defined (macintosh) || defined (UNIX) || defined (__MINGW32__)
		return Melder_peek8to32 (getenv (Melder_peek32to8 (variableName)));
	#elif defined (_WIN32)
		static char32 buffer [11] [255];
		static int ibuffer = 0;
		if (++ ibuffer == 11) ibuffer = 0;
		long n = GetEnvironmentVariableW (variableName, buffer [ibuffer], 255);
		if (n == ERROR_ENVVAR_NOT_FOUND) return NULL;
		return & buffer [ibuffer] [0];
	#endif
}

void Melder_system (const char32 *command) {
	#if defined (macintosh) || defined (UNIX)
		if (system (Melder_peek32to8 (command)) != 0)
			Melder_throw (U"System command failed.");
	#elif defined (_WIN32)
		STARTUPINFO siStartInfo;
		PROCESS_INFORMATION piProcInfo;
		char32 *comspec = Melder_getenv (U"COMSPEC");   // e.g. "C:\WINDOWS\COMMAND.COM" or "C:\WINNT\windows32\cmd.exe"
		if (comspec == NULL) {
			comspec = Melder_getenv (U"ComSpec");
		}
		autoMelderString buffer;
		if (comspec != NULL) {
			MelderString_copy (& buffer, comspec);
		} else {
			OSVERSIONINFOEX osVersionInfo;
			memset (& osVersionInfo, 0, sizeof (OSVERSIONINFOEX));
			osVersionInfo. dwOSVersionInfoSize = sizeof (OSVERSIONINFOEX);
			if (! GetVersionEx ((OSVERSIONINFO *) & osVersionInfo)) {
				osVersionInfo. dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
				if (! GetVersionEx ((OSVERSIONINFO *) & osVersionInfo))
					Melder_throw (U"System command cannot find system version.");
			}
			switch (osVersionInfo. dwPlatformId) {
				case VER_PLATFORM_WIN32_NT: {
					MelderString_copy (& buffer, U"cmd.exe");
				} break; case VER_PLATFORM_WIN32_WINDOWS: {
					MelderString_copy (& buffer, U"command.com");
				} break; default: {
					MelderString_copy (& buffer, U"command.com");
				}
			}
		}
		MelderString_append (& buffer, U" /c ", command);
        memset (& siStartInfo, 0, sizeof (siStartInfo));
        siStartInfo. cb = sizeof (siStartInfo);
		if (! CreateProcess (NULL, Melder_peek32toW (buffer.string), NULL, NULL, TRUE, 0, NULL, NULL, & siStartInfo, & piProcInfo))
			Melder_throw (U"Cannot create subprocess.");
		WaitForSingleObject (piProcInfo. hProcess, -1);
		CloseHandle (piProcInfo. hProcess);
		CloseHandle (piProcInfo. hThread);
	#endif
}

/* End of file melder_sysenv.cpp */

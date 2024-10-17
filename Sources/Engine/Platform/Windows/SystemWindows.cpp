//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "Platform/Platform.h"

#ifdef EUGENIX_PLATFORM_WINDOWS

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "Core/Base.h"
#include "Platform/Runtime.h"
#include "Platform/Windows/PlatformWindows.h"

namespace Eugenix
{
	namespace System
	{
		void Exit(int32_t exitCode)
		{
			::exit(exitCode);
		}

		void Message(const char* format, ...)
		{
			va_list argptr;
			va_start(argptr, format);
			static char buf[1024];
			memset(buf, 0, sizeof(buf));
			_vsnprintf_s(buf, sizeof(buf), format, argptr);
			va_end(argptr);
			//if (fullscreen)
			//	ChangeDisplaySettings(NULL, 0);
			::MessageBox(GetActiveWindow(), buf, "Eugenix Message", MB_ICONERROR);
		}

		void Error(const char* format, ...)
		{
			va_list argptr;
			va_start(argptr, format);
			static char buf[1024];
			memset(buf, 0, sizeof(buf));
			_vsnprintf_s(buf, sizeof(buf), format, argptr);
			va_end(argptr);
			MessageBoxA(GetActiveWindow(), buf, "Eugenix Error", MB_ICONERROR);

			//if (fullscreen)
			//	ChangeDisplaySettings(NULL, 0);
			Exit(1);
		}

		void Sleep(const uint32_t msec)
		{
			::Sleep(msec);
		}
	} // namespace System
} // namespace Eugenix

#endif // EUGENIX_WINDOWS
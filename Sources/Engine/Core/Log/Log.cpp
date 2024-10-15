//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include <cstdarg>
#include <cstdio>
#include "Log.h"

namespace Eugenix
{
	namespace Log
	{
		void Impl(const char* formatMessage, va_list args)
		{
			vprintf(formatMessage, args);
		}
		void Info(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			Impl(formatMessage, args);
			va_end(args);
		}
		void Warning(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			Impl(formatMessage, args);
			va_end(args);
		}
		void Error(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			Impl(formatMessage, args);
			va_end(args);
		}
		void Fatal(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			Impl(formatMessage, args);
			va_end(args);
		}
	} // namespace Log
} // namespace Eugenix
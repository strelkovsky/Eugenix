//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include <cstdarg>
#include <cstdio>
#include <list>
#include <memory>
#include "Log.h"

namespace Eugenix
{
	namespace Log
	{
		namespace _details
		{
			std::list<std::unique_ptr<LogProcessor>> processors;

			Level current_level;

			void logMessage(Level level, const char* format, va_list argsptr)
			{
				if (processors.empty())
				{
					vprintf(format, argsptr);
				}
				else
				{
					for (auto& p : processors)
					{
						p->Message(level, format, argsptr);
					}
				}
			}
		} // namespace _details

		void Info(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			_details::logMessage(Level::Info, formatMessage, args);
			va_end(args);
		}
		void Warning(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			_details::logMessage(Level::Warning, formatMessage, args);
			va_end(args);
		}
		void Error(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			_details::logMessage(Level::Error, formatMessage, args);
			va_end(args);
		}
		void Fatal(const char* formatMessage, ...)
		{
			va_list args;
			va_start(args, formatMessage);
			_details::logMessage(Level::Fatal, formatMessage, args);
			va_end(args);
		}
	} // namespace Log
} // namespace Eugenix
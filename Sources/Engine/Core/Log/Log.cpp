//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include <cstdarg>
#include <cstdio>
#include <list>

#include "Log.h"

namespace Eugenix
{
	namespace Log
	{
		namespace _details
		{
			std::list<std::shared_ptr<LogProcessor>> processors;
			Level currentLevel{Level::Info};

			void logMessage(Level level, const char* format, va_list argsptr)
			{
				if (level >= currentLevel)
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
			}
		} // namespace _details

		void SetLoglevel(Level level)
		{
			_details::currentLevel = level;
		}

		void SetLogFilter(const char* filter)
		{
		}

		void Attach(std::shared_ptr<LogProcessor> processor)
		{
			_details::processors.push_back(processor);
		}

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
//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include <memory>

namespace Eugenix
{
	namespace Log
	{
		enum struct Level : short
		{
			None = 0,

			Info,
			Warning,
			Error,
			Fatal
		};

		struct LogProcessor
		{
			virtual ~LogProcessor() = 0;
			virtual void Message(Level level, const char* format, va_list argsptr) = 0;
		};

		void SetLoglevel(Level level);
		void SetLogFilter(const char* filter);

		void Attach(std::unique_ptr<LogProcessor> proc);

		void Info(const char* fmtMsg, ...);
		void Warning(const char* fmtMsg, ...);
		void Error(const char* fmtMsg, ...);
		void Fatal(const char* fmtMsg, ...);
	} // namespace Log
}

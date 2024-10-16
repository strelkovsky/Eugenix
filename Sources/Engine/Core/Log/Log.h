//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

namespace Eugenix
{
	namespace Log
	{
		enum struct Level
		{
			None = 0,

			Info,
			Warning,
			Error,
			Fatal
		};

		void Info(const char* fmtMsg, ...);
		void Warning(const char* fmtMsg, ...);
		void Error(const char* fmtMsg, ...);
		void Fatal(const char* fmtMsg, ...);
	} // namespace Log
}

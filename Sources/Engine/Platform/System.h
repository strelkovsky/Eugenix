//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include "Core/Base.h"

namespace Eugenix
{
	namespace System
	{
		void Error(const char* format, ...);
		void Exit(int32_t exitCode);
		void Message(const char* format, ...);
		void Sleep(const uint32_t msec);
	} // System
} // Eugenix

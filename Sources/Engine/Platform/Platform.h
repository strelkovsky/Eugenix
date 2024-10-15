//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once
#include "CompileConfig.h"

#ifdef _DEBUG
#	define EUGENIX_DEBUG 
#endif

#ifdef _WIN32
#	define EUGENIX_WINDOWS 
#endif

#ifdef _MSC_VER
#	define EUGENIX_COMPILER_MSVC
#endif

#ifdef UNICODE
#	define EUGENIX_UNICODE
#endif

#ifdef EUGENIX_WINDOWS
#	define EUGENIX_BREAK __debugbreak
#	define EUGENIX_ALIGN(bytes) __declspec(align(bytes))
#	define EUGENIX_INLINE __forceinline
#	define EUGENIX_FUNCTION __FUNCSIG__
#endif
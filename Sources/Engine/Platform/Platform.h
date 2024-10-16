//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include "CompileConfig.h"
#include "DetectCompiler.h"
#include "DetectOS.h"

#ifdef UNICODE
#	define EUGENIX_UNICODE
#endif

#ifdef EUGENIX_PLATFORM_WINDOWS
#	define EUGENIX_BREAK __debugbreak
#	define EUGENIX_ALIGN(bytes) __declspec(align(bytes))
#	define EUGENIX_INLINE __forceinline
#	define EUGENIX_FUNCTION __FUNCSIG__
#endif

#if EUGENIX_DEBUG
//#	define EUGENIX_ASSERT(expr) _ASSERT_EXPR((expr), nullptr)
//#	define EUGENIX_ASSERT_MSG(expr, msg) _ASSERT_EXPR((expr), _CRT_WIDE(msg))
#else
//#	define EUGENIX_ASSERT(expr) ((void)0)
//#	define EUGENIX_ASSERT_MSG(expr, msg) ((void)0)
#endif
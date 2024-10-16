//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

// Platform OS
#define EUGENIX_PLATFORM_WINDOWS        0
#define EUGENIX_PLATFORM_ANDROID        0
#define EUGENIX_PLATFORM_EMSCRIPTEN     0

//=============================================================================
// Platform
//=============================================================================
#if defined(_WIN32) || defined(_WIN64)
#	undef  EUGENIX_PLATFORM_WINDOWS
#	define EUGENIX_PLATFORM_WINDOWS 1
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#	include <sys/cdefs.h> // Defines __BIONIC__ and includes android/api-level.h
#	undef  EUGENIX_PLATFORM_ANDROID
#	define EUGENIX_PLATFORM_ANDROID __ANDROID_API__
#elif defined(__EMSCRIPTEN__)
#	undef  EUGENIX_PLATFORM_EMSCRIPTEN
#	define EUGENIX_PLATFORM_EMSCRIPTEN 1
#endif
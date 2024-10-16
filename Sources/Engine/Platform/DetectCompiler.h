//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

// Architecture
#define EUGENIX_ARCH_32BIT              0
#define EUGENIX_ARCH_64BIT              0

// Debug
#define EUGENIX_DEBUG                   0

// Compiler
#define EUGENIX_COMPILER_MSVC           0

//=============================================================================
// Architecture
//=============================================================================
#if defined(__x86_64__)    || \
	defined(_M_X64)        || \
	defined(__aarch64__)   || \
	defined(__64BIT__)     || \
	defined(__mips64)      || \
	defined(__powerpc64__) || \
	defined(__ppc64__)     || \
	defined(__LP64__)
#	undef  EUGENIX_ARCH_64BIT
#	define EUGENIX_ARCH_64BIT 64
#else
#	undef  EUGENIX_ARCH_32BIT
#	define EUGENIX_ARCH_32BIT 32
#endif

//=============================================================================
// DEBUG MODE
//=============================================================================
#if (defined(_DEBUG) || defined(DEBUG)) && !defined(NDEBUG)
#	undef  EUGENIX_DEBUG
#	define EUGENIX_DEBUG 1
#endif

//=============================================================================
// Compiler
//=============================================================================
#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#	undef  EUGENIX_COMPILER_MSVC
#	define EUGENIX_COMPILER_MSVC _MSC_VER
#else
#	error "Unknown compiler"
#endif
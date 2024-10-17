//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include "Platform/Platform.h"

#define EUGENIX_STRINGIZE( _n ) EGUENIX_STRINGIZE_2( _n )
#define EGUENIX_STRINGIZE_2( _n ) #_n

#if EUGENIX_COMPILER_MSVC
#	define TODO( _msg ) __pragma(message("" __FILE__ "(" EUGENIX_STRINGIZE(__LINE__) "): TODO: " _msg))
#else
#	define TODO( _msg )
#endif

#define EUGENIX_ARRAY_SIZE(a) ((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))
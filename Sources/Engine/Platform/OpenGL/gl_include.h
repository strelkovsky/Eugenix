//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) strelkovsky.com, Inc.
//-----------------------------------------------------------------------------

#pragma once

#include "CompileConfig.h"

#ifdef EUGENIX_OPENGL_GLEW
#	define GLEW_STATIC
#	include <gl\glew.h>
#else
#	include <gl/gl.h>
#	ifdef EUGENIX_PLATFORM_WINDOWS
#		include "gl/glext.h"
#		include "gl/wglext.h"
#	else
#		error "Unknown platform for GL header include!"
#	endif
#endif



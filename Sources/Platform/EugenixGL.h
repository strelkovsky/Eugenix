//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#ifdef EUGENIX_OPENGL

#ifdef EUGENIX_OPENGL_GLEW
#define GLEW_STATIC
	#include <GL/glew.h>
#elif EUGENIX_OPENGL_NATIVE
	#include <gl/GL.h>
	#include <gl/glext.h>
	#include <gl/wglext.h>
#endif

namespace Eugenix
{
	namespace EugenixGL
	{
		bool Init();
	} // namespace EugenixGL
} // namespace Eugenix

#endif // EUGENIX_OPENGL
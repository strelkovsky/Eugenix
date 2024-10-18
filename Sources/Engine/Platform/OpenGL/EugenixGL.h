//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#include "Core/Base.h"
#include "gl_include.h"

#pragma once

#ifdef EUGENIX_DEBUG
#define OPENGL_CALL(expr) \
			expr; \
			while ((Eugenix::EugenixGL::oglError = glGetError()) != GL_NO_ERROR) \
			{\
				printf("%s %s %s $s $s\n", #expr, __FILE__, __FUNCTION__, __LINE__, Eugenix::EugenixGL::GetErrorString(Eugenix::EugenixGL::oglError)); \
			}
#else
#define OPENGL_CALL expr;
#endif

namespace Eugenix
{
	namespace EugenixGL
	{
		extern GLenum oglError;
		const char* GetErrorString(GLenum err);

		bool Init();
	} // namespace EugenixGL
} // namespace Eugenix



//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include "Core/Base.h"

namespace Eugenix
{
	struct DisplaySetup final
	{
		int width = 1024;
		int height = 738;
		bool fullScreen = false;
	};

	namespace Runtime
	{
		bool Init();
		void Term();

		bool RunFrame();
		void CommitFrame();

		void Resize(uint32_t width, uint32_t);
		const DisplaySetup& Setup();
	} // namespace Runtime

	int EugenixMain();
} // namespace Eugenix

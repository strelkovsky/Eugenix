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
		uint32_t width = 1024;
		uint32_t height = 738;
		bool resizable = false;
		bool fullScreen = false;
	};
	namespace Runtime
	{
		bool Init(const DisplaySetup& setup);
		void Term();

		bool RunFrame();
		void CommitFrame();

		void Resize(uint32_t width, uint32_t);
		void ToggleFullScreen(bool fullScreen);

		//void PushEvent(const Event& e);

		const DisplaySetup& Setup();
	} // namespace Runtime

	// TODO
	int AppEntryPoint();
} // namespace Eugenix
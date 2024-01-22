//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include "Platform/Runtime/Runtime.h"

namespace Eugenix
{
	class App
	{
	public:
		int Run()
		{
			if (!Runtime::Init(DisplaySetup()))
				return 0;

			if (!OnInit())
				return 0;

			int status = InternalMainLoop();

			OnShutdown();

			return status;
		}

	protected:
		virtual bool OnInit() { return true; }
		virtual void OnShutdown() {}
		virtual void OnRender() {}
		virtual void OnUpdate(float elapsedTime) {}

	private:
		int InternalMainLoop()
		{
			while (Runtime::RunFrame())
			{
				OnUpdate(0);
				OnRender();

				Runtime::CommitFrame();
			}

			return 0;
		}
	};
} // namespace Eugenix
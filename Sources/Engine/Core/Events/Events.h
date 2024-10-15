//-----------------------------------------------------------------------------
// Eugenix Framework
// Copyright (C) Evgeny Strelkovsky, 2024
//-----------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace Eugenix
{
	namespace Events
	{
		enum struct EventType
		{
			MOUSE_PRESS_LEFT_BUTTON,
			MOUSE_RELEASE_LEFT_BUTTON,
			MOUSE_PRESS_RIGHT_BUTTON,
			MOUSE_RELEASE_RIGHT_BUTTON,
			MOUSE_PRESS_MIDDLE_BUTTON,
			MOUSE_RELEASE_MIDDLE_BUTTON,
			MOUSE_MOVE
		};
		enum struct KeyCode
		{
			A
		};
		void MouseEvent(EventType type, int32_t x, int32_t y) {}
		void KeyDown(KeyCode key) {}
		void KeyUp(KeyCode key) {}
	} // namespace Events
} // namespace Eugenix
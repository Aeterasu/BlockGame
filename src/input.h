#pragma once

#include <SDL2/SDL.h>
#include <cstdint>

namespace blockgame::input
{
	enum class Button : uint8_t
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		BUTTON_1,
		BUTTON_2,
		START,
		SELECT
	};

	void Init();
	void ProcessEvent(const SDL_Event& event);
	void OnTickEnd();

	bool IsButtonJustPressed(Button button);
	bool IsButtonJustReleased(Button button);
	bool IsButtonHeld(Button button);

	void BindKey(Button button, SDL_Keycode key);
} // namespace blockgame::input

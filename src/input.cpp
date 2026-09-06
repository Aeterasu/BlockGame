#include "input.h"

#include <map>
#include <set>

namespace blockgame::input
{
	namespace
	{
		std::multimap<Button, SDL_Keycode> bindings;

		std::set<SDL_Keycode> pressedKeys;
		std::set<SDL_Keycode> justPressedKeys;
		std::set<SDL_Keycode> justReleasedKeys;

		bool initialized = false;

		void DefaultBindings()
		{
			bindings.emplace(Button::UP, SDLK_UP);
			bindings.emplace(Button::UP, SDLK_w);
			bindings.emplace(Button::DOWN, SDLK_DOWN);
			bindings.emplace(Button::DOWN, SDLK_s);
			bindings.emplace(Button::LEFT, SDLK_LEFT);
			bindings.emplace(Button::LEFT, SDLK_a);
			bindings.emplace(Button::RIGHT, SDLK_RIGHT);
			bindings.emplace(Button::RIGHT, SDLK_d);
			bindings.emplace(Button::BUTTON_1, SDLK_z);
			bindings.emplace(Button::BUTTON_2, SDLK_x);
			bindings.emplace(Button::START, SDLK_RETURN);
			bindings.emplace(Button::SELECT, SDLK_BACKSPACE);
		}

		bool AnyKeyInSet(Button button, const std::set<SDL_Keycode>& set)
		{
			auto range = bindings.equal_range(button);
			for (auto it = range.first; it != range.second; ++it)
			{
				if (set.count(it->second))
				{
					return true;
				}
			}
			return false;
		}
	} // namespace

	void Init()
	{
		if (!initialized)
		{
			DefaultBindings();
			initialized = true;
		}
	}

	void OnTickEnd()
	{
		justPressedKeys.clear();
		justReleasedKeys.clear();
	}

	void ProcessEvent(const SDL_Event& event)
	{
		if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
		{
			pressedKeys.insert(event.key.keysym.sym);
			justPressedKeys.insert(event.key.keysym.sym);
		}
		else if (event.type == SDL_KEYUP)
		{
			pressedKeys.erase(event.key.keysym.sym);
			justReleasedKeys.insert(event.key.keysym.sym);
		}
	}

	bool IsButtonJustPressed(Button button)
	{
		return AnyKeyInSet(button, justPressedKeys);
	}

	bool IsButtonJustReleased(Button button)
	{
		return AnyKeyInSet(button, justReleasedKeys);
	}

	bool IsButtonHeld(Button button)
	{
		return AnyKeyInSet(button, pressedKeys);
	}

	void BindKey(Button button, SDL_Keycode key)
	{
		bindings.emplace(button, key);
	}
} // namespace blockgame::input

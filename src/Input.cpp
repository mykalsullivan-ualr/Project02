#include "Input.h"

// Definition of static members
std::set<SDL_Keycode> Input::m_KeysDown;
std::set<Uint8> Input::m_MouseButtonsDown;
std::pair<int, int> Input::m_MousePosition = { 0, 0 };

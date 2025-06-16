#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <unordered_map>

#include "InputBinding.hpp"

// Input State may have different binding list
enum InputState
{
    GlobalState,
    SettingState,
    PlayState
};

class InputManager
{
public:
    InputManager();
    ~InputManager();

    void Update();
    void HandleInput(const sf::Event& event);

    void SetFocus(bool focus) { m_isFocus = focus; }
    void SetInputState(InputState state) { m_state = state; }

private:
    void _InitInputBinding();

private:
    bool m_isFocus;
    InputState m_state;

    std::unordered_map<InputState, std::vector<std::shared_ptr<input::Binding>>> m_bindings;
};

#endif // INPUTMANAGER_HPP
#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <unordered_map>

#include "InputAction.hpp"
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

    bool CheckAction(const std::string_view& action);

private:
    void _InitInputBinding();
    void _InitInputAction();

private:
    bool m_isFocus;
    InputState m_state;

    std::unordered_map<InputState, std::vector<std::shared_ptr<input::binding::Binding>>> m_bindings;
    std::unordered_map<InputState, std::unordered_map<std::string_view, std::shared_ptr<input::action::Action>>> m_actions;
};

#endif // INPUTMANAGER_HPP
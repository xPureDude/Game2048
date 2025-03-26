#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "InputBinding.hpp"
#include "scene/SceneDependent.hpp"

class InputManager : public SceneDependent
{
public:
    InputManager();
    ~InputManager();

    void Update(const sf::Time& elapsed);
    void HandleInput(const sf::Event& event);

    void SetFocus(bool focus);

    bool AddInputBindingCallback(SceneType type, ib::BindType bindType, const std::string_view& name, CallbackType callback);
    void DelInputBindingCallback(SceneType type, ib::BindType bindType, const std::string_view& name);

private:
    bool m_isFocus;
    InputBindingFactory m_inputBindingFactory;
    std::map<SceneType, std::map<ib::BindType, ib::BindingPtr>> m_bindings;
};

#endif // INPUTMANAGER_HPP
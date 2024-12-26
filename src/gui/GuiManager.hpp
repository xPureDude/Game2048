#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include "../pch.hpp" // IWYU pragma: keep
#include "../scene/SceneDependent.hpp"
#include "Element.hpp"

class GuiManager : public SceneDependent
{
public:
    GuiManager(SharedContext* ctx);
    ~GuiManager();

    void Update(const sf::Time& elapsed);
    void HandleInput(const sf::Event& event);
    void Render();

    void ClearSceneGui(SceneType type);

    std::optional<std::vector<gui::ElementPtr>> GetAllSceneElements(SceneType type);
    gui::ElementPtr GetSceneElement(SceneType type, const std::string& name);

private:
    SharedContext* m_ctx;

    sf::RenderTexture m_target;
    sf::Sprite m_sprite;
    std::map<SceneType, std::vector<gui::ElementPtr>> m_elements;
};

#endif // GUIMANAGER_HPP
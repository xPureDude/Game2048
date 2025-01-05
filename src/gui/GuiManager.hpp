#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include "../core/SharedContextDependent.hpp"
#include "../scene/SceneDependent.hpp"
#include "ElementFactory.hpp"

class GuiManager : public SceneDependent, public SharedContextDependent
{
    using ElementContainer = std::vector<std::shared_ptr<gui::Element>>;

public:
    GuiManager(SharedContext* ctx);
    ~GuiManager();

    void Update(const sf::Time& elapsed);
    void HandleInput(const sf::Event& event);
    void Render();

    void ClearSceneGui(SceneType type);
    void AddSceneGui(SceneType type, std::shared_ptr<gui::Element> elem);
    void AddSceneGui(SceneType type, std::vector<std::shared_ptr<gui::Element>>& elem);

    std::optional<ElementContainer> GetAllSceneElements(SceneType type);
    std::shared_ptr<gui::Element> GetSceneElementByName(SceneType type, const std::string& name);

    gui::ElementFactory& GetElementFactory() { return m_factory; }

private:
    gui::ElementFactory m_factory;
    sf::RenderTexture m_target;
    sf::Sprite m_sprite;
    std::map<SceneType, ElementContainer> m_elements;
};

#endif // GUIMANAGER_HPP
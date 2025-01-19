#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include "../core/SharedContextDependent.hpp"
#include "../scene/SceneDependent.hpp"
#include "Button.hpp"
#include "ElementFactory.hpp"
#include "ElementStyle.hpp"
#include "Label.hpp"
#include "Widget.hpp"

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
    gui::StyleSheet* GetStyleSheetByName(const std::string& name);

    gui::ElementFactory& GetElementFactory() { return m_factory; }

    bool LoadStyleSheetsFromFile(const std::string_view& file);
    bool LoadSceneGuiFromFile(SceneType type, const std::string_view& file, const std::map<std::string_view, CallbackType>& callbacks);

private:
    bool _TryLoadEmplaceStyleSheet(gui::StyleSheet* style, tinyxml2::XMLElement* e);
    bool _ParseGuiElements(tinyxml2::XMLElement* e);
    bool _ParseWidget(std::shared_ptr<gui::Widget> widget, tinyxml2::XMLElement* e);
    bool _ParseButton(std::shared_ptr<gui::Button> button, tinyxml2::XMLElement* e);
    bool _ParseLabel(std::shared_ptr<gui::Label> label, tinyxml2::XMLElement* e);

private:
    std::map<std::string, gui::StyleSheet*> m_styleSheets;
    gui::ElementFactory m_factory;
    sf::RenderTexture m_target;
    sf::Sprite m_sprite;
    std::map<SceneType, ElementContainer> m_elements;
};

#endif // GUIMANAGER_HPP
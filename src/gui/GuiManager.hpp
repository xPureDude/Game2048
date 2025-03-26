#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <map>

#include "Button.hpp"
#include "ElementFactory.hpp"
#include "ElementStyle.hpp"
#include "Label.hpp"
#include "Widget.hpp"
#include "common/GlobalDefine.hpp"
#include "scene/SceneDependent.hpp"


struct SceneGuiInfo
{
    SceneType m_type;
    std::string_view m_fileName;
    std::map<std::string_view, CallbackType> m_callbacks;
};

class GuiManager : public SceneDependent
{
    using ElementContainer = std::vector<std::shared_ptr<gui::Element>>;

public:
    GuiManager();
    ~GuiManager();

    void Update(const sf::Time& elapsed);
    void HandleInput(const sf::Event& event);
    void Render();

    void ClearSceneGui(SceneType type);
    void AddSceneGui(SceneType type, std::shared_ptr<gui::Element> elem);
    void AddSceneGui(SceneType type, std::vector<std::shared_ptr<gui::Element>>& elem);

    std::optional<ElementContainer> GetAllSceneElements(SceneType type);
    std::shared_ptr<gui::Element> FindSceneElementByName(SceneType type, const std::string& name);
    gui::StyleSheet* FindStyleSheetByName(const std::string& name);

    gui::ElementFactory& GetElementFactory() { return m_factory; }

    bool LoadStyleSheetsFromFile(const std::string_view& file);
    bool LoadSceneGuiFromFile(const SceneGuiInfo& info);

private:
    bool _ParseGuiElements(const SceneGuiInfo& info, std::shared_ptr<gui::Widget> parent, tinyxml2::XMLElement* e);
    std::shared_ptr<gui::Widget> _ParseWidget(const SceneGuiInfo& info, tinyxml2::XMLElement* e);
    std::shared_ptr<gui::Button> _ParseButton(const SceneGuiInfo& info, tinyxml2::XMLElement* e);
    std::shared_ptr<gui::Label> _ParseLabel(const SceneGuiInfo& info, tinyxml2::XMLElement* e);

private:
    std::map<std::string, gui::StyleSheet*> m_styleSheets;
    gui::ElementFactory m_factory;
    sf::RenderTexture m_target;
    sf::Sprite m_sprite;
    std::map<SceneType, ElementContainer> m_elements;
};

#endif // GUIMANAGER_HPP
#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include <map>

#include "ElementStyle.hpp"

enum class GuiLayerType
{
    None,
    Global,
    MainMenu,
    Setting,
    Play,
    GameOver
};

namespace gui
{
class Layer;
} // namespace gui

class GuiManager
{
public:
    GuiManager();
    ~GuiManager();

    void UpdateFrame(const sf::Time& elapsed);
    void Render();

    gui::Layer* GetGuiLayer(GuiLayerType);

    template <typename T>
    void CreateGuiLayer(GuiLayerType type);
    void ReleaseGuiLayer(GuiLayerType type);

    void SetCurLayerType(GuiLayerType type) { m_curLayerType = type; }

    gui::StyleSheet* FindStyleSheetByName(const std::string& name);
    bool LoadStyleSheetsFromFile(const std::string_view& file);

private:
    GuiLayerType m_curLayerType;
    std::map<GuiLayerType, std::unique_ptr<gui::Layer>> m_layers;
    std::map<std::string, gui::StyleSheet*> m_styleSheets;
    sf::RenderTexture m_context;
};

template <typename T>
void GuiManager::CreateGuiLayer(GuiLayerType type)
{
    m_layers[type] = std::make_unique<T>();
}

#endif // GUIMANAGER_HPP
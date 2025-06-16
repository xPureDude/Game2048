#include "GuiManager.hpp"

#include "tinyxml/tinyxml2.h"

#include "ElementStyle.hpp"
#include "GuiLayer.hpp"
#include "common/Log.hpp"
#include "core/ConfigManager.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"

GuiManager::GuiManager()
    : m_context({SharedContext::Instance()->Get<ConfigManager>()->GetWindowConfig().m_width,
                 SharedContext::Instance()->Get<ConfigManager>()->GetWindowConfig().m_height})
{
}

GuiManager::~GuiManager()
{
    for (auto it : m_styleSheets)
    {
        if (it.second)
        {
            delete it.second;
        }
    }
    m_styleSheets.clear();
}

void GuiManager::UpdateFrame(const sf::Time& elapsed)
{
    if (m_layers.contains(GuiLayerType::Global))
    {
        m_layers[GuiLayerType::Global]->UpdateFrame(elapsed);
    }

    if (m_layers.contains(m_curLayerType))
    {
        m_layers[m_curLayerType]->UpdateFrame(elapsed);
    }
}

void GuiManager::Render()
{
    m_context.clear(sf::Color::Transparent);
    Window* window = SharedContext::Instance()->Get<Window>();

    if (m_layers.contains(m_curLayerType))
    {
        m_layers[m_curLayerType]->Render(&m_context);
    }

    if (m_layers.contains(GuiLayerType::Global))
    {
        m_layers[GuiLayerType::Global]->Render(&m_context);
    }

    m_context.display();
    auto view = window->GetViewSpace();
    sf::Sprite sprite(m_context.getTexture());
    sprite.setPosition(view.position);

    window->Render(sprite);
}

void GuiManager::ReleaseGuiLayer(GuiLayerType type)
{
    if (m_layers.contains(type))
    {
        m_layers.erase(type);
        if (m_curLayerType == type)
        {
            SetCurLayerType(GuiLayerType::None);
        }
    }
}

gui::StyleSheet* GuiManager::FindStyleSheetByName(const std::string& name)
{
    if (m_styleSheets.contains(name))
        return m_styleSheets[name];
    return nullptr;
}

bool GuiManager::LoadStyleSheetsFromFile(const std::string_view& file)
{
    tinyxml2::XMLDocument doc;
    if (tinyxml2::XMLError err = doc.LoadFile(file.data()))
    {
        ERR("GuiManager::LoadStyleSheetsFromFile, Load file failed, file: {}, ErrorID: {}", file, static_cast<std::int32_t>(err));
        return false;
    }

    auto rootElement = doc.RootElement();
    if (!rootElement)
    {
        ERR("GuiManager::LoadStyleSheetsFromFile, rootElement is nullptr, file: {}", file);
        return false;
    }

    if (std::strcmp(rootElement->Name(), "StyleSheet"))
    {
        ERR("GuiManager::LoadStyleSheetsFromFile, rootElement is not <StyleSheet>, file: {}", file);
        return false;
    }

    for (auto e = rootElement->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        gui::StyleSheet* style = nullptr;
        if (std::strcmp(e->Name(), "WidgetStyle") == 0)
        {
            style = new gui::WidgetStyle;
        }
        else if (std::strcmp(e->Name(), "ButtonStyle") == 0)
        {
            style = new gui::ButtonStyle;
        }
        else if (std::strcmp(e->Name(), "LabelStyle") == 0)
        {
            style = new gui::LabelStyle;
        }
        else if (std::strcmp(e->Name(), "TextStyle") == 0)
        {
            style = new gui::TextStyle;
        }
        else
        {
            DBG("GuiManager::LoadStyleSheetsFromFile, Unknown element name: <{}>, file: {}", e->Name(), file);
            return false;
        }

        if (!style->LoadFromXmlElement(e))
        {
            DBG("GuiManager::_TryLoadEmplaceStyleSheet, StyleSheet load failed");
            delete style;
            return false;
        }

        auto result = m_styleSheets.try_emplace(style->m_name, style);
        if (!result.second)
        {
            DBG("GuiManager::_TryLoadEmplaceStyleSheet, duplicate StyleSheet name: {}", style->m_name);
            delete style;
            return false;
        }
    }

    return true;
}

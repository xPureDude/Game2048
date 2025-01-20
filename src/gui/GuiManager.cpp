#include "GuiManager.hpp"

#include "../common/Log.hpp"
#include "../core/ConfigManager.hpp"
#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../gui/Widget.hpp"
#include "../pch.hpp"
#include "../tinyxml/tinyxml2.h"
#include "ElementStyle.hpp"

GuiManager::GuiManager(SharedContext* ctx)
    : SceneDependent(),
      SharedContextDependent(ctx),
      m_target({m_ctx->Get<ConfigManager>()->GetWindowConfig().m_width, m_ctx->Get<ConfigManager>()->GetWindowConfig().m_height}),
      m_sprite(m_target.getTexture())
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
    m_elements.clear();
}

void GuiManager::Update(const sf::Time& elapsed)
{
    for (auto& elem : m_elements[SceneType::None])
    {
        elem->Update(elapsed);
    }

    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->Update(elapsed);
    }
}

void GuiManager::HandleInput(const sf::Event& event)
{
    for (auto& elem : m_elements[SceneType::None])
    {
        if (elem->HandleInput(event))
        {
            return;
        }
    }

    for (auto& elem : m_elements[s_curSceneType])
    {
        if (elem->HandleInput(event))
        {
            return;
        }
    }
}

void GuiManager::Render()
{
    m_target.clear(sf::Color::Transparent);
    Window* window = m_ctx->Get<Window>();
    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->Render(&m_target);
    }

    for (auto& elem : m_elements[SceneType::None])
    {
        elem->Render(&m_target);
    }
    m_target.display();
    auto view = window->GetViewSpace();
    m_sprite.setPosition(view.position);

    window->Render(m_sprite);
}

void GuiManager::ClearSceneGui(SceneType type)
{
    auto iter = m_elements.find(type);
    if (iter != m_elements.end())
    {
        m_elements.erase(iter);
    }
}

void GuiManager::AddSceneGui(SceneType type, std::shared_ptr<gui::Element> elem)
{
    m_elements[type].emplace_back(elem);
}

void GuiManager::AddSceneGui(SceneType type, std::vector<std::shared_ptr<gui::Element>>& elem)
{
    for (auto& elemPtr : elem)
    {
        m_elements[type].emplace_back(elemPtr);
    }
}

std::optional<std::vector<std::shared_ptr<gui::Element>>> GuiManager::GetAllSceneElements(SceneType type)
{
    std::optional<ElementContainer> elements = std::nullopt;
    if (m_elements.find(type) != m_elements.end())
    {
        elements = m_elements[type];
    }
    return elements;
}

std::shared_ptr<gui::Element> GuiManager::GetSceneElementByName(SceneType type, const std::string& name)
{
    if (m_elements.find(type) != m_elements.end())
    {
        for (auto& item : m_elements[type])
        {
            if (item->GetName() == name)
            {
                return item;
            }
            else if (auto widget = std::dynamic_pointer_cast<gui::Widget>(item))
            {
                if (auto child = widget->FindChild(name))
                {
                    return child;
                }
            }
        }
    }
    return nullptr;
}

gui::StyleSheet* GuiManager::GetStyleSheetByName(const std::string& name)
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
        if (std::strcmp(e->Name(), "WidgetStyle") == 0)
        {
            auto style = new gui::WidgetStyle;
            if (!_TryLoadEmplaceStyleSheet(style, e))
            {
                DBG("GuiManager::LoadStyleSheetsFromFile, Load WidgetStyle failed, name: {}", style->m_name);
                delete style;
                return false;
            }
        }
        else if (std::strcmp(e->Name(), "ButtonStyle") == 0)
        {
            auto style = new gui::ButtonStyle;
            if (!_TryLoadEmplaceStyleSheet(style, e))
            {
                DBG("GuiManager::LoadStyleSheetsFromFile, Load ButtonStyle failed, name: {}", style->m_name);
                delete style;
                return false;
            }
        }
        else if (std::strcmp(e->Name(), "LabelStyle") == 0)
        {
            auto style = new gui::LabelStyle;

            if (!_TryLoadEmplaceStyleSheet(style, e))
            {
                DBG("GuiManager::LoadStyleSheetsFromFile, Load LabelStyle failed, name: {}", style->m_name);
                delete style;
                return false;
            }
        }
        else if (std::strcmp(e->Name(), "TextStyle") == 0)
        {
            auto style = new gui::TextStyle;

            if (!_TryLoadEmplaceStyleSheet(style, e))
            {
                DBG("GuiManager::LoadStyleSheetsFromFile, Load WidgetStyle failed, name: {}", style->m_name);
                delete style;
                return false;
            }
        }
        else
        {
            DBG("GuiManager::LoadStyleSheetsFromFile, Unknown element name: <{}>, file: {}", e->Name(), file);
            return false;
        }
    }

    return true;
}

bool GuiManager::LoadSceneGuiFromFile(SceneType type, const std::string_view& file, const std::map<std::string_view, CallbackType>& callbacks)
{
    tinyxml2::XMLDocument doc;
    if (tinyxml2::XMLError err = doc.LoadFile(file.data()))
    {
        ERR("GuiManager::LoadSceneGuiFromFile, Load file failed, file: {}, ErrorID: {}", file, static_cast<std::int32_t>(err));
        return false;
    }

    for (auto e = doc.RootElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (std::strcmp(e->Name(), "Elements") == 0)
        {
            if (!_ParseGuiElements(e))
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse GuiElements failed file: {}", file);
            }
        }
        else
        {
            DBG("GuiManager::LoadSceneGuiFromFile, Unknown element name: <{}>, file: {}", e->Name(), file);
        }
    }

    return true;
}

bool GuiManager::_TryLoadEmplaceStyleSheet(gui::StyleSheet* style, tinyxml2::XMLElement* e)
{
    if (!style->LoadFromXmlElement(e, m_ctx))
    {
        DBG("GuiManager::_TryLoadEmplaceStyleSheet, StyleSheet load failed");
        return false;
    }

    auto result = m_styleSheets.try_emplace(style->m_name, style);
    if (!result.second)
    {
        DBG("GuiManager::_TryLoadEmplaceStyleSheet, duplicate StyleSheet name: {}", style->m_name);
        return false;
    }
    return true;
}

bool GuiManager::_ParseGuiElements(tinyxml2::XMLElement* e)
{
    for (auto elem = e->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement())
    {
        if (std::strcmp(elem->Name(), "Widget") == 0)
        {
            auto widget = std::dynamic_pointer_cast<gui::Widget>(m_factory.CreateElement(gui::ElementType::Widget, ""));
            if (!_ParseWidget(widget, elem))
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse Widget failed");
                return false;
            }
        }
        else if (std::strcmp(elem->Name(), "Button") == 0)
        {
            auto button = std::dynamic_pointer_cast<gui::Button>(m_factory.CreateElement(gui::ElementType::Button, ""));
            if (!_ParseButton(button, elem))
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse Button failed");
                return false;
            }
        }
        else if (std::strcmp(elem->Name(), "Label") == 0)
        {
            auto label = std::dynamic_pointer_cast<gui::Label>(m_factory.CreateElement(gui::ElementType::Label, ""));
            if (!_ParseLabel(label, elem))
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse Button failed");
                return false;
            }
        }
        else
        {
            DBG("GuiManager::LoadStyleSheetsFromFile, Unknown element name: <{}>", e->Name());
            return false;
        }
    }

    return true;
}

bool GuiManager::_ParseWidget(std::shared_ptr<gui::Widget> widget, tinyxml2::XMLElement* e)
{
    auto attr = e->FindAttribute("name");
    if (!attr)
    {
        DBG("GuiManager::LoadSceneGuiFromFile, Load Widget failed");
        return false;
    }
    widget->SetName(attr->Value());

    return true;
}

bool GuiManager::_ParseButton(std::shared_ptr<gui::Button> button, tinyxml2::XMLElement* e)
{
    return true;
}

bool GuiManager::_ParseLabel(std::shared_ptr<gui::Label> label, tinyxml2::XMLElement* e)
{
    return true;
}
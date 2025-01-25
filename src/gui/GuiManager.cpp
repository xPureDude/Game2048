#include "GuiManager.hpp"

#include "../common/Log.hpp"
#include "../core/ConfigManager.hpp"
#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../gui/Widget.hpp"
#include "../pch.hpp"
#include "../tinyxml/tinyxml2.h"
#include "ElementStyle.hpp"

std::optional<sf::FloatRect> ParseFloatRect(tinyxml2::XMLElement* e)
{
    sf::FloatRect rect;

    if ((e->QueryFloatAttribute("x", &rect.position.x) != tinyxml2::XML_SUCCESS)
        || (e->QueryFloatAttribute("y", &rect.position.y) != tinyxml2::XML_SUCCESS)
        || (e->QueryFloatAttribute("w", &rect.size.x) != tinyxml2::XML_SUCCESS)
        || (e->QueryFloatAttribute("h", &rect.size.y) != tinyxml2::XML_SUCCESS))
    {
        return std::optional<sf::FloatRect>();
    }

    return rect;
}

bool ParseSignalTrigger(std::shared_ptr<gui::Element> elem, const SceneGuiInfo& info, tinyxml2::XMLElement* e)
{
    // Parse SignalTrigger
    for (auto childElem = e->FirstChildElement(); childElem != nullptr; childElem = childElem->NextSiblingElement())
    {
        if (std::strcmp(childElem->Name(), "SignalTrigger") == 0)
        {
            auto signalAttr = childElem->FindAttribute("signal");
            auto callbackAttr = childElem->FindAttribute("callback");
            if (!signalAttr || !callbackAttr)
            {
                DBG("ParseSignalTrigger, failed to parse SignalTrigger");
                return false;
            }
            auto sig = gui::TranslateStringToSignal(signalAttr->Value());
            if (!sig)
            {
                DBG("ParseSignalTrigger, Signal not support: {}", signalAttr->Value());
                return false;
            }

            auto it = info.m_callbacks.find(callbackAttr->Value());
            if (it == info.m_callbacks.end())
            {
                DBG("ParseSignalTrigger, Callback not support: {}", callbackAttr->Value());
                return false;
            }

            elem->ConnectSignalCallback(sig.value(), callbackAttr->Value(), it->second);
        }
        else
        {
            DBG("BindingSignalCallbacks, Unknown Child Element: {}", childElem->Name());
            return false;
        }
    }
    return true;
}

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
    DBG("GuiManager::AddSceneGui, emplace new elem, name: {}", elem->GetName());
}

void GuiManager::AddSceneGui(SceneType type, std::vector<std::shared_ptr<gui::Element>>& elems)
{
    for (auto& elem : elems)
    {
        m_elements[type].emplace_back(elem);
        DBG("GuiManager::AddSceneGui, emplace new elem, name: {}", elem->GetName());
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

std::shared_ptr<gui::Element> GuiManager::FindSceneElementByName(SceneType type, const std::string& name)
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

        if (!style->LoadFromXmlElement(e, m_ctx))
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

bool GuiManager::LoadSceneGuiFromFile(const SceneGuiInfo& info)
{
    tinyxml2::XMLDocument doc;
    if (tinyxml2::XMLError err = doc.LoadFile(info.m_fileName.data()))
    {
        ERR("GuiManager::LoadSceneGuiFromFile, Load file failed, file: {}, ErrorID: {}", info.m_fileName, static_cast<std::int32_t>(err));
        return false;
    }

    for (auto e = doc.RootElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (std::strcmp(e->Name(), "Elements") == 0)
        {
            if (!_ParseGuiElements(info, nullptr, e))
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse GuiElements failed file: {}", info.m_fileName);
                return false;
            }
        }
        else
        {
            DBG("GuiManager::LoadSceneGuiFromFile, Unknown element name: <{}>, file: {}", e->Name(), info.m_fileName);
            return false;
        }
    }

    return true;
}

bool GuiManager::_ParseGuiElements(const SceneGuiInfo& info, std::shared_ptr<gui::Widget> parent, tinyxml2::XMLElement* e)
{
    for (auto elem = e->FirstChildElement(); elem != nullptr; elem = elem->NextSiblingElement())
    {
        std::shared_ptr<gui::Element> guiElem;
        if (std::strcmp(elem->Name(), "Widget") == 0)
        {
            if (guiElem = _ParseWidget(info, elem); !guiElem)
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse Widget failed");
                return false;
            }
        }
        else if (std::strcmp(elem->Name(), "Button") == 0)
        {
            if (guiElem = _ParseButton(info, elem); !guiElem)
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse Button failed");
                return false;
            }
        }
        else if (std::strcmp(elem->Name(), "Label") == 0)
        {
            if (guiElem = _ParseLabel(info, elem); !guiElem)
            {
                DBG("GuiManager::LoadSceneGuiFromFile, Parse Button failed");
                return false;
            }
        }
        else
        {
            DBG("GuiManager::LoadSceneGuiFromFile, Unknown element name: <{}>", elem->Name());
            return false;
        }

        if (std::strcmp(elem->Name(), "Widget") != 0 && !ParseSignalTrigger(guiElem, info, elem))
        {
            DBG("GuiManager::LoadSceneGuiFromFile, ParseSignalTrigger failed: <{}>", guiElem->GetName());
            return false;
        }

        if (parent)
        {
            parent->AppendChild(guiElem);
            guiElem->SetParent(parent);
        }
        else
        {
            AddSceneGui(info.m_type, guiElem);
        }
    }

    return true;
}

std::shared_ptr<gui::Widget> GuiManager::_ParseWidget(const SceneGuiInfo& info, tinyxml2::XMLElement* e)
{

    auto nameAttr = e->FindAttribute("name");
    if (!nameAttr)
    {
        DBG("GuiManager::_ParseWidget, attr 'name' not found");
        return nullptr;
    }

    auto styleAttr = e->FindAttribute("style");
    if (!styleAttr)
    {
        DBG("GuiManager::_ParseWidget, attr 'style' not found");
        return nullptr;
    }
    auto style = static_cast<gui::WidgetStyle*>(FindStyleSheetByName(styleAttr->Value()));
    if (!style)
    {
        DBG("GuiManager::_ParseWidget, stylesheet not fount: {}", styleAttr->Value());
        return nullptr;
    }

    auto optRect = ParseFloatRect(e);
    if (!optRect)
    {
        DBG("GuiManager::_ParseWidget, parse attr 'x y w h' failed");
        return nullptr;
    }

    std::shared_ptr<gui::Widget> widget = m_factory.CreateElement<gui::Widget>();
    widget->SetName(nameAttr->Value());
    widget->SetWidgetStyle(style);
    widget->SetPosition(optRect->position);
    widget->SetSize(optRect->size);

    if (auto childElem = e->FirstChildElement(); childElem)
    {
        if (!_ParseGuiElements(info, widget, e))
        {
            DBG("GuiManager::_ParseWidget, parse child elements failed");
            return nullptr;
        }
    }
    return widget;
}

std::shared_ptr<gui::Button> GuiManager::_ParseButton(const SceneGuiInfo& info, tinyxml2::XMLElement* e)
{
    auto nameAttr = e->FindAttribute("name");
    if (!nameAttr)
    {
        DBG("GuiManager::_ParseButton, attr 'name' not found");
        return nullptr;
    }

    auto optRect = ParseFloatRect(e);
    if (!optRect)
    {
        DBG("GuiManager::_ParseButton, parse attr 'x y w h' failed");
        return nullptr;
    }

    auto normalAttr = e->FindAttribute("normal");
    if (!normalAttr)
    {
        DBG("GuiManager::_ParseButton, attr 'normal' not found");
        return nullptr;
    }
    auto normalStyle = static_cast<gui::ButtonStyle*>(FindStyleSheetByName(normalAttr->Value()));
    if (!normalStyle)
    {
        DBG("GuiManager::_ParseButton, StyleSheet '{}' not found", normalAttr->Value());
        return nullptr;
    }

    auto hoverAttr = e->FindAttribute("hover");
    if (!hoverAttr)
    {
        DBG("GuiManager::_ParseButton, attr 'hover' not found");
        return nullptr;
    }
    auto hoverStyle = static_cast<gui::ButtonStyle*>(FindStyleSheetByName(hoverAttr->Value()));
    if (!hoverStyle)
    {
        DBG("GuiManager::_ParseButton, StyleSheet '{}' not found", hoverAttr->Value());
        return nullptr;
    }

    auto pressAttr = e->FindAttribute("press");
    if (!pressAttr)
    {
        DBG("GuiManager::_ParseButton, attr 'press' not found");
        return nullptr;
    }
    auto pressStyle = static_cast<gui::ButtonStyle*>(FindStyleSheetByName(pressAttr->Value()));
    if (!pressStyle)
    {
        DBG("GuiManager::_ParseButton, StyleSheet '{}' not found", pressAttr->Value());
        return nullptr;
    }

    auto textAttr = e->FindAttribute("text");
    auto textStrAttr = e->FindAttribute("textStr");
    if ((textAttr && !textStrAttr) || (!textAttr && textStrAttr))
    {
        DBG("GuiManager::_ParseButton, attr 'text' or 'textStr' not found");
        return nullptr;
    }

    std::shared_ptr<gui::Button> button = m_factory.CreateElement<gui::Button>();
    button->SetName(nameAttr->Value());
    button->SetPosition(optRect->position);
    button->SetSize(optRect->size);
    button->SetButtonStyle(normalStyle, hoverStyle, pressStyle, nullptr);
    if (textAttr && textStrAttr)
    {
        auto textStyle = static_cast<gui::TextStyle*>(FindStyleSheetByName(textAttr->Value()));
        if (!textStyle)
        {
            DBG("GuiManager::_ParseButton, StyleSheet '{}' not found", textAttr->Value());
            return nullptr;
        }
        button->SetText(textStrAttr->Value());
        button->SetTextStyle(textStyle);
    }

    return button;
}

std::shared_ptr<gui::Label> GuiManager::_ParseLabel(const SceneGuiInfo& info, tinyxml2::XMLElement* e)
{
    auto nameAttr = e->FindAttribute("name");
    if (!nameAttr)
    {
        DBG("GuiManager::_ParseLabel, attr 'name' not found");
        return nullptr;
    }

    auto optRect = ParseFloatRect(e);
    if (!optRect)
    {
        DBG("GuiManager::_ParseLabel, parse attr 'x y w h' failed");
        return nullptr;
    }

    auto styleAttr = e->FindAttribute("style");
    if (!styleAttr)
    {
        DBG("GuiManager::_ParseLabel, attr 'style' not found");
        return nullptr;
    }
    auto style = static_cast<gui::LabelStyle*>(FindStyleSheetByName(styleAttr->Value()));
    if (!style)
    {
        DBG("GuiManager::_ParseButton, StyleSheet '{}' not found", styleAttr->Value());
        return nullptr;
    }

    auto textAttr = e->FindAttribute("text");
    auto textStrAttr = e->FindAttribute("textStr");
    if ((textAttr && !textStrAttr) || (!textAttr && textStrAttr))
    {
        DBG("GuiManager::_ParseLabel, attr 'text' or 'textStr' not found");
        return nullptr;
    }

    std::shared_ptr<gui::Label> label = m_factory.CreateElement<gui::Label>();
    label->SetName(nameAttr->Value());
    label->SetPosition(optRect->position);
    label->SetSize(optRect->size);
    label->SetLabelStyle(style);

    if (textAttr && textStrAttr)
    {
        auto textStyle = static_cast<gui::TextStyle*>(FindStyleSheetByName(textAttr->Value()));
        if (!textStyle)
        {
            DBG("GuiManager::_ParseButton, StyleSheet '{}' not found", textAttr->Value());
            return nullptr;
        }
        label->SetText(textStrAttr->Value());
        label->SetTextStyle(textStyle);
    }

    return label;
}
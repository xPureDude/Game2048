#include "GuiLayer.hpp"

#include "Element.hpp"
#include "GuiComponent.hpp"
#include "GuiManager.hpp"
#include "event/GlobalEvent.hpp"
#include "event/InputEvent.hpp"

namespace gui
{

Layer::Layer()
    : m_isActivate(false)
{
    SubscribeEvent<evt::MouseMove>(BindCallback(&Layer::OnEventMouseMove));
    SubscribeEvent<evt::MouseButtonDown>(BindCallback(&Layer::OnEventMouseDown));
    SubscribeEvent<evt::MouseButtonUp>(BindCallback(&Layer::OnEventMouseUp));

    SubscribeEvent<evt::LanguageChanged>(BindCallback(&Layer::_OnLanguageChanged));
}

Layer::~Layer()
{
}

void Layer::UpdateFrame(const sf::Time& elapsed)
{
    for (auto& elem : m_elements)
    {
        elem->UpdateFrame(elapsed);
    }
}

void Layer::Render(sf::RenderTarget* target)
{
    for (auto& elem : m_elements)
    {
        elem->Render(target);
    }
}

void Layer::SetActivate(bool flag)
{
    m_eventListener.SetActivate(flag);
}

std::shared_ptr<gui::Element> Layer::AddElement(const std::string& name)
{
    auto elem = std::make_shared<gui::Element>(name);
    m_elements.push_back(elem);
    return elem;
}

void Layer::DelElement(const std::string& name)
{
    for (auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
    {
        if ((*iter)->GetName() == name)
        {
            m_elements.erase(iter);
            break;
        }
    }
}

void Layer::OnEventMouseMove(evt::Base* event)
{
    auto mouseMove = static_cast<evt::MouseMove*>(event);
    for (auto& elem : m_elements)
    {
        auto baseCompo = elem->GetComponent<ElementBaseComponent>();
        if (!baseCompo)
            continue;

        if (!baseCompo->GetActivate())
            continue;

        auto transCompo = elem->GetComponent<TransformComponent>();
        if (!transCompo)
            continue;

        auto state = baseCompo->GetElementState();
        if (transCompo->IsIntersectWith(mouseMove->m_curPos))
        {
            if (state == gui::ElementState::Normal)
            {
                baseCompo->SetElementState(gui::ElementState::Hover);
                elem->OnMouseHover();
                break;
            }
        }
        else
        {
            if (state != gui::ElementState::Normal)
            {
                baseCompo->SetElementState(gui::ElementState::Normal);
                elem->OnMouseLeave();
                break;
            }
        }
    }
}

void Layer::OnEventMouseDown(evt::Base* event)
{
    auto mouseDown = static_cast<evt::MouseButtonDown*>(event);
    for (auto& elem : m_elements)
    {
        auto baseCompo = elem->GetComponent<ElementBaseComponent>();
        if (!baseCompo)
            continue;

        if (!baseCompo->GetActivate())
            continue;

        if (baseCompo->GetElementState() == gui::ElementState::Hover)
        {
            baseCompo->SetElementState(gui::ElementState::Pressed);
            elem->OnMousePress();
            break;
        }
    }
}

void Layer::OnEventMouseUp(evt::Base* event)
{
    auto mouseUp = static_cast<evt::MouseButtonUp*>(event);
    for (auto& elem : m_elements)
    {
        auto baseCompo = elem->GetComponent<ElementBaseComponent>();
        if (!baseCompo)
            continue;

        if (!baseCompo->GetActivate())
            continue;

        auto transCompo = elem->GetComponent<TransformComponent>();
        if (!transCompo)
            continue;

        if (baseCompo->GetElementState() == gui::ElementState::Pressed)
        {
            baseCompo->SetElementState(gui::ElementState::Hover);
            elem->OnMouseRelease();
            break;
        }
    }
}

void Layer::OnEventMouseScroll(evt::Base* event)
{
    /*
    auto mouseDown = static_cast<evt::MouseButtonDown*>(event);
    for (auto& elem : m_elements)
    {
        auto transCompo = elem->GetComponent<TransformComponent>();
        if (!transCompo)
            continue;

        auto baseCompo = elem->GetComponent<ElementBaseComponent>();
        if (!baseCompo)
            continue;

        if (baseCompo->GetElementState() == gui::ElementState::Hover)
        {
            baseCompo->SetElementState(gui::ElementState::Pressed);
            elem->OnMousePress();
            break;
        }
    }
    */
}

void Layer::_OnLanguageChanged(evt::Base* event)
{
    auto langChanged = static_cast<evt::LanguageChanged*>(event);
    for (auto& elem : m_elements)
    {
        auto textComp = elem->GetComponent<TextComponent>();
        if (textComp)
        {
            elem->SetNeedUpdateContext(true);
        }
    }
}

} // namespace gui
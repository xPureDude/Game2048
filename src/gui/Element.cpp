#include "Element.hpp"

#include "GuiComponent.hpp"

namespace gui
{

std::optional<Signal> TranslateStringToSignal(const std::string& str)
{
    std::optional<Signal> ret;
    if (str == "OnLeave")
        ret = Signal::OnLeave;
    else if (str == "OnHover")
        ret = Signal::OnHover;
    else if (str == "OnPress")
        ret = Signal::OnPress;
    else if (str == "OnRelease")
        ret = Signal::OnRelease;
    else if (str == "OnClicked")
        ret = Signal::OnClicked;

    return ret;
}

Element::Element(const std::string& name)
    : m_name(name),
      m_needReRender(false),
      m_needUpdateContext(false)
{
}

Element::~Element()
{
}

void Element::UpdateFrame(const sf::Time& elapsed)
{
    if (m_needUpdateContext)
    {
        UpdateContext();
        m_needUpdateContext = false;
    }

    for (auto& compo : m_components)
    {
        compo->UpdateFrame(elapsed);
    }
}

void Element::Render(sf::RenderTarget* target)
{
    for (auto& compo : m_components)
    {
        compo->Render(target);
    }
}

void Element::UpdateContext()
{
    for (auto& compo : m_components)
    {
        compo->UpdateContext();
    }
}

bool Element::ConnectSignalCallback(Signal sig, const std::string_view& name, CallbackType callback)
{
    auto& callbacks = m_callbacks[sig];

    auto ret = callbacks.try_emplace(name, callback);
    return ret.second;
}

void Element::DisConnectSignalCallback(Signal sig, const std::string_view& name)
{
    if (m_callbacks.find(sig) == m_callbacks.end())
        return;

    auto& callbacks = m_callbacks[sig];
    auto iter = callbacks.find(name);

    if (iter != callbacks.end())
        callbacks.erase(iter);
}

sf::Vector2f Element::GetGlobalPosition()
{
    auto localPosition = GetLocalPosition();
    if (auto baseCompo = GetComponent<ElementBaseComponent>(); baseCompo)
    {
        auto parent = baseCompo->GetParent();
        if (auto ptr = parent.lock())
        {
            return ptr->GetGlobalPosition() + localPosition;
        }
    }

    return localPosition;
}

sf::Vector2f Element::GetLocalPosition()
{
    if (auto myTransform = GetComponent<TransformComponent>(); myTransform)
    {
        return myTransform->GetPosition();
    }
    return {0, 0};
}

void Element::OnMouseHover()
{
    for (auto& compo : m_components)
    {
        compo->OnMouseHover();
    }
}

void Element::OnMousePress()
{
    for (auto& compo : m_components)
    {
        compo->OnMousePress();
    }
}

void Element::OnMouseRelease()
{
    for (auto& compo : m_components)
    {
        compo->OnMouseRelease();
    }
}

void Element::OnMouseLeave()
{
    for (auto& compo : m_components)
    {
        compo->OnMouseLeave();
    }
}

} // namespace gui
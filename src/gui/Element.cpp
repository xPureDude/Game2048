#include "Element.hpp"

#include "Widget.hpp"

namespace gui
{

Element::Element()
    : m_activate(true),
      m_state(ElementState::Default)
{
}

Element::~Element()
{
}

void Element::Update(const sf::Time& elapsed)
{
    _ProcessCallback();
}

bool Element::HandleInput(const sf::Event& event)
{
    if (event.visit(*this))
        return true;
    return false;
}

void Element::Render(sf::RenderTarget* target)
{
    _RenderPrimitive(target);
    if (m_text)
    {
        target->draw(*m_text);
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

void Element::SetParent(std::shared_ptr<Element> parent)
{
    if (!m_parent.expired())
    {
        m_parent.lock()->RemoveChild(m_name);
    }
    m_parent = std::dynamic_pointer_cast<gui::Widget>(parent);

    if (!m_parent.expired())
    {
        m_parent.lock()->SetRedraw(true);
    }
}

void Element::SetPosition(const sf::Vector2f& pos)
{
    m_position = pos;
    _UpdatePosition();

    if (m_text)
    {
        m_text->setPosition(m_position + m_size / 2.f);
    }

    _RedrawParent();
}

void Element::SetSize(const sf::Vector2f& size)
{
    m_size = size;
    _UpdateSize();

    _RedrawParent();
}

void Element::SetTextInfo(const TextInfo& info)
{
    m_textInfo = info;
    _UpdateText();

    _RedrawParent();
}

sf::Vector2f Element::GetGlobalPosition()
{
    if (!m_parent.expired())
    {
        return m_parent.lock()->GetGlobalPosition() + m_position;
    }
    return m_position;
}

sf::Vector2f Element::GetLocalPosition()
{
    return m_position;
}

void Element::_UpdateText()
{
    if (!m_text)
    {
        m_text = std::make_shared<sf::Text>(*(m_textInfo.m_font));
    }

    m_text->setString(m_textInfo.m_textStr);
    m_text->setCharacterSize(m_textInfo.m_charSize);
    m_text->setFillColor(m_textInfo.m_color);
    m_text->setStyle(m_textInfo.m_style);
    m_text->setOrigin(m_text->getLocalBounds().getCenter());
    m_text->setPosition(m_position + m_size / 2.f);
}

void Element::_RedrawParent()
{
    if (!m_parent.expired())
    {
        m_parent.lock()->SetRedraw(true);
    }
}

void Element::_ProcessCallback()
{
    for (auto& info : m_signalQueue)
    {
        if (m_callbacks.find(info.m_sig) == m_callbacks.end())
            continue;
        auto& callbacks = m_callbacks[info.m_sig];
        for (auto& callback : callbacks)
        {
            callback.second(info.m_data);
        }
    }
    m_signalQueue.clear();
}

} // namespace gui
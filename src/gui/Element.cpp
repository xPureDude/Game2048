#include "Element.hpp"

#include "Widget.hpp"

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
    if (event.visit<InputVisitorDependent&>(*this))
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

bool Element::operator()(const sf::Event::MouseMoved& event)
{
    sf::Rect rect{GetGlobalPosition(), m_size};
    if (rect.contains(sf::Vector2f{event.position}))
    {
        if (m_state == ElementState::Default)
        {
            m_state = ElementState::Hover;
            _UpdateCurrentState();
            m_signalQueue.emplace_back(Signal::OnHover, std::any());
            DBG("Element '{}' OnHover", m_name);
        }
    }
    else
    {
        if (m_state == ElementState::Pressed || m_state == ElementState::Hover)
        {
            m_state = ElementState::Default;
            _UpdateCurrentState();
            m_signalQueue.emplace_back(Signal::OnLeave, std::any());
            DBG("Element '{}' OnDefault", m_name);
        }
    }

    return false;
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

    if (m_text)
    {
        m_text->setPosition(m_position + m_size / 2.f);
    }

    _RedrawParent();
}

void Element::SetText(const std::string& textStr)
{
    m_textStr = textStr;

    _UpdateText();
    _RedrawParent();
}

void Element::SetTextStyle(TextStyle* style)
{
    m_textStyle = *style;
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
        if (!m_textStyle.m_font)
            return;
        m_text = std::make_shared<sf::Text>(*(m_textStyle.m_font));
    }

    m_text->setString(m_textStr);
    m_text->setCharacterSize(m_textStyle.m_charSize);
    m_text->setFillColor(m_textStyle.m_color);
    m_text->setStyle(m_textStyle.m_style);
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
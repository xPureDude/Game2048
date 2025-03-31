#include "Widget.hpp"

#include "pch.hpp" // IWYU pragma: keep

#include "common/Log.hpp"

namespace gui
{

Widget::Widget()
    : Element(),
      m_needRedraw(true),
      m_panelSprite(m_panelTexture.getTexture())
{
}

Widget::~Widget()
{
}

void Widget::Update(const sf::Time& elapsed)
{
    for (auto& elem : m_childs)
    {
        elem->Update(elapsed);
    }
}

bool Widget::HandleInput(const sf::Event& event)
{
    for (auto& elem : m_childs)
    {
        if (elem->HandleInput(event))
            return true;
    }
    return false;
}

void Widget::SetWidgetStyle(WidgetStyle* style)
{
    m_style = *style;
}

void Widget::SetRedraw(bool flag)
{
    m_needRedraw = flag;
}

std::shared_ptr<Element> Widget::FindChild(const std::string& name)
{
    for (auto it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        if ((*it)->GetName() == name)
        {
            return *it;
        }
        else if (auto widget = std::dynamic_pointer_cast<Widget>(*it))
        {
            if (auto child = widget->FindChild(name))
            {
                return child;
            }
        }
    }
    return nullptr;
}

bool Widget::AppendChild(std::shared_ptr<Element> child)
{
    for (auto& elem : m_childs)
    {
        if (elem->GetName() == child->GetName())
        {
            DBG("Widget::AppendChild, Child name duplicant: {}", child->GetName());
            return false;
        }
    }
    m_childs.emplace_back(child);
    m_needRedraw = true;

    return true;
}

void Widget::RemoveChild(const std::string& name)
{
    for (auto it = m_childs.begin(); it != m_childs.end(); ++it)
    {
        if ((*it)->GetName() == name)
        {
            m_childs.erase(it);
            return;
        }
    }
}

void Widget::_RedrawPanel()
{
    m_panelTexture.clear(m_style.m_backColor);
    for (auto& elem : m_childs)
    {
        elem->Render(m_panelTexture);
    }
    m_panelTexture.display();
    m_needRedraw = false;
}

void Widget::_RenderPrimitive(sf::RenderTarget& target)
{
    if (m_needRedraw)
    {
        _RedrawPanel();
    }
    target.draw(m_panelSprite);
}

void Widget::_UpdatePosition()
{
    m_panelSprite.setPosition(m_position);
}

void Widget::_UpdateSize()
{
    auto ret = m_panelTexture.resize({(std::uint32_t)m_size.x, (std::uint32_t)m_size.y});
    if (ret)
    {
        m_panelSprite.setTexture(m_panelTexture.getTexture(), true);
    }
}

} // namespace gui
#include "GuiComponent.hpp"

#include "Element.hpp"
#include "GuiManager.hpp"
#include "core/SharedContext.hpp"
#include "resource/TextStringManager.hpp"

namespace gui
{

Component::Component(const ComponentParam& param)
    : m_owner(param.m_owner)
{
}

/////////////////////////////////////////////////////////////////////////
ElementBaseComponent::ElementBaseComponent(const ParamType& param)
    : Component(param),
      m_parent(param.m_parent),
      m_activate(true),
      m_state(ElementState::Normal)
{
}

void ElementBaseComponent::SetParent(std::shared_ptr<Element> parent)
{
    m_parent = parent;
    m_owner->SetNeedUpdateContext(true);
}

void ElementBaseComponent::SetActivate(bool flag)
{
    m_activate = flag;
    m_owner->SetNeedUpdateContext(true);
}

void ElementBaseComponent::SetElementState(ElementState state)
{
    m_state = state;
    m_owner->SetNeedUpdateContext(true);
}

/////////////////////////////////////////////////////////////////////////

TransformComponent::TransformComponent(const ParamType& param)
    : Component(param),
      m_position(param.m_position),
      m_size(param.m_size),
      m_zValue(param.m_zValue)
{
}

void TransformComponent::SetPosition(sf::Vector2f& position)
{
    m_position = position;
    m_owner->UpdateContext();
}

void TransformComponent::SetSize(sf::Vector2f& size)
{
    m_size = size;
    m_owner->UpdateContext();
}

void TransformComponent::SetZValue(std::int32_t value)
{
    m_zValue = value;
    m_owner->UpdateContext();
}

bool TransformComponent::IsIntersectWith(sf::Vector2f& point)
{
    sf::FloatRect rect = {{m_position.x - m_size.x / 2, m_position.y - m_size.y / 2}, m_size};
    if (rect.contains(point))
        return true;
    return false;
}

/////////////////////////////////////////////////////////////////////////
LabelComponent::LabelComponent(const ParamType& param)
    : Component(param),
      m_style(nullptr)
{
    SetLabelStyle(param.m_styleName);
}

void LabelComponent::UpdateContext()
{
    if (!m_sprite && m_style != nullptr)
    {
        m_sprite.emplace(*m_style->m_texture, m_style->m_rect);
    }
    else
    {
        m_sprite->setTexture(*m_style->m_texture);
        m_sprite->setTextureRect(m_style->m_rect);
    }
    m_sprite->setOrigin(m_sprite->getLocalBounds().getCenter());

    auto transCompo = m_owner->GetComponent<TransformComponent>();
    if (!transCompo)
        return;

    m_sprite->setScale({transCompo->GetSize().x / m_style->m_rect.size.x, transCompo->GetSize().y / m_style->m_rect.size.y});
    m_sprite->setPosition(transCompo->GetPosition());
}

void LabelComponent::Render(sf::RenderTarget* target)
{
    if (m_sprite)
        target->draw(m_sprite.value());
}

void LabelComponent::SetLabelStyle(const std::string& styleName)
{
    auto style = SharedContext::Instance()->Get<GuiManager>()->FindStyleSheetByName(styleName);
    if (!style)
        return;

    m_style = static_cast<gui::LabelStyle*>(style);
    m_owner->SetNeedUpdateContext(true);
}

/////////////////////////////////////////////////////////////////////////
TextComponent::TextComponent(const ParamType& param)
    : Component(param),
      m_style(nullptr),
      m_strID(param.m_strID),
      m_args(param.m_args),
      m_vAlignment(param.m_vAlignment),
      m_hAlignment(param.m_hAlignment)
{
    SetStyle(param.m_styleName);
}

void TextComponent::UpdateContext()
{
    if (!m_style)
        return;

    auto transCompo = m_owner->GetComponent<TransformComponent>();
    if (!transCompo)
        return;

    if (!m_text)
    {
        m_text.emplace(*m_style->m_font);
    }

    m_text->setFillColor(m_style->m_color);
    m_text->setCharacterSize(m_style->m_charSize);
    m_text->setStyle(m_style->m_style);
    m_text->setScale(m_style->m_scale);

    auto str = SharedContext::Instance()->Get<TextStringManager>()->FindTextString(m_strID);
    if (!m_args.empty())
    {
        str = SharedContext::Instance()->Get<TextStringManager>()->PackFormatString(str, m_args);
    }
    m_text->setString(str);

    auto bound = m_text->getLocalBounds();
    m_text->setOrigin(bound.getCenter());

    sf::Vector2f pos;
    switch (m_hAlignment)
    {
    case gui::HAlignment::Left:
        pos.x = transCompo->GetPosition().x - transCompo->GetSize().x / 2 + bound.size.x / 2;
        break;
    case gui::HAlignment::Center:
        pos.x = transCompo->GetPosition().x;
        break;
    case gui::HAlignment::Right:
        pos.x = transCompo->GetPosition().x + transCompo->GetSize().x - bound.size.x / 2;
        break;
    }

    switch (m_vAlignment)
    {
    case gui::VAlignment::Top:
        pos.y = transCompo->GetPosition().y - transCompo->GetSize().y / 2 + bound.size.y / 2;
        break;
    case gui::VAlignment::Center:
        pos.y = transCompo->GetPosition().y;
        break;
    case gui::VAlignment::Down:
        pos.y = transCompo->GetPosition().y + transCompo->GetSize().y - bound.size.y / 2;
        break;
    }

    m_text->setPosition(pos);
}

void TextComponent::Render(sf::RenderTarget* target)
{
    if (m_text)
        target->draw(m_text.value());
}

void TextComponent::SetStyle(const std::string& styleName)
{
    m_style = static_cast<gui::TextStyle*>(SharedContext::Instance()->Get<GuiManager>()->FindStyleSheetByName(styleName));
    m_owner->SetNeedUpdateContext(true);
}

void TextComponent::SetStringID(const std::string& strID)
{
    if (strID == m_strID)
        return;
    m_strID = strID;

    m_owner->SetNeedUpdateContext(true);
}

void TextComponent::SetArgs(const std::vector<std::string>& args)
{
    m_args = args;
    m_owner->SetNeedUpdateContext(true);
}

void TextComponent::SetVAlignment(VAlignment align)
{
    m_vAlignment = align;
    m_owner->SetNeedUpdateContext(true);
}

void TextComponent::SetHAlignment(HAlignment align)
{
    m_hAlignment = align;
    m_owner->SetNeedUpdateContext(true);
}

/////////////////////////////////////////////////////////////////////////
ButtonComponent::ButtonComponent(const ParamType& param)
    : Component(param),
      m_callback(param.m_callback)
{
    auto guiManager = SharedContext::Instance()->Get<GuiManager>();
    m_styles[ElementState::Normal] = static_cast<gui::ButtonStyle*>(guiManager->FindStyleSheetByName(param.m_normalStyleName));
    m_styles[ElementState::Hover] = static_cast<gui::ButtonStyle*>(guiManager->FindStyleSheetByName(param.m_hoverStyleName));
    m_styles[ElementState::Pressed] = static_cast<gui::ButtonStyle*>(guiManager->FindStyleSheetByName(param.m_pressStyleName));
}

void ButtonComponent::UpdateContext()
{
    auto baseCompo = m_owner->GetComponent<ElementBaseComponent>();
    auto transCompo = m_owner->GetComponent<TransformComponent>();
    auto curStyle = m_styles[baseCompo->GetElementState()];
    if (curStyle)
    {
        if (!m_sprite)
        {
            m_sprite.emplace(*curStyle->m_texture, curStyle->m_rect);
        }
        else
        {
            m_sprite->setTexture(*curStyle->m_texture);
            m_sprite->setTextureRect(curStyle->m_rect);
        }
        m_sprite->setOrigin(m_sprite->getLocalBounds().getCenter());
        m_sprite->setScale({transCompo->GetSize().x / curStyle->m_rect.size.x, transCompo->GetSize().y / curStyle->m_rect.size.y});
        m_sprite->setPosition(transCompo->GetPosition());
    }
}

void ButtonComponent::Render(sf::RenderTarget* target)
{
    if (m_sprite)
    {
        target->draw(m_sprite.value());
    }
}

void ButtonComponent::OnMouseRelease()
{
    if (m_callback)
    {
        m_callback();
    }
}

/////////////////////////////////////////////////////////////////////////
WidgetComponent::WidgetComponent(const ParamType& param)
    : Component(param),
      m_sprite(m_panelTexture.getTexture())
{
}

void WidgetComponent::UpdateFrame(const sf::Time& elapsed)
{
    for (auto& elem : m_vecChilds)
    {
        elem->UpdateFrame(elapsed);
    }
}

void WidgetComponent::UpdateContext()
{
    auto transCompo = m_owner->GetComponent<TransformComponent>();
    if (transCompo)
    {
        sf::Vector2u newSize(transCompo->GetSize().x, transCompo->GetSize().y);
        if (m_panelTexture.resize(newSize))
        {
            m_sprite.setTexture(m_panelTexture.getTexture());
        }
    }

    for (auto& elem : m_vecChilds)
    {
        elem->UpdateContext();
    }
}

void WidgetComponent::Render(sf::RenderTarget* target)
{
    if (m_owner->GetNeedReRender())
    {
        m_panelTexture.clear();

        for (auto& elem : m_vecChilds)
        {
            elem->Render(&m_panelTexture);
        }
        m_panelTexture.display();
    }

    sf::Sprite sprite(m_panelTexture.getTexture());
    target->draw(sprite);
}

bool WidgetComponent::AddChild(std::shared_ptr<Element> child)
{
    if (m_mapChilds.contains(child->GetName()))
    {
        return false;
    }

    auto ret = m_mapChilds.try_emplace(child->GetName(), child);
    if (!ret.second)
    {
        return false;
    }

    m_vecChilds.push_back(child);
    return true;
}

void WidgetComponent::DelChild(const std::string& name)
{
    if (!m_mapChilds.contains(name))
    {
        return;
    }

    m_mapChilds.erase(name);
    for (auto iter = m_vecChilds.begin(); iter != m_vecChilds.end(); ++iter)
    {
        if ((*iter)->GetName() == name)
        {
            m_vecChilds.erase(iter);
            break;
        }
    }
}

Element* WidgetComponent::GetChild(const std::string& name)
{
    if (m_mapChilds.contains(name))
    {
        return m_mapChilds[name].get();
    }
    return nullptr;
}

} // namespace gui
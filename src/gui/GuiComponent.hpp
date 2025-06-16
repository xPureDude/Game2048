#pragma once

#include "Element.hpp"
#include "ElementStyle.hpp"
#include "SFML/Graphics.hpp" // IWYU pragma: keep

namespace gui
{

enum class VAlignment
{
    Top,
    Center,
    Down
};

enum class HAlignment
{
    Left,
    Center,
    Right
};

class Element;

struct ComponentParam
{
    Element* m_owner;
};

class Component
{
public:
    Component(const ComponentParam& param);
    virtual ~Component() = default;

    virtual void UpdateFrame(const sf::Time& elapsed) {};
    virtual void UpdateContext() {};
    virtual void Render(sf::RenderTarget* target) {};

    virtual void OnMouseHover() {};
    virtual void OnMousePress() {};
    virtual void OnMouseRelease() {};
    virtual void OnMouseLeave() {};

protected:
    Element* m_owner;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ElementBaseComponentParam : public ComponentParam
{
    std::weak_ptr<Element> m_parent;
};

class ElementBaseComponent : public Component
{
public:
    using ParamType = ElementBaseComponentParam;

public:
    ElementBaseComponent(const ParamType& param);

    void SetParent(std::shared_ptr<Element> parent);
    void SetActivate(bool flag);
    void SetElementState(ElementState state);

    std::weak_ptr<Element> GetParent() { return m_parent; }
    bool GetActivate() { return m_activate; }
    ElementState GetElementState() { return m_state; }

private:
    std::weak_ptr<Element> m_parent;
    bool m_activate;
    ElementState m_state;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TransformComponentParam : public ComponentParam
{
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    std::int32_t m_zValue{0};
};

class TransformComponent : public Component
{
public:
    using ParamType = TransformComponentParam;

public:
    TransformComponent(const ParamType& param);

    void SetPosition(sf::Vector2f& position);
    void SetSize(sf::Vector2f& size);
    void SetZValue(std::int32_t value);

    sf::Vector2f GetPosition() { return m_position; }
    sf::Vector2f GetSize() { return m_size; }
    std::int32_t GetZValue() { return m_zValue; }

    bool IsIntersectWith(sf::Vector2f& point);

private:
    sf::Vector2f m_position;
    sf::Vector2f m_size;
    std::int32_t m_zValue;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct LabelComponentParam : public ComponentParam
{
    std::string m_styleName;
};

class LabelComponent : public Component
{
public:
    using ParamType = LabelComponentParam;

public:
    LabelComponent(const ParamType& param);

    virtual void UpdateContext() override;
    virtual void Render(sf::RenderTarget* target) override;

    void SetLabelStyle(const std::string& styleName);

private:
    gui::LabelStyle* m_style;
    std::optional<sf::Sprite> m_sprite;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TextComponentParam : public ComponentParam
{
    std::string m_styleName;
    std::string m_strID;
    std::vector<std::string> m_args;
    VAlignment m_vAlignment{VAlignment::Center};
    HAlignment m_hAlignment{HAlignment::Center};
};

class TextComponent : public Component
{
public:
    using ParamType = TextComponentParam;

public:
    TextComponent(const ParamType& param);

    virtual void UpdateContext() override;
    virtual void Render(sf::RenderTarget* target) override;

    void SetStyle(const std::string& styleName);
    void SetStringID(const std::string& strID);
    void SetArgs(const std::vector<std::string>& args);
    void SetVAlignment(VAlignment align);
    void SetHAlignment(HAlignment align);

private:
    TextStyle* m_style;
    std::optional<sf::Text> m_text;
    std::string m_strID;
    std::vector<std::string> m_args;
    VAlignment m_vAlignment{VAlignment::Center};
    HAlignment m_hAlignment{HAlignment::Center};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ButtonComponentParam : public ComponentParam
{
    std::string m_normalStyleName;
    std::string m_hoverStyleName;
    std::string m_pressStyleName;
    std::function<void(void)> m_callback;
};

class ButtonComponent : public Component
{
public:
    using ParamType = ButtonComponentParam;

public:
    ButtonComponent(const ParamType& param);

    virtual void UpdateContext() override;
    virtual void Render(sf::RenderTarget* target) override;

    virtual void OnMouseRelease() override;

private:
    std::optional<sf::Sprite> m_sprite;
    std::unordered_map<ElementState, gui::ButtonStyle*> m_styles;
    std::function<void(void)> m_callback;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct WidgetComponentParam : public ComponentParam
{
    std::string m_styleName;
};

class WidgetComponent : public Component
{
public:
    using ParamType = WidgetComponentParam;

public:
    WidgetComponent(const ParamType& param);

    virtual void UpdateFrame(const sf::Time& elapsed) override;
    virtual void UpdateContext() override;
    virtual void Render(sf::RenderTarget* target) override;

    bool AddChild(std::shared_ptr<Element> child);
    void DelChild(const std::string& name);
    Element* GetChild(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<Element>> m_mapChilds;
    std::vector<std::shared_ptr<Element>> m_vecChilds;
    WidgetStyle* m_style;

    sf::RenderTexture m_panelTexture;
    sf::Sprite m_sprite;
};

} // namespace gui
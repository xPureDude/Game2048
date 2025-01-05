#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "../input/InputVisitorDependent.hpp"
#include "../pch.hpp" // IWYU pragma: keep

namespace gui
{

enum class Signal
{
    // All
    OnLeave,
    OnHover,

    // Button
    OnPress,
    OnRelease,
    OnClicked,
};

struct SignalTriggerInfo
{
    Signal m_sig;
    std::any m_data;
};

enum class ElementState
{
    Default,
    Hover,
    Pressed,
    Disable,
};

struct TextInfo
{
    std::shared_ptr<sf::Font> m_font;
    std::string m_textStr;
    std::int32_t m_charSize{30};
    sf::Color m_color;
    std::uint32_t m_style;
};

class Widget;

class Element : public InputVisitorDependent
{
public:
    Element();
    virtual ~Element();

    virtual void Update(const sf::Time& elapsed);
    void Render(sf::RenderTarget* target);

    bool ConnectSignalCallback(Signal sig, const std::string_view& name, CallbackType callback);
    void DisConnectSignalCallback(Signal sig, const std::string_view& name);

    void SetName(const std::string& name) { m_name = name; }
    void SetParent(std::shared_ptr<Element> parent);
    void SetPosition(const sf::Vector2f& pos);
    void SetSize(const sf::Vector2f& size);
    void SetText(const std::string& textStr);
    void SetTextInfo(const TextInfo& info);

    sf::Vector2f GetGlobalPosition();
    sf::Vector2f GetLocalPosition();
    std::string GetName() { return m_name; }
    TextInfo GetTextInfo() { return m_textInfo; }

private:
    void _RedrawParent();
    void _ProcessCallback();
    void _UpdateText();
    virtual void _RenderPrimitive(sf::RenderTarget* target) = 0;
    virtual void _UpdatePosition() {}
    virtual void _UpdateSize() {}

protected:
    std::weak_ptr<Widget> m_parent;
    bool m_activate;
    ElementState m_state;

    std::string m_name;
    sf::Vector2f m_position; // leftTop corner
    sf::Vector2f m_size;

    TextInfo m_textInfo;
    std::shared_ptr<sf::Text> m_text;

    std::vector<SignalTriggerInfo> m_signalQueue;
    std::map<Signal, std::map<std::string_view, CallbackType>> m_callbacks;
};

} // namespace gui

#endif // ELEMENT_HPP
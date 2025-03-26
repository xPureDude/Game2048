#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <map>
#include <string>

#include "ElementStyle.hpp"
#include "common/globaldefine.hpp"
#include "input/InputVisitorDependent.hpp"

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

std::optional<Signal> TranslateStringToSignal(const std::string& str);

struct SignalTriggerInfo
{
    Signal m_sig;
    std::any m_data;
};

enum class ElementState
{
    Normal,
    Hover,
    Pressed,
    Disable,
};

class Widget;

class Element : public InputVisitorDependent
{
public:
    Element();
    virtual ~Element();

    virtual void Update(const sf::Time& elapsed);
    virtual bool HandleInput(const sf::Event& event);
    void Render(sf::RenderTarget* target);

    virtual bool operator()(const sf::Event::MouseMoved& event) override;

    bool ConnectSignalCallback(Signal sig, const std::string_view& name, CallbackType callback);
    void DisConnectSignalCallback(Signal sig, const std::string_view& name);

    void SetName(const std::string& name) { m_name = name; }
    void SetParent(std::shared_ptr<Element> parent);
    void SetPosition(const sf::Vector2f& pos);
    void SetSize(const sf::Vector2f& size);
    void SetText(const sf::String& textStr, TextStyle* style = nullptr);
    void SetTextStyle(TextStyle* style);

    sf::Vector2f GetGlobalPosition();
    sf::Vector2f GetLocalPosition();
    std::string GetName() { return m_name; }
    sf::String GetText() { return m_textStr; }
    TextStyle* GetTextStyle() { return &m_textStyle; }

protected:
    void _UpdateText();

private:
    void _RedrawParent();
    void _ProcessCallback();
    virtual void _RenderPrimitive(sf::RenderTarget* target) = 0;
    virtual void _UpdateCurrentState() {}
    virtual void _UpdatePosition() {}
    virtual void _UpdateSize() {}

protected:
    std::weak_ptr<Widget> m_parent;
    bool m_activate;
    ElementState m_state;

    std::string m_name;
    sf::Vector2f m_position; // leftTop corner
    sf::Vector2f m_size;

    sf::String m_textStr;
    TextStyle m_textStyle;
    std::shared_ptr<sf::Text> m_text;

    std::vector<SignalTriggerInfo> m_signalQueue;
    std::map<Signal, std::map<std::string_view, CallbackType>> m_callbacks;
};

} // namespace gui

#endif // ELEMENT_HPP
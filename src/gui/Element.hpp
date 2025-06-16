#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <map>
#include <optional>
#include <string>

#include "SFML/Graphics.hpp" // IWYU pragma: keep
#include "common/Utility.hpp"

namespace gui
{

class Component;

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
    Pressed
};

class Widget;

class Element
{
public:
    Element(const std::string& name);
    virtual ~Element();

    void UpdateFrame(const sf::Time& elapsed);
    void Render(sf::RenderTarget* target);

    void UpdateContext();

    bool ConnectSignalCallback(Signal sig, const std::string_view& name, CallbackType callback);
    void DisConnectSignalCallback(Signal sig, const std::string_view& name);

    void SetName(const std::string& name) { m_name = name; }
    void SetNeedUpdateContext(bool flag) { m_needUpdateContext = flag; }
    void SetNeedReRender(bool flag) { m_needReRender = flag; }

    std::string GetName() { return m_name; }
    bool GetNeedUpdateContext() { return m_needUpdateContext; }
    bool GetNeedReRender() { return m_needReRender; }

    sf::Vector2f GetGlobalPosition();
    sf::Vector2f GetLocalPosition();

    void OnMouseHover();
    void OnMousePress();
    void OnMouseRelease();
    void OnMouseLeave();

    template <typename T>
    Element* AddComponent(typename T::ParamType& param);

    template <typename T>
    void DelComponent();

    template <typename T>
    T* GetComponent();

private:
    void _RedrawParent();

protected:
    std::string m_name;
    bool m_needUpdateContext;
    bool m_needReRender;

    std::vector<std::shared_ptr<Component>> m_components;
    std::vector<std::shared_ptr<Element>> m_childs;

    std::vector<SignalTriggerInfo> m_signalQueue;
    std::map<Signal, std::map<std::string_view, CallbackType>> m_callbacks;
};

template <typename T>
Element* Element::AddComponent(typename T::ParamType& param)
{
    for (auto& ptr : m_components)
    {
        if (std::dynamic_pointer_cast<T>(ptr))
        {
            return this;
        }
    }

    param.m_owner = this;
    auto comp = std::make_shared<T>(param);
    m_components.push_back(comp);

    SetNeedUpdateContext(true);
    SetNeedReRender(true);
    return this;
}

template <typename T>
void Element::DelComponent()
{
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
    {
        if (auto ret = std::dynamic_pointer_cast<T>(*iter))
        {
            iter = m_components.erase(iter);
            SetNeedUpdateContext(true);
            SetNeedReRender(true);
            break;
        }
    }
}

template <typename T>
T* Element::GetComponent()
{
    for (auto& ptr : m_components)
    {
        if (auto ret = std::dynamic_pointer_cast<T>(ptr))
        {
            return ret.get();
        }
    }
    return nullptr;
}

} // namespace gui

#endif // ELEMENT_HPP
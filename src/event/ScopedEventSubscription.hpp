#pragma once

#include <cstdint>

#include "EventDefine.hpp"
#include "common/Utility.hpp"

class ScopedEventSubscription
{
public:
    ScopedEventSubscription() = default;
    ~ScopedEventSubscription()
    {
        UnSubscribeAll();
    }

    template <typename EventT>
    auto Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::GlobalBase, EventT>, void>;

    template <typename EventT>
    auto Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::UIBase, EventT>, void>;

    template <typename EventT>
    auto Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::SceneBase, EventT>, void>;

    template <typename EventT>
    auto Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::InputBase, EventT>, void>;

    void UnSubscribeAll()
    {
    }

private:
    std::vector<std::uint32_t> m_tokens;
};

template <typename EventT>
auto ScopedEventSubscription::Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::GlobalBase, EventT>, void>
{
}

template <typename EventT>
auto ScopedEventSubscription::Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::UIBase, EventT>, void>
{
}

template <typename EventT>
auto ScopedEventSubscription::Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::SceneBase, EventT>, void>
{
}

template <typename EventT>
auto ScopedEventSubscription::Subscribe(CallbackType callback) -> std::enable_if_t<std::is_base_of_v<class evt::InputBase, EventT>, void>
{
}

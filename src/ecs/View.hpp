#pragma once

#include <tuple>

#include "Registry.hpp"

template <typename... Components>
class View
{
public:
    explicit View(Registry& registry)
        : m_registry(registry)
    {
        _DoQueryEntities();
    }

    class Iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::tuple<Entity, Components*...>;
        using difference_type = ptrdiff_t;

        explicit Iterator(Registry& registry, std::size_t index)
            : m_registry(registry),
              m_index(index)
        {
            // 初始化指针（示例代码需根据实际组件类型展开）
            m_entity = _GetEntity(index);
            m_components = _GetComponentPtr(m_entity);
        }

    private:
        Entity _GetEntity(std::size_t index)
        {
            return m_entity;
        }

        std::tuple<Components*...> _GetComponentPtr(Entity entity)
        {
        }

    private:
        Registry& m_registry;
        std::size_t m_index;

        Entity m_entity;
        std::tuple<Components*...> m_components;
    };

    std::size_t Size()
    {
        auto storage = m_registry.template GetStorage<std::tuple_element_t<0, std::tuple<Components...>>>();
        return storage.GetSize();
    }

private:
    void _DoQueryEntities();

private:
    Registry& m_registry;
};

template <typename... Components>
void View<Components...>::_DoQueryEntities()
{
}
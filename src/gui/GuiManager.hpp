#ifndef GUIMANAGER_HPP
#define GUIMANAGER_HPP

#include "../pch.hpp" // IWYU pragma: keep
#include "../scene/SceneDependent.hpp"
#include "Element.hpp"

class GuiManager : public SceneDependent
{
public:
    GuiManager(SharedContext* ctx);
    ~GuiManager();

    void Update(const sf::Time& elapsed);
    void Render();

    void ClearSceneGui(SceneType type);

private:
    SharedContext* m_ctx;
    sf::RenderTarget* m_target;
    std::map<SceneType, std::vector<gui::ElementPtr>> m_elements;
};

#endif // GUIMANAGER_HPP
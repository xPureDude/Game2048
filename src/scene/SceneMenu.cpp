#include "SceneMenu.hpp"

#include "pch.hpp" // IWYU pragma: keep

#include "SceneManager.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"
#include "gui/GuiManager.hpp"
#include "resource/TextureManager.hpp"

SceneMenu::SceneMenu()
{
}

SceneMenu::~SceneMenu()
{
}

bool SceneMenu::OnCreate()
{
    if (!_InitGui())
        return false;

    return true;
}

void SceneMenu::OnDestroy()
{
    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();
    guiManager->ClearSceneGui(SceneType::MainMenu);
}

void SceneMenu::Update(const sf::Time& elasped)
{
}

void SceneMenu::Render(Window* window)
{
    window->Render(m_background);
}

void SceneMenu::OnEnter(const std::any& param)
{
}

void SceneMenu::OnLeave()
{
}

bool SceneMenu::_InitGui()
{
    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();

    SceneGuiInfo info;
    info.m_type = SceneType::MainMenu;
    info.m_fileName = "assets/SceneMenu.xml";
    info.m_callbacks.try_emplace("OnBeginGame", BindCallback(&SceneMenu::_OnBeginGame));
    info.m_callbacks.try_emplace("OnQuitGame", BindCallback(&SceneMenu::_OnQuitGame));
    if (!guiManager->LoadSceneGuiFromFile(info))
    {
        DBG("SceneMenu::_InitGui, failed to load SceneGui: {}", info.m_fileName);
        return false;
    }

    TextureManager* textureManager = SharedContext::Instance()->Get<TextureManager>();
    m_backgroundTexture = textureManager->RequestResource("scene_menu_bg");
    if (!m_backgroundTexture)
    {
        DBG("SceneMenu::_InitGui, backTexture is nullptr, name: {}", "scene_menu_bg");
        return false;
    }
    m_background.setTexture(m_backgroundTexture.get(), true);
    m_background.setSize(static_cast<sf::Vector2f>(m_backgroundTexture.get()->getSize()));

    return true;
}

void SceneMenu::_OnBeginGame(const std::any& param)
{
    // Note: Temp default 4x4
    std::any sceneParam = std::make_any<std::size_t>(4);
    SharedContext::Instance()->Get<SceneManager>()->ChangeScene(SceneType::Play, sceneParam);
}

void SceneMenu::_OnQuitGame(const std::any& param)
{
    SharedContext::Instance()->Get<Window>()->Close();
}
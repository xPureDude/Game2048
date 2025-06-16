#include "SceneMainMenu.hpp"

#include "SceneMainMenuLayer.hpp"
#include "SceneManager.hpp"
#include "common/Log.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"
#include "event/GuiEvent.hpp"
#include "gui/GuiManager.hpp"
#include "resource/TextureManager.hpp"

SceneMainMenu::SceneMainMenu()
{
}

SceneMainMenu::~SceneMainMenu()
{
}

bool SceneMainMenu::OnCreate()
{
    if (!_InitGui())
        return false;

    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();
    guiManager->CreateGuiLayer<MainMenuLayer>(GuiLayerType::MainMenu);

    SubscribeEvent<evt::BeginGame>(BindCallback(&SceneMainMenu::_OnBeginGame));
    SubscribeEvent<evt::OpenSetting>(BindCallback(&SceneMainMenu::_OnOpenSetting));
    SubscribeEvent<evt::QuitGame>(BindCallback(&SceneMainMenu::_OnQuitGame));

    return true;
}

void SceneMainMenu::OnDestroy()
{
    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();
    guiManager->ReleaseGuiLayer(GuiLayerType::MainMenu);
}

void SceneMainMenu::Update(const sf::Time& elasped)
{
}

void SceneMainMenu::Render(Window* window)
{
    window->Render(m_background);
}

void SceneMainMenu::OnEnter(const std::any& param)
{
    Scene::OnEnter(param);
    SharedContext::Instance()->Get<GuiManager>()->SetCurLayerType(GuiLayerType::MainMenu);
}

void SceneMainMenu::OnLeave()
{
    Scene::OnLeave();
}

bool SceneMainMenu::_InitGui()
{
    TextureManager* textureManager = SharedContext::Instance()->Get<TextureManager>();
    m_backgroundTexture = textureManager->RequestResource("scene_menu_bg");
    if (!m_backgroundTexture)
    {
        DBG("SceneMainMenu::_InitGui, backTexture is nullptr, name: {}", "scene_menu_bg");
        return false;
    }
    m_background.setTexture(m_backgroundTexture.get(), true);
    m_background.setSize(static_cast<sf::Vector2f>(m_backgroundTexture->getSize()));

    return true;
}

void SceneMainMenu::_OnBeginGame(const std::any& param)
{
    // Note: Temp default 4x4
    ScenePlayParam scenePlayParam;
    scenePlayParam.m_isContinue = false;
    SharedContext::Instance()->Get<SceneManager>()->ChangeScene(SceneType::Play, std::make_any<ScenePlayParam>(scenePlayParam));
}

void SceneMainMenu::_OnOpenSetting(const std::any& param)
{
    SharedContext::Instance()->Get<SceneManager>()->ChangeScene(SceneType::Setting, param);
}

void SceneMainMenu::_OnQuitGame(const std::any& param)
{
    SharedContext::Instance()->Get<Window>()->Close();
}
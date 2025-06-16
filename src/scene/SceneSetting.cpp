#include "SceneSetting.hpp"

#include "SceneManager.hpp"
#include "SceneSettingLayer.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"
#include "event/GuiEvent.hpp"
#include "gui/GuiManager.hpp"
#include "resource/TextureManager.hpp"

SceneSetting::SceneSetting()
    : Scene()
{
    m_renderTransparent = true;
}

SceneSetting::~SceneSetting()
{
}

bool SceneSetting::OnCreate()
{
    SharedContext::Instance()->Get<GuiManager>()->CreateGuiLayer<SceneSettingLayer>(GuiLayerType::Setting);

    SubscribeEvent<evt::BackToMainMenu>(BindCallback(&SceneSetting::_OnBackToMenuEvent));

    TextureManager* textureManager = SharedContext::Instance()->Get<TextureManager>();
    m_backgroundTexture = textureManager->RequestResource("scene_menu_bg");
    m_background.setTexture(m_backgroundTexture.get(), true);
    m_background.setSize(static_cast<sf::Vector2f>(m_backgroundTexture->getSize()));

    return true;
}

void SceneSetting::OnDestroy()
{
    SharedContext::Instance()->Get<GuiManager>()->ReleaseGuiLayer(GuiLayerType::Setting);
}

void SceneSetting::Update(const sf::Time& elasped)
{
}

void SceneSetting::Render(Window* window)
{
    window->Render(m_background);
}

void SceneSetting::OnEnter(const std::any& param)
{
    Scene::OnEnter(param);
    SharedContext::Instance()->Get<GuiManager>()->SetCurLayerType(GuiLayerType::Setting);
}

void SceneSetting::OnLeave()
{
    Scene::OnLeave();
}

void SceneSetting::_OnBackToMenuEvent(evt::Base* event)
{
    SharedContext::Instance()->Get<SceneManager>()->ChangeScene(SceneType::MainMenu, std::any());
}
#include "SceneGameOver.hpp"

#include "SceneGameOverLayer.hpp"
#include "SceneManager.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"
#include "event/GuiEvent.hpp"
#include "gui/GuiManager.hpp"

SceneGameOver::SceneGameOver()
    : m_isWin(false),
      m_state(State::Entering)
{
    m_renderTransparent = true;
}

SceneGameOver::~SceneGameOver()
{
}

bool SceneGameOver::OnCreate()
{
    Window* window = SharedContext::Instance()->Get<Window>();
    m_background.setSize(sf::Vector2f(window->GetSize()));
    m_background.setFillColor(sf::Color(0xFFFFFF00));

    SharedContext::Instance()->Get<GuiManager>()->CreateGuiLayer<SceneGameOverLayer>(GuiLayerType::GameOver);

    SubscribeEvent<evt::ContinueGame>(BindCallback(&SceneGameOver::_OnContinueGameEvent));
    SubscribeEvent<evt::RetryGame>(BindCallback(&SceneGameOver::_OnRetryGameEvent));
    SubscribeEvent<evt::BackToMainMenu>(BindCallback(&SceneGameOver::_OnBackToMenuEvent));

    return true;
}

void SceneGameOver::OnDestroy()
{
    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();
    guiManager->ReleaseGuiLayer(GuiLayerType::GameOver);
}

void SceneGameOver::Update(const sf::Time& elasped)
{
    if (m_state == State::Entering)
    {
        m_fadeInElapsed += elasped;
        m_background.setFillColor(sf::Color(0xFFFFFF00 + 0x77 * m_fadeInElapsed.asMilliseconds() / 1500));
        if (m_fadeInElapsed > sf::seconds(1.5))
        {
            m_fadeInElapsed = sf::Time::Zero;
            m_background.setFillColor(sf::Color(0xFFFFFF77));
            m_state = State::Normal;

            SharedContext::Instance()->Get<GuiManager>()->SetCurLayerType(GuiLayerType::GameOver);
        }
    }
}

void SceneGameOver::Render(Window* window)
{
    window->Render(m_background);
}

void SceneGameOver::OnEnter(const std::any& param)
{
    m_state = State::Entering;
    m_fadeInElapsed = sf::Time::Zero;
    m_background.setFillColor(sf::Color(0xFFFFFF00));

    SharedContext::Instance()->Get<GuiManager>()->SetCurLayerType(GuiLayerType::None);
}

void SceneGameOver::OnLeave()
{
}

void SceneGameOver::_OnContinueGameEvent(evt::Base* event)
{
    auto sceneManager = SharedContext::Instance()->Get<SceneManager>();

    ScenePlayParam param;
    param.m_isContinue = true;
    sceneManager->ChangeScene(SceneType::Play, std::make_any<ScenePlayParam>(param));
}

void SceneGameOver::_OnRetryGameEvent(evt::Base* event)
{
    auto sceneManager = SharedContext::Instance()->Get<SceneManager>();
    sceneManager->ChangeScene(SceneType::Play, std::make_any<ScenePlayParam>());
}

void SceneGameOver::_OnBackToMenuEvent(evt::Base* event)
{
    auto sceneManager = SharedContext::Instance()->Get<SceneManager>();
    sceneManager->ChangeScene(SceneType::MainMenu, std::any());
}
#include "SceneGameOver.hpp"

#include "SceneManager.hpp"
#include "common/Log.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"
#include "gui/GuiManager.hpp"
#include "resource/FontManager.hpp"
#include "resource/TextStringManager.hpp"

SceneGameOver::SceneGameOver()
    : m_isWin(false)
{
    m_renderTransparent = true;
}

SceneGameOver::~SceneGameOver()
{
}

bool SceneGameOver::OnCreate()
{
    if (!_InitGui())
        return false;

    m_font = SharedContext::Instance()->Get<FontManager>()->RequestResource("block_font");
    m_text = std::make_shared<sf::Text>(*m_font);
    m_text->setCharacterSize(40);
    m_text->setOutlineColor(sf::Color::White);
    m_text->setOutlineThickness(2);

    return true;
}

void SceneGameOver::OnDestroy()
{
    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();
    guiManager->ClearSceneGui(SceneType::GameOver);
}

void SceneGameOver::Update(const sf::Time& elasped)
{
    if (m_state == State::Entering)
    {
        m_fadeInElapsed += elasped;
        m_background.setFillColor(sf::Color(0xFFFFFF00 + 0x77 * m_fadeInElapsed.asMilliseconds() / 2000));
        if (m_fadeInElapsed > sf::seconds(2.0))
        {
            m_fadeInElapsed = sf::Time::Zero;
            m_background.setFillColor(sf::Color(0xFFFFFF77));
            m_state = State::Normal;
        }
    }
}

void SceneGameOver::Render(Window* window)
{
    window->Render(m_background);
    if (m_state == State::Normal)
        window->Render(*m_text);
}

void SceneGameOver::OnEnter(const std::any& param)
{
    m_state = State::Entering;
    try
    {
        m_isWin = std::any_cast<bool>(param);
        auto textManager = SharedContext::Instance()->Get<TextStringManager>();
        auto button = SharedContext::Instance()->Get<GuiManager>()->FindSceneElementByName(SceneType::GameOver, "FirstButton");
        if (m_isWin)
        {
            button->SetText(textManager->FindTextString(TextString::CONTINUE));
            m_text->setFillColor(sf::Color::Red);
            m_text->setString(textManager->FindTextString(TextString::YOU_WIN));
        }
        else
        {
            button->SetText(textManager->FindTextString(TextString::REPLAY));
            m_text->setFillColor(sf::Color::Black);
            m_text->setString(textManager->FindTextString(TextString::YOU_LOSE));
        }

        auto bounds = m_text->getLocalBounds();
        m_text->setPosition({250 - bounds.size.x / 2, 250 - bounds.size.y});
    }
    catch (std::bad_any_cast& err)
    {
        DBG("SceneGameOver::OnEnter, param is not bool");
    }
}

void SceneGameOver::OnLeave()
{
}

bool SceneGameOver::_InitGui()
{
    Window* window = SharedContext::Instance()->Get<Window>();
    m_background.setSize(sf::Vector2f(window->GetSize()));
    m_background.setFillColor(sf::Color(0xFFFFFF00));

    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();
    auto factory = guiManager->GetElementFactory();

    sf::Vector2f buttonSize{200, 60};

    auto textStyle = static_cast<gui::TextStyle*>(guiManager->FindStyleSheetByName("text_normal"));
    if (!textStyle)
    {
        DBG("SceneGameOver::_InitGui, failed to find StyleSheet: text_normal");
        return false;
    }

    std::string name = "button_normal";
    auto normalStyle = static_cast<gui::ButtonStyle*>(guiManager->FindStyleSheetByName(name));
    if (!normalStyle)
    {
        DBG("SceneGameOver::_InitGui, failed to find StyleSheet: {}", name);
        return false;
    }
    name = "button_hover";
    auto hoverStyle = static_cast<gui::ButtonStyle*>(guiManager->FindStyleSheetByName(name));
    if (!hoverStyle)
    {
        DBG("SceneGameOver::_InitGui, failed to find StyleSheet: {}", name);
        return false;
    }
    name = "button_press";
    auto pressStyle = static_cast<gui::ButtonStyle*>(guiManager->FindStyleSheetByName(name));
    if (!pressStyle)
    {
        DBG("SceneGameOver::_InitGui, failed to find StyleSheet: {}", name);
        return false;
    }

    auto textManager = SharedContext::Instance()->Get<TextStringManager>();

    auto firstButton = factory.CreateElement<gui::Button>();
    firstButton->SetName("FirstButton");
    firstButton->SetPosition({150, 280});
    firstButton->SetSize(buttonSize);
    firstButton->SetTextStyle(textStyle);
    firstButton->SetButtonStyle(normalStyle, hoverStyle, pressStyle, nullptr);
    firstButton->ConnectSignalCallback(gui::Signal::OnClicked, "OnContinueOrReplay", BindCallback(&SceneGameOver::_OnFirstButtonClicked));
    guiManager->AddSceneGui(SceneType::GameOver, firstButton);

    auto backButton = factory.CreateElement<gui::Button>();
    backButton->SetName("BackButton");
    backButton->SetPosition({150, 360});
    backButton->SetSize(buttonSize);
    backButton->SetText(textManager->FindTextString(TextString::BACK_TO_MENU), textStyle);
    backButton->SetButtonStyle(normalStyle, hoverStyle, pressStyle, nullptr);
    backButton->ConnectSignalCallback(gui::Signal::OnClicked, "OnBackToMenu", BindCallback(&SceneGameOver::_OnBackButtonClicked));
    guiManager->AddSceneGui(SceneType::GameOver, backButton);

    return true;
}

void SceneGameOver::_OnFirstButtonClicked(const std::any& param)
{
    auto sceneManager = SharedContext::Instance()->Get<SceneManager>();
    if (m_isWin)
    {
        sceneManager->PopScene();
    }
    else
    {
        sceneManager->ChangeScene(SceneType::Play, std::any());
    }
}

void SceneGameOver::_OnBackButtonClicked(const std::any& param)
{
    auto sceneManager = SharedContext::Instance()->Get<SceneManager>();
    sceneManager->ChangeScene(SceneType::MainMenu, std::any());
}
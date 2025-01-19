#include "SceneMenu.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../gui/Button.hpp"
#include "../gui/GuiManager.hpp"
#include "../pch.hpp"
#include "../resource/TextureManager.hpp"
#include "SceneManager.hpp"

SceneMenu::SceneMenu(SceneManager* manager)
    : Scene(manager)
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
    TextureManager* textureManager = m_sceneManager->GetSharedContext()->Get<TextureManager>();
    m_backgroundTexture = textureManager->RequestResource("scene_menu_bg");
    if (!m_backgroundTexture)
    {
        DBG("SceneMenu::_InitGui, backTexture is nullptr, name: {}", "scene_menu_bg");
        return false;
    }
    m_background.setTexture(m_backgroundTexture.get(), true);
    m_background.setSize(static_cast<sf::Vector2f>(m_backgroundTexture.get()->getSize()));

    GuiManager* guiManager = m_sceneManager->GetSharedContext()->Get<GuiManager>();
    gui::ElementFactory& factory = guiManager->GetElementFactory();

    auto beginButton = std::dynamic_pointer_cast<gui::Button>(factory.CreateElement(gui::ElementType::Button));
    auto quitButton = std::dynamic_pointer_cast<gui::Button>(factory.CreateElement(gui::ElementType::Button));
    if (auto style = static_cast<gui::ButtonStyle*>(guiManager->GetStyleSheetByName("button_default")); style)
    {
        beginButton->SetButtonStyle(gui::ElementState::Default, style);
        quitButton->SetButtonStyle(gui::ElementState::Default, style);
    }
    if (auto style = static_cast<gui::ButtonStyle*>(guiManager->GetStyleSheetByName("button_hover")); style)
    {
        beginButton->SetButtonStyle(gui::ElementState::Hover, style);
        quitButton->SetButtonStyle(gui::ElementState::Hover, style);
    }
    if (auto style = static_cast<gui::ButtonStyle*>(guiManager->GetStyleSheetByName("button_press")); style)
    {
        beginButton->SetButtonStyle(gui::ElementState::Pressed, style);
        quitButton->SetButtonStyle(gui::ElementState::Pressed, style);
    }

    if (auto style = static_cast<gui::TextStyle*>(guiManager->GetStyleSheetByName("text_default")); style)
    {
        style->m_textStr = "Begin Game";
        beginButton->SetTextStyle(style);
        style->m_textStr = "Quit Game";
        quitButton->SetTextStyle(style);
    }
    beginButton->SetPosition({150, 400});
    beginButton->SetSize({200, 60});
    beginButton->ConnectSignalCallback(gui::Signal::OnClicked, "OnBeginGame", BindCallback(&SceneMenu::_OnBeginGame));

    quitButton->SetPosition({150, 500});
    quitButton->SetSize({200, 60});
    beginButton->ConnectSignalCallback(gui::Signal::OnClicked, "OnQuitGame", BindCallback(&SceneMenu::_OnQuitGame));

    guiManager->AddSceneGui(SceneType::MainMenu, beginButton);
    guiManager->AddSceneGui(SceneType::MainMenu, quitButton);

    return true;
}

void SceneMenu::_OnBeginGame(const std::any& param)
{
    // Note: Temp default 4x4
    std::any sceneParam = std::make_any<std::size_t>(4);
    m_sceneManager->PushScene(SceneType::Play, sceneParam);
}

void SceneMenu::_OnQuitGame(const std::any& param)
{
}
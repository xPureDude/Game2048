#include "SceneMainMenuLayer.hpp"

#include "core/SharedContext.hpp"
#include "event/EventManager.hpp"
#include "event/GuiEvent.hpp"
#include "gui/Element.hpp"
#include "gui/GuiComponent.hpp"

MainMenuLayer::MainMenuLayer()
    : gui::Layer()
{
    _InitLayer();
}

void MainMenuLayer::_InitLayer()
{
    gui::ElementBaseComponentParam baseParam;

    gui::TransformComponentParam transParam;
    transParam.m_position = {250, 330};
    transParam.m_size = {200, 60};

    gui::ButtonComponentParam buttonParam;
    buttonParam.m_normalStyleName = "button_normal";
    buttonParam.m_hoverStyleName = "button_hover";
    buttonParam.m_pressStyleName = "button_press";
    buttonParam.m_callback = []() { SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::BeginGame>(); };

    gui::TextComponentParam textParam;
    textParam.m_styleName = "text_normal";
    textParam.m_strID = "begin_game";

    auto beginButton = AddElement("ButtonBegin");
    beginButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);

    transParam.m_position = {250, 410};
    buttonParam.m_callback = []() { SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::OpenSetting>(); };
    textParam.m_strID = "game_options";
    auto settingButton = AddElement("ButtonSetting");
    settingButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);

    transParam.m_position = {250, 490};
    buttonParam.m_callback = []() { SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::QuitGame>(); };
    textParam.m_strID = "quit_game";

    auto quitButton = AddElement("ButtonQuit");
    quitButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);
}
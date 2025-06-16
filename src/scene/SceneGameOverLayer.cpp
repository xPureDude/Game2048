#include "SceneGameOverLayer.hpp"

#include "core/SharedContext.hpp"
#include "event/EventManager.hpp"
#include "event/GuiEvent.hpp"
#include "event/SceneEvent.hpp"
#include "gui/GuiComponent.hpp"

SceneGameOverLayer::SceneGameOverLayer()
    : gui::Layer(),
      m_isWin(false)
{
    m_eventListener.Subscribe<evt::GameOver>(BindCallback(&SceneGameOverLayer::_OnGameOver));

    _InitLayer();
}

void SceneGameOverLayer::_InitLayer()
{
    gui::ElementBaseComponentParam baseParam;
    gui::TransformComponentParam transParam;
    gui::TextComponentParam textParam;

    transParam.m_position = {250, 140};
    textParam.m_styleName = "text_tip";
    m_tipText = AddElement("TipText");
    m_tipText->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::TextComponent>(textParam);

    gui::ButtonComponentParam buttonParam;
    buttonParam.m_normalStyleName = "button_normal";
    buttonParam.m_hoverStyleName = "button_hover";
    buttonParam.m_pressStyleName = "button_press";

    transParam.m_position = {250, 280};
    transParam.m_size = {200, 60};
    buttonParam.m_callback = [this]() {
        if (this->m_isWin)
        {
            SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::ContinueGame>();
        }
        else
        {
            SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::RetryGame>();
        }
    };
    textParam.m_styleName = "text_normal";

    m_firstButton = AddElement("FirstButton");
    m_firstButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);

    transParam.m_position = {250, 360};
    buttonParam.m_callback = []() { SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::BackToMainMenu>(); };
    textParam.m_strID = "back_to_menu";
    auto backButton = AddElement("BackButton");
    backButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);
}

void SceneGameOverLayer::_OnGameOver(evt::Base* event)
{
    auto gameOver = static_cast<evt::GameOver*>(event);
    m_isWin = gameOver->m_isWin;

    auto tipTextComp = m_tipText->GetComponent<gui::TextComponent>();
    auto buttonTextComp = m_firstButton->GetComponent<gui::TextComponent>();

    if (m_isWin)
    {
        tipTextComp->SetStringID("you_win");
        buttonTextComp->SetStringID("continue_game");
    }
    else
    {
        tipTextComp->SetStringID("you_lose");
        buttonTextComp->SetStringID("retry_game");
    }
}
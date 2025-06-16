#include "ScenePlayLayer.hpp"

#include "core/SharedContext.hpp"
#include "event/EventManager.hpp"
#include "event/GuiEvent.hpp"
#include "event/SceneEvent.hpp"
#include "gui/GuiComponent.hpp"
#include "gui/GuiManager.hpp"

ScenePlayLayer::ScenePlayLayer()
    : gui::Layer()
{
    _InitLayer();

    m_eventListener.Subscribe<evt::ScoreChange>(BindCallback(&ScenePlayLayer::_OnScoreChange));
}

void ScenePlayLayer::_InitLayer()
{
    gui::ElementBaseComponentParam baseParam;

    gui::TransformComponentParam transParam;
    transParam.m_position = {390, 160};
    transParam.m_size = {200, 60};

    gui::ButtonComponentParam buttonParam;
    buttonParam.m_normalStyleName = "button_normal";
    buttonParam.m_hoverStyleName = "button_hover";
    buttonParam.m_pressStyleName = "button_press";
    buttonParam.m_callback = []() { SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::RetryGame>(); };

    gui::TextComponentParam textParam;
    textParam.m_styleName = "text_normal";
    textParam.m_strID = "retry_game";

    auto retryButton = AddElement("ButtonRetry");
    retryButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);

    transParam.m_position = {110, 160};
    textParam.m_strID = "cur_score";
    textParam.m_args.push_back("0");

    gui::LabelComponentParam labelParam;
    labelParam.m_styleName = "label_normal";

    m_scoreLabel = AddElement("LabelScore");
    m_scoreLabel->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transParam)
        ->AddComponent<gui::LabelComponent>(labelParam)
        ->AddComponent<gui::TextComponent>(textParam);
}

void ScenePlayLayer::_OnScoreChange(evt::Base* event)
{
    auto textComp = m_scoreLabel->GetComponent<gui::TextComponent>();
    if (!textComp)
    {
        return;
    }
    auto scoreEvent = static_cast<evt::ScoreChange*>(event);
    std::vector<std::string> a;
    a.push_back(std::to_string(scoreEvent->m_curScore));
    textComp->SetArgs(a);
}
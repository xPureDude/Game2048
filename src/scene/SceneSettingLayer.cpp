#include "SceneSettingLayer.hpp"

#include "core/ConfigManager.hpp"
#include "core/SharedContext.hpp"
#include "event/EventManager.hpp"
#include "event/GlobalEvent.hpp"
#include "event/GuiEvent.hpp"
#include "gui/GuiComponent.hpp"
#include "gui/GuiManager.hpp"

SceneSettingLayer::SceneSettingLayer()
    : gui::Layer()
{
    gui::ElementBaseComponentParam baseParam;

    gui::TransformComponentParam transformParam;
    transformParam.m_size = {200, 60};

    gui::ButtonComponentParam buttonParam;
    buttonParam.m_normalStyleName = "button_normal";
    buttonParam.m_hoverStyleName = "button_hover";
    buttonParam.m_pressStyleName = "button_press";

    gui::TextComponentParam textParam;
    textParam.m_styleName = "text_normal";

    transformParam.m_position = {250, 300};
    buttonParam.m_callback = [this]() { _OnLanguageChanged("English"); };
    textParam.m_strID = "lang_english";
    auto englishButton = AddElement("button_english");
    englishButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transformParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);

    transformParam.m_position = {250, 380};
    buttonParam.m_callback = [this]() { _OnLanguageChanged("Chinese"); };
    textParam.m_strID = "lang_chinese";
    auto chineseButton = AddElement("button_chinese");
    chineseButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transformParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);

    transformParam.m_position = {390, 660};
    buttonParam.m_callback = [this]() { _OnBackToMainMenu(); };
    textParam.m_strID = "back_to_menu";
    auto backButton = AddElement("button_back");
    backButton->AddComponent<gui::ElementBaseComponent>(baseParam)
        ->AddComponent<gui::TransformComponent>(transformParam)
        ->AddComponent<gui::ButtonComponent>(buttonParam)
        ->AddComponent<gui::TextComponent>(textParam);
}

SceneSettingLayer::~SceneSettingLayer()
{
}

void SceneSettingLayer::_OnLanguageChanged(const std::string& lang)
{
    SharedContext::Instance()->Get<ConfigManager>()->GetVisualConfig().m_language = lang;
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::LanguageChanged>();
}

void SceneSettingLayer::_OnBackToMainMenu()
{
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::BackToMainMenu>();
}
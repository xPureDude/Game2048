#include "GameApp.hpp"

#include "common/Log.hpp"
#include "core/ConfigManager.hpp"
#include "core/SharedContext.hpp"
#include "gui/GuiManager.hpp"
#include "resource/FontManager.hpp"
#include "resource/TextStringManager.hpp"
#include "resource/TextureManager.hpp"
#include "scene/SceneManager.hpp"

GameApp::GameApp()
    : PureFramework()
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
    PureFramework::Init();

    if (!_LoadResources())
        return false;

    SceneManager* sceneManager = SharedContext::Instance()->Get<SceneManager>();
    std::any param = std::make_any<std::int32_t>(0);
    // std::any isWin = std::make_any<bool>(true);
    sceneManager->PushScene(SceneType::MainMenu, std::any());

    return true;
}

bool GameApp::_LoadResources()
{
    if (!SharedContext::Instance()->Get<TextureManager>()->LoadResourceInfoFromFile("Resource/TextureInfo.xml"))
    {
        ERR("GameApp::_LoadResources, TextureManager load failed, file: {}", "Resource/TextureInfo.xml");
        return false;
    }

    if (!SharedContext::Instance()->Get<FontManager>()->LoadResourceInfoFromFile("Resource/FontInfo.xml"))
    {
        ERR("GameApp::_LoadResources, FontManager load failed, file: {}", "Resource/FontInfo.xml");
        return false;
    }

    std::string& local = SharedContext::Instance()->Get<ConfigManager>()->GetVisualConfig().m_language;
    if (!SharedContext::Instance()->Get<TextStringManager>()->LoadTextStringFromFile("Resource/TextString.xml", local))
    {
        ERR("GameApp::_LoadResources, TextStringManager load failed, file: {}", "Resource/TextString.xml");
        return false;
    }

    if (!SharedContext::Instance()->Get<GuiManager>()->LoadStyleSheetsFromFile("Resource/GuiStyleSheet.xml"))
    {
        ERR("GameApp::_LoadResources, GuiManager load failed, file: {}", "Resource/GuiStyleSheet.xml");
        return false;
    }

    return true;
}
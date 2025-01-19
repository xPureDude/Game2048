#include "GameApp.hpp"

#include "../common/Log.hpp"
#include "../core/SharedContext.hpp"
#include "../gui/GuiManager.hpp"
#include "../resource/FontManager.hpp"
#include "../resource/TextureManager.hpp"
#include "../scene/SceneManager.hpp"

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

    SceneManager* sceneManager = m_ctx.Get<SceneManager>();
    std::any param = std::make_any<std::int32_t>(0);
    sceneManager->PushScene(SceneType::MainMenu, param);

    return true;
}

bool GameApp::_LoadResources()
{
    if (!m_ctx.Get<TextureManager>()->LoadResourceInfoFromFile("Resource/TextureInfo.xml"))
    {
        ERR("GameApp::_LoadResources, TextureManager load failed, file: {}", "Resource/TextureInfo.xml");
        return false;
    }

    if (!m_ctx.Get<FontManager>()->LoadResourceInfoFromFile("Resource/FontInfo.xml"))
    {
        ERR("GameApp::_LoadResources, FontManager load failed, file: {}", "Resource/FontInfo.xml");
        return false;
    }

    if (!m_ctx.Get<GuiManager>()->LoadStyleSheetsFromFile("Resource/GuiStyleSheet.xml"))
    {
        ERR("GameApp::_LoadResources, GuiManager load failed, file: {}", "Resource/GuiStyleSheet.xml");
        return false;
    }

    return true;
}
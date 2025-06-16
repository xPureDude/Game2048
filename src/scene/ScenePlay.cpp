#include "ScenePlay.hpp"

#include "SceneManager.hpp"
#include "ScenePlayLayer.hpp"
#include "common/Log.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"
#include "event/GuiEvent.hpp"
#include "event/InputEvent.hpp"
#include "event/InputManager.hpp"
#include "event/SceneEvent.hpp"
#include "gui/GuiManager.hpp"
#include "resource/TextureManager.hpp"

ScenePlay::ScenePlay()
    : m_game2048(nullptr)
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate()
{
    SubscribeEvent<evt::MoveLeft>(BindCallback(&ScenePlay::_OnMoveLeft));
    SubscribeEvent<evt::MoveRight>(BindCallback(&ScenePlay::_OnMoveRight));
    SubscribeEvent<evt::MoveUp>(BindCallback(&ScenePlay::_OnMoveUp));
    SubscribeEvent<evt::MoveDown>(BindCallback(&ScenePlay::_OnMoveDown));

    SubscribeEvent<evt::RetryGame>(BindCallback(&ScenePlay::_OnNewGame));
    SubscribeEvent<evt::GameOver>(BindCallback(&ScenePlay::_OnGameOver));

    if (!_InitGui())
    {
        DBG("ScenePlay::OnCreate, InitGui failed");
        return false;
    }

    TextureManager* textureManager = SharedContext::Instance()->Get<TextureManager>();
    auto blockTexture = textureManager->RequestResource("blocks");
    if (!blockTexture)
    {
        DBG("ScenePlay::OnCreate, blockTexture is nullptr, name: {}", "blocks");
        return false;
    }

    m_game2048 = new Game2048(blockTexture);

    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();
    guiManager->CreateGuiLayer<ScenePlayLayer>(GuiLayerType::Play);

    return true;
}

void ScenePlay::OnDestroy()
{
    delete m_game2048;
}

void ScenePlay::Update(const sf::Time& elapsed)
{
    // Check Window Focus
    m_game2048->Update(elapsed);
}

void ScenePlay::Render(Window* window)
{
    window->Render(m_background);
    m_game2048->Render(window->GetRenderWindow());
}

void ScenePlay::OnEnter(const std::any& param)
{
    SharedContext::Instance()->Get<GuiManager>()->SetCurLayerType(GuiLayerType::Play);
    SharedContext::Instance()->Get<InputManager>()->SetInputState(InputState::PlayState);

    ScenePlayParam scenePlayParam;
    try
    {
        scenePlayParam = std::any_cast<ScenePlayParam>(param);
    }
    catch (const std::bad_any_cast& err)
    {
        DBG("ScenePlay::OnEnter, wrong param type {}", err.what());
        return;
    }

    if (scenePlayParam.m_isContinue)
    {
        return;
    }

    Window* window = SharedContext::Instance()->Get<Window>();
    auto windowSize = window->GetSize();

    std::size_t rowCount = 4;
    std::uint32_t boardWidth = 500;

    float blockSpace = boardWidth / (rowCount * 6.f + 1.f);
    float blockSize = blockSpace * 5.f;
    sf::Vector2f offset = {0, float(windowSize.y - boardWidth)};
    m_game2048->SetPosition(offset);

    m_info.m_boardSize = {boardWidth, boardWidth};
    m_info.m_blockSize = blockSize;
    m_info.m_blockSpace = blockSpace;
    m_info.m_rowCount = rowCount;
    m_info.m_colCount = rowCount;
    m_info.m_position = offset;
    m_game2048->OnNewGame(m_info);
}

void ScenePlay::OnLeave() {}

bool ScenePlay::_InitGui()
{
    TextureManager* textureManager = SharedContext::Instance()->Get<TextureManager>();
    m_backgroundTexture = textureManager->RequestResource("scene_play_bg");
    if (!m_backgroundTexture)
    {
        DBG("ScenePlay::_InitGui, backgroundTexture is nullptr, name: {}", "scene_play_bg");
        return false;
    }
    m_background.setTexture(m_backgroundTexture.get(), true);
    m_background.setSize({500, 700});

    return true;
}

void ScenePlay::_OnMoveLeft(evt::Base* event)
{
    m_game2048->OnMoveLeft();
}

void ScenePlay::_OnMoveRight(evt::Base* event)
{
    m_game2048->OnMoveRight();
}

void ScenePlay::_OnMoveUp(evt::Base* event)
{
    m_game2048->OnMoveUp();
}

void ScenePlay::_OnMoveDown(evt::Base* event)
{
    m_game2048->OnMoveDown();
}

void ScenePlay::_OnNewGame(evt::Base* event)
{
    m_game2048->OnNewGame(m_info);
}

void ScenePlay::_OnGameOver(evt::Base* event)
{
    auto evtGameOver = static_cast<evt::GameOver*>(event);
    SharedContext::Instance()->Get<SceneManager>()->PushScene(SceneType::GameOver, std::make_any<bool>(evtGameOver->m_isWin));
}
#include "ScenePlay.hpp"

#include "../common/Log.hpp"
#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../gui/Element.hpp"
#include "../gui/GuiManager.hpp"
#include "../input/InputManager.hpp"
#include "../resource/FontManager.hpp"
#include "../resource/TextureManager.hpp"
#include "SceneManager.hpp"

ScenePlay::ScenePlay(SceneManager* manager)
    : Scene(manager),
      m_game2048(nullptr)
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate()
{
    SharedContext* ctx = m_sceneManager->GetSharedContext();

    InputManager* inputManager = ctx->Get<InputManager>();
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft", BindCallback(&ScenePlay::_OnMoveLeft));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight", BindCallback(&ScenePlay::_OnMoveRight));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp", BindCallback(&ScenePlay::_OnMoveUp));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown", BindCallback(&ScenePlay::_OnMoveDown));

    TextureManager* textureManager = ctx->Get<TextureManager>();
    auto blockTexture = textureManager->RequestResource("blocks");
    if (!blockTexture)
    {
        DBG("ScenePlay::OnCreate, blockTexture is nullptr, name: {}", "blocks");
        return false;
    }

    m_game2048 = new Game2048(blockTexture);
    m_game2048->ConnectGameSignalCallback(GameSignal::ScoreChange, BindCallback(&ScenePlay::_OnScoreChange));
    m_game2048->ConnectGameSignalCallback(GameSignal::GameLose, BindCallback(&ScenePlay::_OnGameLose));
    m_game2048->ConnectGameSignalCallback(GameSignal::GameWin, BindCallback(&ScenePlay::_OnGameWin));

    if (!_InitGui())
    {
        DBG("ScenePlay::OnCreate, InitGui failed");
        return false;
    }

    return true;
}

void ScenePlay::OnDestroy()
{
    delete m_game2048;

    InputManager* inputManager = m_sceneManager->GetSharedContext()->Get<InputManager>();
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft");
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight");
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp");
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown");
}

void ScenePlay::Update(const sf::Time& elapsed)
{
    // Check Window Focus
    m_game2048->Update(elapsed);
}

void ScenePlay::Render(Window* window)
{
    window->Render(m_background);
    m_game2048->Render(window);
}

void ScenePlay::OnEnter(const std::any& param)
{
    Window* window = m_sceneManager->GetSharedContext()->Get<Window>();
    auto windowSize = window->GetSize();

    std::size_t rowCount = 0;
    try
    {
        rowCount = std::any_cast<std::size_t>(param);
    }
    catch (const std::bad_any_cast& err)
    {
        DBG("ScenePlay::OnEnter, wrong param type {}", err.what());
    }

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
    SharedContext* ctx = m_sceneManager->GetSharedContext();

    std::map<std::string_view, CallbackType> callbacks;
    callbacks.emplace("OnNewGame", BindCallback(&ScenePlay::_OnNewGameClicked));

    GuiManager* guiManager = ctx->Get<GuiManager>();

    TextureManager* textureManager = ctx->Get<TextureManager>();
    m_backgroundTexture = textureManager->RequestResource("scene_play_bg");
    if (!m_backgroundTexture)
    {
        return false;
    }
    m_background.setTexture(m_backgroundTexture.get(), true);
    m_background.setSize({500, 700});

    FontManager* fontManager = ctx->Get<FontManager>();
    auto blockFont = fontManager->RequestResource("block_font");
    if (blockFont == nullptr)
    {
        return false;
    }

    return true;
}

void ScenePlay::_OnMoveLeft(const std::any& param)
{
    m_game2048->OnMoveLeft();
}

void ScenePlay::_OnMoveRight(const std::any& param)
{
    m_game2048->OnMoveRight();
}

void ScenePlay::_OnMoveUp(const std::any& param)
{
    m_game2048->OnMoveUp();
}

void ScenePlay::_OnMoveDown(const std::any& param)
{
    m_game2048->OnMoveDown();
}

void ScenePlay::_OnScoreChange(const std::any& param)
{
    try
    {
        auto score = std::any_cast<std::size_t>(param);
        auto element = m_sceneManager->GetSharedContext()->Get<GuiManager>()->GetSceneElementByName(SceneType::Play, "score_label");
        if (element)
        {
            auto style = element->GetTextStyle();
            style->m_textStr = "Score: " + std::to_string(score);
            element->SetTextStyle(style);
        }
    }
    catch (const std::bad_any_cast& err)
    {
        DBG("ScenePlay::_OnScoreChange, wrong param type {}", err.what());
    }
}

void ScenePlay::_OnNewGameClicked(const std::any& param)
{
    std::any score = std::make_any<std::size_t>(0);
    _OnScoreChange(score);

    m_game2048->OnNewGame(m_info);
}

void ScenePlay::_OnGameLose(const std::any& param)
{
}

void ScenePlay::_OnGameWin(const std::any& param)
{
}
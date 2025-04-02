#include "ScenePlay.hpp"

#include "SceneManager.hpp"
#include "common/Log.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"
#include "event/InputManager.hpp"
#include "gui/GuiManager.hpp"
#include "resource/TextureManager.hpp"


ScenePlay::ScenePlay()
    : m_game2048(nullptr)
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate()
{
    InputManager* inputManager = SharedContext::Instance()->Get<InputManager>();
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft", BindCallback(&ScenePlay::_OnMoveLeft));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight", BindCallback(&ScenePlay::_OnMoveRight));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp", BindCallback(&ScenePlay::_OnMoveUp));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown", BindCallback(&ScenePlay::_OnMoveDown));

    TextureManager* textureManager = SharedContext::Instance()->Get<TextureManager>();
    auto blockTexture = textureManager->RequestResource("blocks");
    if (!blockTexture)
    {
        DBG("ScenePlay::OnCreate, blockTexture is nullptr, name: {}", "blocks");
        return false;
    }

    m_game2048 = new Game2048(blockTexture);
    m_game2048->ConnectGameSignalCallback(GameSignal::ScoreChange, BindCallback(&ScenePlay::_OnScoreChange));
    m_game2048->ConnectGameSignalCallback(GameSignal::GameOver, BindCallback(&ScenePlay::_OnGameOver));

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

    InputManager* inputManager = SharedContext::Instance()->Get<InputManager>();
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
    m_scoreLabel->Render(window->GetRenderWindow());
    m_game2048->Render(window->GetRenderWindow());
}

void ScenePlay::OnEnter(const std::any& param)
{
    Window* window = SharedContext::Instance()->Get<Window>();
    auto windowSize = window->GetSize();

    std::size_t rowCount = 4;
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

    _OnScoreChange(std::make_any<std::size_t>(0));
}

void ScenePlay::OnLeave() {}

bool ScenePlay::_InitGui()
{
    GuiManager* guiManager = SharedContext::Instance()->Get<GuiManager>();

    SceneGuiInfo info;
    info.m_type = SceneType::Play;
    info.m_fileName = "assets/ScenePlay.xml";
    info.m_callbacks.try_emplace("OnNewGame", BindCallback(&ScenePlay::_OnNewGame));
    if (!guiManager->LoadSceneGuiFromFile(info))
    {
        DBG("ScenePlay::_InitGui, failed to load SceneGui: {}", info.m_fileName);
        return false;
    };

    TextureManager* textureManager = SharedContext::Instance()->Get<TextureManager>();
    m_backgroundTexture = textureManager->RequestResource("scene_play_bg");
    if (!m_backgroundTexture)
    {
        DBG("ScenePlay::_InitGui, backgroundTexture is nullptr, name: {}", "scene_play_bg");
        return false;
    }
    m_background.setTexture(m_backgroundTexture.get(), true);
    m_background.setSize({500, 700});

    // <Label name = "score_label" x = "10" y = "130" w = "200" h = "60" style = "label_normal" text = "text_normal" textStr = "Score: 0" />
    m_scoreLabel = gui::ElementFactory::CreateElement<gui::Label>();
    m_scoreLabel->SetName("score_label");
    m_scoreLabel->SetPosition({10, 130});
    m_scoreLabel->SetSize({200, 60});
    m_scoreLabel->SetLabelStyle(static_cast<gui::LabelStyle*>(SharedContext::Instance()->Get<GuiManager>()->FindStyleSheetByName("label_normal")));
    m_scoreLabel->SetText("Score: 0", static_cast<gui::TextStyle*>(SharedContext::Instance()->Get<GuiManager>()->FindStyleSheetByName("text_normal")));

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

void ScenePlay::_OnNewGame(const std::any& param)
{
    std::any score = std::make_any<std::size_t>(0);
    _OnScoreChange(score);

    m_game2048->OnNewGame(m_info);
}

void ScenePlay::_OnScoreChange(const std::any& param)
{
    try
    {
        auto score = std::any_cast<std::size_t>(param);
        m_scoreLabel->SetText("Score: " + std::to_string(score));
    }
    catch (const std::bad_any_cast& err)
    {
        DBG("ScenePlay::_OnScoreChange, wrong param type {}", err.what());
    }
}

void ScenePlay::_OnGameOver(const std::any& param)
{
    SharedContext::Instance()->Get<SceneManager>()->PushScene(SceneType::GameOver, param);
}
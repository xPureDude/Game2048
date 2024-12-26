#include "ScenePlay.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../event/InputManager.hpp"
#include "../gameplay/Game2048.hpp"
#include "../resource/FontManager.hpp"
#include "SceneManager.hpp"

ScenePlay::ScenePlay()
    : Scene(),
      m_game2048(nullptr),
      m_boardTexture({500, 500}),
      m_boardSprite(m_boardTexture.getTexture())
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate(SceneManager* manager)
{
    if (Scene::OnCreate(manager) == false)
        return false;

    SharedContext* ctx = m_sceneManager->GetSharedContext();

    InputManager* inputManager = ctx->Get<InputManager>();
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft", &ScenePlay::_OnMoveLeft, this);
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight", &ScenePlay::_OnMoveRight, this);
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp", &ScenePlay::_OnMoveUp, this);
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown", &ScenePlay::_OnMoveDown, this);

    FontManager* fontManager = ctx->Get<FontManager>();
    if (!fontManager->LoadFontFromFile("block", "SourceHanSansCN-Regular.otf"))
    {
        return false;
    }
    auto blockFont = fontManager->GetResource("block");
    if (blockFont == nullptr)
    {
        return false;
    }

    m_game2048 = new Game2048(blockFont);

    return true;
}

void ScenePlay::OnDestroy()
{
    InputManager* inputManager = m_sceneManager->GetSharedContext()->Get<InputManager>();
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft");
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight");
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp");
    inputManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown");

    FontManager* fontManager = m_sceneManager->GetSharedContext()->Get<FontManager>();
    fontManager->ReleaseResource("block");
}

void ScenePlay::Update(const sf::Time& elapsed)
{
    // Check Window Focus
    m_game2048->Update(elapsed);
}

void ScenePlay::Render(Window* window)
{
    window->Render(m_boardSprite);
    m_game2048->Render(window);
}

void ScenePlay::OnEnter()
{
    Window* window = m_sceneManager->GetSharedContext()->Get<Window>();
    auto windowSize = window->GetSize();

    // Note: Temp default 4x4
    std::size_t rowCount, colCount;
    rowCount = colCount = 4;
    std::uint32_t boardWidth = 500;

    float blockSpace = boardWidth / (colCount * 6.f + 1.f);
    float blockSize = blockSpace * 5.f;
    m_game2048->OnNewGame(rowCount, colCount, blockSize, blockSpace);
    sf::Vector2f offset = {0, float(windowSize.y - boardWidth)};
    m_game2048->SetPosition(offset);

    bool ret = m_boardTexture.resize({boardWidth, boardWidth});
    m_boardTexture.clear(sf::Color(0xBBADA0FF));

    sf::RectangleShape rect;
    rect.setSize({blockSize, blockSize});
    rect.setFillColor(sf::Color(0xEEE4DA5F));
    rect.setOrigin(rect.getLocalBounds().getCenter());

    for (std::size_t row = 0; row < rowCount; ++row)
    {
        for (std::size_t col = 0; col < colCount; ++col)
        {
            auto xpos = (col + 1) * blockSpace + col * blockSize + blockSize / 2.f;
            auto ypos = (row + 1) * blockSpace + row * blockSize + blockSize / 2.f;
            rect.setPosition({xpos, ypos});
            m_boardTexture.draw(rect);
        }
    }
    m_boardSprite.setTexture(m_boardTexture.getTexture());
    m_boardSprite.setPosition(offset);
}

void ScenePlay::OnLeave() {}

void ScenePlay::_OnMoveLeft()
{
    m_game2048->OnMoveLeft();
}

void ScenePlay::_OnMoveRight()
{
    m_game2048->OnMoveRight();
}

void ScenePlay::_OnMoveUp()
{
    m_game2048->OnMoveUp();
}

void ScenePlay::_OnMoveDown()
{
    m_game2048->OnMoveDown();
}
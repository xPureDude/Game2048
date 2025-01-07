#include "ScenePlay.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../gui/Button.hpp"
#include "../gui/GuiManager.hpp"
#include "../gui/Label.hpp"
#include "../gui/Widget.hpp"
#include "../input/InputManager.hpp"
#include "../resource/FontManager.hpp"
#include "../resource/TextureManager.hpp"
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
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft", BindCallback(&ScenePlay::_OnMoveLeft));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight", BindCallback(&ScenePlay::_OnMoveRight));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp", BindCallback(&ScenePlay::_OnMoveUp));
    inputManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown", BindCallback(&ScenePlay::_OnMoveDown));

    FontManager* fontManager = ctx->Get<FontManager>();
    if (!fontManager->LoadResourceInfoFromFile("block", "SourceHanSansCN-Regular.otf"))
    {
        return false;
    }
    auto blockFont = fontManager->RequestResource("block");
    if (blockFont == nullptr)
    {
        return false;
    }

    TextureManager* textureManager = ctx->Get<TextureManager>();
    if (!textureManager->LoadResourceInfoFromFile("element_base", "button_sheet.png"))
    {
        return false;
    }

    m_game2048 = new Game2048(blockFont);
    m_game2048->ConnectGameSignalCallback(GameSignal::ScoreChange, BindCallback(&ScenePlay::_OnScoreChange));

    _InitGui();

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
    window->Render(m_boardSprite);
    m_game2048->Render(window->GetRenderWindow());
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
    sf::Vector2f offset = {0, float(windowSize.y - boardWidth)};
    m_game2048->SetPosition(offset);

    m_info.m_boardSize = {boardWidth, boardWidth};
    m_info.m_blockSize = blockSize;
    m_info.m_blockSpace = blockSpace;
    m_info.m_rowCount = rowCount;
    m_info.m_colCount = colCount;
    m_info.m_position = offset;
    m_game2048->OnNewGame(m_info);

    bool ret = m_boardTexture.resize({boardWidth, boardWidth});
    m_boardTexture.clear(sf::Color(0xBBADA0FF));

    sf::RectangleShape rect;
    rect.setSize({blockSize, blockSize});
    rect.setFillColor(sf::Color(0xEEE4DA5F));
    rect.setOrigin(rect.getLocalBounds().getCenter());

    ret = m_boardTexture.setActive(true);
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

void ScenePlay::_InitGui()
{
    auto guiManager = m_sceneManager->GetSharedContext()->Get<GuiManager>();
    auto fontManager = m_sceneManager->GetSharedContext()->Get<FontManager>();
    auto& elementFactory = guiManager->GetElementFactory();

    auto topWidget = std::dynamic_pointer_cast<gui::Widget>(elementFactory.CreateElement(gui::ElementType::Widget, guiManager));
    guiManager->AddSceneGui(SceneType::Play, topWidget);

    topWidget->SetName("head_widget");
    topWidget->SetSize({500.f, 200.f});

    gui::WidgetInfo widgetInfo;
    widgetInfo.m_backColor = sf::Color(0xEEE4DAFF);

    topWidget->SetWidgetInfo(widgetInfo);

    auto scoreLabel = std::dynamic_pointer_cast<gui::Label>(elementFactory.CreateElement(gui::ElementType::Label));
    scoreLabel->SetName("score_label");
    topWidget->AppendChild(scoreLabel);
    scoreLabel->SetParent(topWidget);
    scoreLabel->SetPosition({10.f, 120.f});

    gui::TextInfo textInfo;
    textInfo.m_charSize = 25;
    textInfo.m_textStr = "Score: 0";
    textInfo.m_color = sf::Color::White;
    textInfo.m_style = sf::Text::Style::Regular;
    textInfo.m_font = fontManager->RequestResource("block");

    scoreLabel->SetTextInfo(textInfo);

    auto textureManager = m_sceneManager->GetSharedContext()->Get<TextureManager>();
    auto texture = textureManager->RequestResource("element_base");

    sf::Vector2i gridSize = {200, 60};

    gui::LabelInfo labelInfo;
    labelInfo.m_texture = texture;
    labelInfo.m_rect = sf::IntRect({0, 0}, gridSize);

    scoreLabel->SetLabelInfo(labelInfo);

    auto newButton = std::dynamic_pointer_cast<gui::Button>(elementFactory.CreateElement(gui::ElementType::Button));
    newButton->SetName("new_button");
    topWidget->AppendChild(newButton);
    newButton->SetParent(topWidget);
    newButton->SetPosition({290.f, 120.f});

    textInfo.m_textStr = "New Game";
    newButton->SetTextInfo(textInfo);

    gui::ButtonInfo buttonInfo;
    buttonInfo.m_texture = texture;

    buttonInfo.m_rect = sf::IntRect({0, 0}, gridSize);
    newButton->SetButtonInfo(gui::ElementState::Default, buttonInfo);
    buttonInfo.m_rect = sf::IntRect({200, 0}, gridSize);
    newButton->SetButtonInfo(gui::ElementState::Hover, buttonInfo);
    buttonInfo.m_rect = sf::IntRect({400, 0}, gridSize);
    newButton->SetButtonInfo(gui::ElementState::Pressed, buttonInfo);

    newButton->ConnectSignalCallback(gui::Signal::OnClicked, "OnNewButtonClicked", BindCallback(&ScenePlay::_OnNewGameClicked));
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
    if (auto score = std::any_cast<std::size_t>(param))
    {
        auto element = m_sceneManager->GetSharedContext()->Get<GuiManager>()->GetSceneElementByName(SceneType::Play, "score_label");
        if (element)
        {
            auto info = element->GetTextInfo();
            info.m_textStr = "Score: " + std::to_string(score);
            element->SetTextInfo(info);
        }
    }
}

void ScenePlay::_OnNewGameClicked(const std::any& param)
{
    std::any score = std::size_t(0);
    _OnScoreChange(score);

    m_game2048->OnNewGame(m_info);
}
#pragma once

#include "../pch.hpp" // IWYU pragma: keep

using Vector2size = sf::Vector2<std::size_t>;

struct BlockInfo
{
    BlockInfo(const sf::Color& backColor, std::shared_ptr<sf::Font> font, const sf::Color& valueColor, std::uint32_t charSize, std::int32_t value);
    void Render(sf::RenderTarget& window);
    void Render(Window* window);
    void SetScale(const sf::Vector2f& scale);
    void SetPosition(const sf::Vector2f& pos);
    sf::RectangleShape m_back;
    std::shared_ptr<sf::Font> m_font;
    sf::Text m_value;
};

class Game2048;

enum class BlockState
{
    Idle,
    Borning,
    Moving,
    Growing
};

struct Block
{
    void Update(const sf::Time& elapsed);

    Game2048* m_game;
    bool m_needGrow{false};
    BlockState m_state;
    std::int64_t m_index{-1};
    sf::Vector2f m_pos;
    Vector2size m_originGrid;
    Vector2size m_destGrid;
    sf::Vector2f m_scale;
    sf::Time m_actionElapsed;

private:
    void _UpdateMove(const sf::Time& elapsed);
    void _UpdateGrow(const sf::Time& elapsed);
};

struct NewGameInfo
{
    sf::Vector2u m_boardSize;
    sf::Vector2f m_position;
    std::size_t m_rowCount;
    std::size_t m_colCount;
    float m_blockSize;
    float m_blockSpace;
};

enum class GameSignal
{
    ScoreChange,
    GameWin,
    GameLose,
};

class Game2048
{
public:
    Game2048(std::shared_ptr<sf::Font> font);
    ~Game2048();

    void Update(const sf::Time& elapsed);
    void Render(sf::RenderTarget* target);

    void OnNewGame(const NewGameInfo& info);

    void SetPosition(const sf::Vector2f& pos);

    sf::Vector2f GetGridPosition(const sf::Vector2<std::size_t>& grid);

    void ConnectGameSignalCallback(GameSignal signal, CallbackType callback);

    void OnMoveLeft();
    void OnMoveRight();
    void OnMoveUp();
    void OnMoveDown();

private:
    void _ResetBoard();
    void _CreateNewBlock();
    void _CheckMoveGrid(Block* block, const Vector2size& oGrid, const Vector2size& dGrid, const Vector2size& pGrid);

private:
    static sf::Time s_moveTime;

    sf::RenderTexture m_boardTexture;
    sf::Sprite m_boardSprite;

    // Game2048Setting
    sf::Vector2f m_position;
    float m_blockSize;
    float m_blockSpace;
    std::size_t m_rowCount;
    std::size_t m_colCount;
    std::vector<BlockInfo> m_blockInfos;

    // Game2048 Data
    bool m_canMove;
    bool m_isPlaying;
    std::size_t m_score;
    std::uint32_t m_createBlockIndex;

    std::vector<Block*> m_blocks;
    std::vector<Block*> m_blockCache;
    std::vector<std::vector<Block*>> m_board;

    std::map<GameSignal, std::vector<CallbackType>> m_callbacks;
};
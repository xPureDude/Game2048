#pragma once

#include "../pch.pch" // IWYU pragma: keep

class Window;

using Vector2size = sf::Vector2<std::size_t>;

struct BlockInfo
{
    BlockInfo(const sf::Color& backColor, const sf::Font& font, const sf::Color& valueColor, std::uint32_t charSize, std::int32_t value);
    void Render(Window* window);
    void SetPosition(const sf::Vector2f& pos);
    sf::RectangleShape m_back;
    sf::Text m_value;
};

class Game2048;
struct Block
{
    void Update(const sf::Time& elapsed);

    Game2048* m_game;
    bool m_needGrow{false};
    std::int64_t m_index{-1};
    sf::Vector2f m_pos;
    Vector2size m_originGrid;
    Vector2size m_destGrid;
    sf::Time m_moveElapsed;
};

class Game2048
{
public:
    Game2048();
    ~Game2048();

    void Update(const sf::Time& elapsed);
    void Render(Window* window);

    void OnNewGame(std::size_t rowCount, std::size_t colCount);

    void SetPosition(const sf::Vector2f& pos);

    sf::Vector2f GetGridPosition(const sf::Vector2<std::size_t>& grid);

    void OnMoveLeft();
    void OnMoveRight();
    void OnMoveUp();
    void OnMoveDown();

private:
    void _ResetBoard();
    void _CreateNewBlock();
    void _CheckMoveGrid(Block* block, const Vector2size& oGrid, const Vector2size& dGrid, const Vector2size& pGrid);

private:
    static std::uint32_t s_boardWidth;
    static sf::Color s_boardColor;
    static sf::Color s_baseBlockColor;
    static sf::Time s_moveTime;

    // Game2048Setting
    sf::Vector2f m_position;
    float m_blockSize;
    float m_blockSpace;
    std::size_t m_rowCount;
    std::size_t m_colCount;
    sf::Font m_font;
    sf::RectangleShape m_baseBoard;
    sf::RectangleShape m_baseBlock;
    std::vector<BlockInfo> m_blockInfos;

    // Game2048 Data
    bool m_isMoving;
    bool m_isPlaying;
    std::size_t m_score;
    std::uint32_t m_createBlockIndex;

    std::list<Block*> m_blocks;
    std::vector<std::vector<Block*>> m_board;
};
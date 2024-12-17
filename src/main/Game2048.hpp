#pragma once

#include "SFML/Graphics.hpp" // IWYU pragma: keep
#include "SFML/System.hpp"   // IWYU pragma: keep
#include "SFML/Window.hpp"   // IWYU pragma: keep

#include <list>
#include <vector>

using Vector2size = sf::Vector2<std::size_t>;

struct BlockInfo
{
    BlockInfo(const sf::Color& backColor, const sf::Font& font, const sf::Color& valueColor, std::uint32_t charSize, std::int32_t value);
    void Render(sf::RenderTarget* target);
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
    void Render(sf::RenderTarget* window);

    void HandleEvent(const sf::Event& e);
    void OnNewGame(std::size_t rowCount, std::size_t colCount);

    sf::Vector2f GetGridPosition(const sf::Vector2<std::size_t>& grid);

private:
    void _OnMoveLeft();
    void _OnMoveRight();
    void _OnMoveUp();
    void _OnMoveDown();

    void _ResetBoard();
    void _CreateNewBlock();
    void _CheckMoveGrid(Block* block, const Vector2size& oGrid, const Vector2size& dGrid, const Vector2size& pGrid);

private:
    static std::uint32_t s_boardWidth;
    static std::uint32_t s_boardOffset;
    static sf::Color s_boardColor;
    static sf::Color s_baseBlockColor;
    static sf::Time s_moveTime;

    // Game2048Setting
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
#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"

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

class Game;
struct Block
{
    void Update(const sf::Time& elapsed);

    Game* m_game;
    bool m_needGrow{false};
    std::int64_t m_index{-1};
    sf::Vector2f m_pos;
    Vector2size m_originGrid;
    Vector2size m_destGrid;
    sf::Time m_moveElapsed;
};

class Game
{
public:
    Game();
    ~Game();

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

    bool m_isMoving;
    bool m_isPlaying;
    std::uint32_t m_createBlockIndex;
    float m_blockSize;
    float m_blockSpace;
    std::size_t m_rowCount;
    std::size_t m_colCount;

    sf::Font m_font;
    sf::RectangleShape m_baseBoard;
    sf::RectangleShape m_baseBlock;
    std::vector<BlockInfo> m_blockInfos;
    std::list<Block*> m_blocks;
    std::vector<std::vector<Block*>> m_board;
};
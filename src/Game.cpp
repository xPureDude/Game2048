#include "Game.hpp"

#include "SFML/Graphics/RenderTarget.hpp"

#include <iostream>

BlockInfo::BlockInfo(const sf::Color& backColor, float blockSize, const sf::Font& font, const sf::Color& valueColor,
                     std::uint32_t charSize, std::int32_t value)
{
    m_back.setSize({blockSize, blockSize});
    m_back.setOrigin({m_back.getSize().x / 2, m_back.getSize().y / 2});
    m_back.setFillColor(backColor);

    m_value.setFillColor(valueColor);
    m_value.setString(std::to_string(value));
    m_value.setCharacterSize(charSize);
    m_value.setFont(font);
    m_value.setStyle(sf::Text::Style::Bold);
    auto localBounds = m_value.getLocalBounds();
    m_value.setOrigin(localBounds.left + localBounds.width / 2.f,
                      localBounds.top + m_value.getLocalBounds().height / 2.f);
}

void BlockInfo::Render(sf::RenderTarget* target)
{
    target->draw(m_back);
    target->draw(m_value);
}

void BlockInfo::SetPosition(const sf::Vector2f& pos)
{
    m_back.setPosition(pos);
    m_value.setPosition(pos);
}

void Block::Update(const sf::Time& elapsed)
{
    auto originPos = m_game->GetGridPosition(m_originGrid);
    auto destPos = m_game->GetGridPosition(m_destGrid);
    if (m_pos != destPos)
    {
        m_moveElapsed += elapsed;
        if (m_moveElapsed >= sf::milliseconds(200))
        {
            m_moveElapsed = sf::Time::Zero;
            m_pos = destPos;
            m_originGrid = m_destGrid;
        }
        else
        {
            float rate = m_moveElapsed / sf::milliseconds(200);
            m_pos = originPos + (destPos - originPos) * rate;
            std::cout << "m_pos: x = " << m_pos.x << ", y = " << m_pos.y << " rate = " << rate << std::endl;
        }
    }
}

std::uint32_t Game::s_boardWidth = 500;
std::uint32_t Game::s_boardOffset = 100;
sf::Color Game::s_boardColor = sf::Color(0xBBADA0FF);
sf::Color Game::s_baseBlockColor = sf::Color(0xEEE4DA5F);
sf::Time Game::s_moveTime = sf::milliseconds(200);

Game::Game()
    : m_createBlockIndex(0),
      m_isMoving(false),
      m_isPlaying(false)
{
    if (m_font.loadFromFile("Vegur-Yg1a.otf") == false)
    {
        std::cout << "Game::Game(), load font failed, file: Vegur-Yg1a.otf" << std::endl;
    }
    m_baseBoard.setSize({(float)s_boardWidth, (float)s_boardWidth});
    m_baseBoard.setPosition(0.0f, s_boardOffset);
    m_baseBoard.setFillColor(s_boardColor);

    m_blockSize = (float)(s_boardWidth - (m_board.size() + 1) * 20) / m_board.size();
    m_baseBlock.setSize({m_blockSize, m_blockSize});
    m_baseBlock.setFillColor(s_baseBlockColor);
    m_baseBlock.setOrigin(m_baseBlock.getSize() / 2.f);

    m_blockInfos.emplace_back(sf::Color(0xEEE4DAFF), m_blockSize, m_font, sf::Color(0x776E65FF), 55, 2);
    m_blockInfos.emplace_back(sf::Color(0xEDE0C8FF), m_blockSize, m_font, sf::Color(0x776E65FF), 55, 4);
    m_blockInfos.emplace_back(sf::Color(0xF2B179FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 55, 8);
    m_blockInfos.emplace_back(sf::Color(0xF59563FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 50, 16);
    m_blockInfos.emplace_back(sf::Color(0xF67C5FFF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 50, 32);
    m_blockInfos.emplace_back(sf::Color(0xF65E3BFF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 50, 64);
    m_blockInfos.emplace_back(sf::Color(0xEDCF72FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 45, 128);
    m_blockInfos.emplace_back(sf::Color(0xEDCC61FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 45, 256);
    m_blockInfos.emplace_back(sf::Color(0xEDC850FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 45, 512);
    m_blockInfos.emplace_back(sf::Color(0xEDC53FFF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 40, 1024);
    m_blockInfos.emplace_back(sf::Color(0xEFC32FFF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 40, 2048);
    m_blockInfos.emplace_back(sf::Color(0xF0C430FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 40, 4096);
    m_blockInfos.emplace_back(sf::Color(0xF1C531FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 40, 8192);
    m_blockInfos.emplace_back(sf::Color(0xF2C632FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 35, 16384);
    m_blockInfos.emplace_back(sf::Color(0xF3C733FF), m_blockSize, m_font, sf::Color(0xFFFFFFFF), 35, 32768);

    _ResetBoard();
}

Game::~Game()
{
    for (auto& block : m_blocks)
    {
        delete block;
    }
}

void Game::Update(const sf::Time& elapsed)
{
    bool prevMoving = m_isMoving;
    m_isMoving = false;
    for (auto info : m_blocks)
    {
        if (info->m_originGrid != info->m_destGrid)
        {
            info->Update(elapsed);
            // After update
            if (info->m_originGrid == info->m_destGrid)
            {
                if (info->m_needGrow)
                {
                    info->m_index++;
                    if (info->m_index == m_blockInfos.size() - 1)
                    {
                        // Win
                    }
                    info->m_needGrow = false;
                }
            }
            else
            {
                m_isMoving = true;
            }
        }
    }

    // Create new block after move
    if (prevMoving && !m_isMoving)
    {
        _CreateNewBlock();
    }
}

void Game::Render(sf::RenderTarget* window)
{
    window->draw(m_baseBoard);

    for (std::size_t row = 0; row < m_board.size(); ++row)
    {
        for (std::size_t col = 0; col < m_board[row].size(); ++col)
        {
            m_baseBlock.setPosition(GetGridPosition({row, col}));
            window->draw(m_baseBlock);
        }
    }

    for (auto info : m_blocks)
    {
        m_blockInfos[info->m_index].SetPosition(info->m_pos);
        m_blockInfos[info->m_index].Render(window);
    }
}

void Game::HandleEvent(const sf::Event& e)
{
    if (m_isMoving)
        return;

    switch (e.key.code)
    {
    case sf::Keyboard::Left:
        _OnMoveLeft();
        break;
    case sf::Keyboard::Right:
        _OnMoveRight();
        break;
    case sf::Keyboard::Up:
        _OnMoveUp();
        break;
    case sf::Keyboard::Down:
        _OnMoveDown();
        break;
    default:
        break;
    }
}

void Game::OnNewGame()
{
    _ResetBoard();
    m_createBlockIndex = 0;
    m_isPlaying = true;
    _CreateNewBlock();
    _CreateNewBlock();
}

sf::Vector2f Game::GetGridPosition(const sf::Vector2<std::size_t>& grid)
{
    auto [row, col] = grid;
    auto xpos = (col + 1) * 20 + col * m_blockSize + m_blockSize / 2.f;
    auto ypos = (row + 1) * 20 + row * m_blockSize + m_blockSize / 2.f;
    return {xpos, ypos + s_boardOffset};
}

void Game::_OnMoveLeft()
{
    // move row by row
    for (std::size_t row = 0; row < m_board.size(); ++row)
    {
        for (std::size_t col = 0; col < m_board[row].size(); ++col)
        {
            auto block = m_board[row][col];
            if (block != nullptr)
            {
                // Find new pos to move
                for (std::size_t destCol = 0; destCol < col; ++destCol)
                {
                    if (_FindCanMoveGrid(block, row, col, row, destCol))
                    {
                        break;
                    }
                }
            }
        }
    }
}

void Game::_OnMoveRight()
{
    // move row by row
    for (std::size_t row = 0; row < m_board.size(); ++row)
    {
        for (std::size_t col = m_board[row].size() - 1; col >= 0; --col)
        {
            // Index overflow
            if (col > m_board[row].size())
                break;
            auto block = m_board[row][col];
            if (block != nullptr)
            {
                // Find new pos to move
                for (std::size_t destCol = m_board[row].size() - 1; destCol > col; --destCol)
                {
                    if (_FindCanMoveGrid(block, row, col, row, destCol))
                    {
                        break;
                    }
                }
            }
        }
    }
}

void Game::_OnMoveUp()
{
    // move col by col
    auto colCount = m_board.front().size();
    for (std::size_t col = 0; col < colCount; ++col)
    {
        for (std::size_t row = 0; row < m_board.size(); ++row)
        {
            auto block = m_board[row][col];
            if (block != nullptr)
            {
                // Find new pos to move
                for (std::size_t destRow = 0; destRow < row; ++destRow)
                {
                    if (_FindCanMoveGrid(block, row, col, destRow, col))
                    {
                        break;
                    }
                }
            }
        }
    }
}

void Game::_OnMoveDown()
{
    // move col by col
    auto colCount = m_board.front().size();
    for (std::size_t col = 0; col < colCount; ++col)
    {
        for (std::size_t row = m_board.size() - 1; row >= 0; --row)
        {
            // Index overflow
            if (row >= m_board.size())
                break;

            auto block = m_board[row][col];
            if (block != nullptr)
            {
                // Find new pos to move
                for (std::size_t destRow = m_board.size() - 1; destRow > row; --destRow)
                {
                    if (_FindCanMoveGrid(block, row, col, destRow, col))
                    {
                        break;
                    }
                }
            }
        }
    }
}

void Game::_ResetBoard()
{
    for (auto& info : m_blocks)
    {
        delete info;
    }
    m_blocks.clear();

    for (auto& line : m_board)
    {
        for (auto& cell : line)
        {
            cell = nullptr;
        }
    }
}

void Game::_CreateNewBlock()
{
    std::vector<std::pair<std::size_t, std::size_t>> availableGrid;
    for (std::size_t row = 0; row < m_board.size(); ++row)
    {
        for (std::size_t col = 0; col < m_board[row].size(); ++col)
        {
            if (m_board[row][col] == nullptr)
            {
                availableGrid.push_back(std::make_pair(row, col));
            }
        }
    }
    if (availableGrid.empty())
    {
        // GameOver
        m_isPlaying = true;
        return;
    }

    auto index = rand() % (availableGrid.size() - 1);
    auto [row, col] = availableGrid[index];

    // three "2" block then one "4" block
    auto block = new Block;
    block->m_index = m_createBlockIndex == 3 ? 1 : 0;
    block->m_originGrid = block->m_destGrid = {row, col};
    block->m_pos = GetGridPosition({row, col});
    block->m_game = this;

    m_blocks.push_back(block);
    m_board[row][col] = block;

    ++m_createBlockIndex;
    m_createBlockIndex %= 4;
}

bool Game::_FindCanMoveGrid(Block* originBlock, std::size_t oRow, std::size_t oCol, std::size_t dRow, std::size_t dCol)
{
    auto destBlock = m_board[dRow][dCol];
    if (destBlock != nullptr && destBlock->m_needGrow)
        return false;

    if (destBlock == nullptr || originBlock->m_index == destBlock->m_index)
    {
        // Find dest grid
        // Set new grid pos
        originBlock->m_destGrid = {dRow, dCol};
        m_board[oRow][oCol] = nullptr;

        if (destBlock != nullptr)
        {
            // delete old block
            m_blocks.remove(destBlock);
            delete destBlock;

            originBlock->m_needGrow = true;
        }

        m_board[dRow][dCol] = originBlock;
        return true;
    }
    return false;
}
#include "Game2048.hpp"

#include "../core/Window.hpp"

#include <iostream>

BlockInfo::BlockInfo(const sf::Color& backColor, const sf::Font& font, const sf::Color& valueColor, std::uint32_t charSize, std::int32_t value)
{
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

void BlockInfo::Render(Window* window)
{
    window->Render(m_back);
    window->Render(m_value);
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
        }
    }
}

std::uint32_t Game2048::s_boardWidth = 500;
sf::Color Game2048::s_boardColor = sf::Color(0xBBADA0FF);
sf::Color Game2048::s_baseBlockColor = sf::Color(0xEEE4DA5F);
sf::Time Game2048::s_moveTime = sf::milliseconds(200);

Game2048::Game2048()
    : m_createBlockIndex(0),
      m_blockSize(0),
      m_blockSpace(0),
      m_rowCount(0),
      m_colCount(0),
      m_isMoving(false),
      m_isPlaying(false),
      m_score(0)
{
    if (m_font.loadFromFile("SourceHanSansCN-Regular.otf") == false)
    {
        std::cout << "Game2048::Game(), load font failed, file: SourceHanSansCN-Regular.otf" << std::endl;
    }
    m_baseBoard.setSize({(float)s_boardWidth, (float)s_boardWidth});
    m_baseBoard.setFillColor(s_boardColor);

    m_blockInfos.emplace_back(sf::Color(0xEEE4DAFF), m_font, sf::Color(0x776E65FF), 55, 2);
    m_blockInfos.emplace_back(sf::Color(0xEDE0C8FF), m_font, sf::Color(0x776E65FF), 55, 4);
    m_blockInfos.emplace_back(sf::Color(0xF2B179FF), m_font, sf::Color(0xFFFFFFFF), 55, 8);
    m_blockInfos.emplace_back(sf::Color(0xF59563FF), m_font, sf::Color(0xFFFFFFFF), 50, 16);
    m_blockInfos.emplace_back(sf::Color(0xF67C5FFF), m_font, sf::Color(0xFFFFFFFF), 50, 32);
    m_blockInfos.emplace_back(sf::Color(0xF65E3BFF), m_font, sf::Color(0xFFFFFFFF), 50, 64);
    m_blockInfos.emplace_back(sf::Color(0xEDCF72FF), m_font, sf::Color(0xFFFFFFFF), 45, 128);
    m_blockInfos.emplace_back(sf::Color(0xEDCC61FF), m_font, sf::Color(0xFFFFFFFF), 45, 256);
    m_blockInfos.emplace_back(sf::Color(0xEDC850FF), m_font, sf::Color(0xFFFFFFFF), 45, 512);
    m_blockInfos.emplace_back(sf::Color(0xEDC53FFF), m_font, sf::Color(0xFFFFFFFF), 40, 1024);
    m_blockInfos.emplace_back(sf::Color(0xEFC32FFF), m_font, sf::Color(0xFFFFFFFF), 40, 2048);
    m_blockInfos.emplace_back(sf::Color(0xF0C734FF), m_font, sf::Color(0xFFFFFFFF), 40, 4096);
    m_blockInfos.emplace_back(sf::Color(0xF2C939FF), m_font, sf::Color(0xFFFFFFFF), 40, 8192);
    m_blockInfos.emplace_back(sf::Color(0xF2CD45FF), m_font, sf::Color(0xFFFFFFFF), 35, 16384);
    m_blockInfos.emplace_back(sf::Color(0xF4D249FF), m_font, sf::Color(0xFFFFFFFF), 35, 32768);
    m_blockInfos.emplace_back(sf::Color(0xF4E854FF), m_font, sf::Color(0xFFFFFFFF), 35, 65536);
}

Game2048::~Game2048()
{
    for (auto& block : m_blocks)
    {
        delete block;
    }
}

void Game2048::Update(const sf::Time& elapsed)
{
    bool prevMoving = m_isMoving;
    m_isMoving = false;
    auto info = m_blocks.begin();
    while (info != m_blocks.end())
    {
        if ((*info)->m_originGrid != (*info)->m_destGrid)
        {
            (*info)->Update(elapsed);
            // After update
            if ((*info)->m_originGrid == (*info)->m_destGrid)
            {
                auto destBlock = m_board[(*info)->m_destGrid.x][(*info)->m_destGrid.y];
                if (destBlock != nullptr && destBlock->m_needGrow && *info != destBlock)
                {
                    destBlock->m_index++;
                    m_score += static_cast<std::size_t>(std::pow(2, destBlock->m_index + 1));
                    std::cout << "score: " << m_score << std::endl;
                    destBlock->m_needGrow = false;
                    if (destBlock->m_index == m_blockInfos.size() - 1)
                    {
                        // Win
                    }
                    delete (*info);
                    info = m_blocks.erase(info);
                    continue;
                }
            }
            else
            {
                m_isMoving = true;
            }
        }
        ++info;
    }

    // Create new block after move
    if (prevMoving && !m_isMoving)
    {
        _CreateNewBlock();
    }
}

void Game2048::Render(Window* window)
{
    window->Render(m_baseBoard);

    for (std::size_t row = 0; row < m_rowCount; ++row)
    {
        for (std::size_t col = 0; col < m_colCount; ++col)
        {
            m_baseBlock.setPosition(GetGridPosition({row, col}));
            window->Render(m_baseBlock);
        }
    }

    for (auto info : m_blocks)
    {
        m_blockInfos[info->m_index].SetPosition(info->m_pos);
        m_blockInfos[info->m_index].Render(window);
    }
}

void Game2048::OnNewGame(std::size_t rowCount, std::size_t colCount)
{
    m_rowCount = rowCount;
    m_colCount = colCount;
    m_blockSpace = s_boardWidth / (m_colCount * 6.f + 1.f);
    m_blockSize = m_blockSpace * 5.f;
    m_baseBlock.setSize({m_blockSize, m_blockSize});
    m_baseBlock.setFillColor(s_baseBlockColor);
    m_baseBlock.setOrigin(m_baseBlock.getSize() / 2.f);

    for (auto& info : m_blockInfos)
    {
        info.m_back.setSize({m_blockSize, m_blockSize});
        info.m_back.setOrigin({m_blockSize / 2.f, m_blockSize / 2.f});
    }
    m_isPlaying = true;
    m_createBlockIndex = 0;
    m_score = 0;

    _ResetBoard();
    _CreateNewBlock();
    _CreateNewBlock();
}

void Game2048::SetPosition(const sf::Vector2f& pos)
{
    m_position = pos;
    m_baseBoard.setPosition(pos);
}

sf::Vector2f Game2048::GetGridPosition(const sf::Vector2<std::size_t>& grid)
{
    auto [row, col] = grid;
    auto xpos = (col + 1) * m_blockSpace + col * m_blockSize + m_blockSize / 2.f;
    auto ypos = (row + 1) * m_blockSpace + row * m_blockSize + m_blockSize / 2.f;
    return {xpos + m_position.x, ypos + m_position.y};
}

void Game2048::OnMoveLeft()
{
    if (m_isMoving)
        return;
    // move row by row
    for (std::size_t row = 0; row < m_rowCount; ++row)
    {
        // Last one block can't move
        for (std::size_t col = 1; col < m_colCount; ++col)
        {
            // Find first !nullptr grid
            auto block = m_board[row][col];
            if (block == nullptr)
                continue;

            std::size_t destCol = col - 1;
            while (destCol >= 0 && destCol < m_colCount)
            {
                if (m_board[row][destCol] != nullptr)
                    break;
                --destCol;
            }
            if (destCol > m_colCount)
                destCol = 0;

            _CheckMoveGrid(block, {row, col}, {row, destCol}, {row, destCol + 1});
        }
    }
}

void Game2048::OnMoveRight()
{
    if (m_isMoving)
        return;
    // move row by row
    for (std::size_t row = 0; row < m_rowCount; ++row)
    {
        // unsigned may overflow
        for (std::size_t col = m_colCount - 2; col >= 0 && col < m_colCount; --col)
        {
            auto block = m_board[row][col];
            if (block == nullptr)
                continue;

            std::size_t destCol = col + 1;
            while (destCol < m_colCount)
            {
                if (m_board[row][destCol] != nullptr)
                    break;
                ++destCol;
            }
            if (destCol == m_colCount)
                destCol = m_colCount - 1;

            _CheckMoveGrid(block, {row, col}, {row, destCol}, {row, destCol - 1});
        }
    }
}

void Game2048::OnMoveUp()
{
    if (m_isMoving)
        return;
    // move col by col
    for (std::size_t col = 0; col < m_colCount; ++col)
    {
        for (std::size_t row = 1; row < m_rowCount; ++row)
        {
            auto block = m_board[row][col];
            if (block == nullptr)
                continue;

            std::size_t destRow = row - 1;
            while (destRow >= 0 && destRow < m_rowCount)
            {
                if (m_board[destRow][col] != nullptr)
                    break;
                --destRow;
            }
            if (destRow > m_rowCount)
                destRow = 0;

            _CheckMoveGrid(block, {row, col}, {destRow, col}, {destRow + 1, col});
        }
    }
}

void Game2048::OnMoveDown()
{
    if (m_isMoving)
        return;
    // move col by col
    for (std::size_t col = 0; col < m_colCount; ++col)
    {
        // uint may overflow
        for (std::size_t row = m_rowCount - 2; row >= 0 && row < m_rowCount; --row)
        {
            auto block = m_board[row][col];
            if (block == nullptr)
                continue;

            std::size_t destRow = row + 1;
            while (destRow < m_rowCount)
            {
                if (m_board[destRow][col] != nullptr)
                    break;
                ++destRow;
            }
            if (destRow == m_rowCount)
                destRow = m_rowCount - 1;

            _CheckMoveGrid(block, {row, col}, {destRow, col}, {destRow - 1, col});
        }
    }
}

void Game2048::_ResetBoard()
{
    for (auto& info : m_blocks)
    {
        delete info;
    }
    m_blocks.clear();

    m_board.resize(m_rowCount);
    for (auto& row : m_board)
    {
        row.clear();
        row.resize(m_colCount, nullptr);
    }
}

void Game2048::_CreateNewBlock()
{
    std::vector<std::pair<std::size_t, std::size_t>> availableGrid;
    for (std::size_t row = 0; row < m_rowCount; ++row)
    {
        for (std::size_t col = 0; col < m_colCount; ++col)
        {
            if (m_board[row][col] == nullptr)
            {
                availableGrid.push_back(std::make_pair(row, col));
            }
        }
    }
    if (availableGrid.empty())
    {
        // Game2048Over
        m_isPlaying = true;
        return;
    }

    std::size_t index = 0;
    if (availableGrid.size() > 1)
    {
        index = rand() % (availableGrid.size() - 1);
    }

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

void Game2048::_CheckMoveGrid(Block* block, const Vector2size& oGrid, const Vector2size& dGrid, const Vector2size& pGrid)
{
    m_board[oGrid.x][oGrid.y] = nullptr;

    auto destBlock = m_board[dGrid.x][dGrid.y];
    if (destBlock == nullptr)
    {
        block->m_destGrid = dGrid;
        m_board[dGrid.x][dGrid.y] = block;
    }
    else
    {
        if (block->m_index == destBlock->m_index && !destBlock->m_needGrow)
        {
            // Move to merge
            destBlock->m_needGrow = true;
            block->m_destGrid = dGrid;
        }
        else
        {
            block->m_destGrid = pGrid;
            m_board[pGrid.x][pGrid.y] = block;
        }
    }
}
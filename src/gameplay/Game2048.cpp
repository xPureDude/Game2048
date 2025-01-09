#include "Game2048.hpp"

#include "../common/Log.hpp"
#include "../core/Window.hpp"

BlockInfo::BlockInfo(const sf::Color& backColor, std::shared_ptr<sf::Font> font, const sf::Color& valueColor, std::uint32_t charSize, std::int32_t value)
    : m_font(font),
      m_value(*m_font, std::to_string(value), charSize)
{
    m_back.setFillColor(backColor);

    m_value.setFillColor(valueColor);
    m_value.setStyle(sf::Text::Style::Bold);
    auto localBounds = m_value.getLocalBounds();
    m_value.setOrigin(localBounds.getCenter());
}

void BlockInfo::Render(sf::RenderTarget& target)
{
    target.draw(m_back);
    target.draw(m_value);
}

void BlockInfo::Render(Window* window)
{
    window->Render(m_back);
    window->Render(m_value);
}

void BlockInfo::SetScale(const sf::Vector2f& scale)
{
    m_back.setScale(scale);
    m_value.setScale(scale);
}

void BlockInfo::SetPosition(const sf::Vector2f& pos)
{
    m_back.setPosition(pos);
    m_value.setPosition(pos);
}

void Block::Update(const sf::Time& elapsed)
{
    switch (m_state)
    {
    case BlockState::Moving:
        _UpdateMove(elapsed);
        break;

    case BlockState::Growing:
    case BlockState::Borning:
        _UpdateGrow(elapsed);
        break;

    default:
        break;
    }
}

void Block::_UpdateMove(const sf::Time& elapsed)
{
    auto originPos = m_game->GetGridPosition(m_originGrid);
    auto destPos = m_game->GetGridPosition(m_destGrid);
    if (m_pos != destPos)
    {
        m_actionElapsed += elapsed;
        if (m_actionElapsed >= sf::milliseconds(200))
        {
            m_actionElapsed = sf::Time::Zero;
            m_pos = destPos;
            m_originGrid = m_destGrid;
            m_state = BlockState::Idle;
        }
        else
        {
            float rate = m_actionElapsed / sf::milliseconds(200);
            m_pos = originPos + (destPos - originPos) * rate;
        }
    }
    else
    {
        m_state = BlockState::Idle;
    }
}

void Block::_UpdateGrow(const sf::Time& elapsed)
{
    m_actionElapsed += elapsed;
    if (m_actionElapsed >= sf::milliseconds(100))
    {
        m_actionElapsed = sf::Time::Zero;
        m_scale = {1, 1};
        m_state = BlockState::Idle;
    }
    else
    {
        float scale = m_actionElapsed / sf::milliseconds(100);
        m_scale = {scale, scale};
    }
}

sf::Time Game2048::s_moveTime = sf::milliseconds(200);

Game2048::Game2048(std::shared_ptr<sf::Font> font)
    : m_boardSprite(m_boardTexture.getTexture()),
      m_blockSize(0),
      m_blockSpace(0),
      m_rowCount(0),
      m_colCount(0),
      m_canMove(true),
      m_isPlaying(false),
      m_score(0),
      m_createBlockIndex(0)
{
    m_blockInfos.emplace_back(sf::Color(0xEEE4DAFF), font, sf::Color(0x776E65FF), 55, 2);
    m_blockInfos.emplace_back(sf::Color(0xEDE0C8FF), font, sf::Color(0x776E65FF), 55, 4);
    m_blockInfos.emplace_back(sf::Color(0xF2B179FF), font, sf::Color(0xFFFFFFFF), 55, 8);
    m_blockInfos.emplace_back(sf::Color(0xF59563FF), font, sf::Color(0xFFFFFFFF), 50, 16);
    m_blockInfos.emplace_back(sf::Color(0xF67C5FFF), font, sf::Color(0xFFFFFFFF), 50, 32);
    m_blockInfos.emplace_back(sf::Color(0xF65E3BFF), font, sf::Color(0xFFFFFFFF), 50, 64);
    m_blockInfos.emplace_back(sf::Color(0xEDCF72FF), font, sf::Color(0xFFFFFFFF), 45, 128);
    m_blockInfos.emplace_back(sf::Color(0xEDCC61FF), font, sf::Color(0xFFFFFFFF), 45, 256);
    m_blockInfos.emplace_back(sf::Color(0xEDC850FF), font, sf::Color(0xFFFFFFFF), 45, 512);
    m_blockInfos.emplace_back(sf::Color(0xEDC53FFF), font, sf::Color(0xFFFFFFFF), 40, 1024);
    m_blockInfos.emplace_back(sf::Color(0xEFC32FFF), font, sf::Color(0xFFFFFFFF), 40, 2048);
    m_blockInfos.emplace_back(sf::Color(0xF0C734FF), font, sf::Color(0xFFFFFFFF), 40, 4096);
    m_blockInfos.emplace_back(sf::Color(0xF2C939FF), font, sf::Color(0xFFFFFFFF), 40, 8192);
    m_blockInfos.emplace_back(sf::Color(0xF2CD45FF), font, sf::Color(0xFFFFFFFF), 35, 16384);
    m_blockInfos.emplace_back(sf::Color(0xF4D249FF), font, sf::Color(0xFFFFFFFF), 35, 32768);
    m_blockInfos.emplace_back(sf::Color(0xF4E854FF), font, sf::Color(0xFFFFFFFF), 35, 65536);
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
    m_canMove = true;
    bool needCreate = false;

    std::set<std::size_t> deleteLater;

    for (auto i = 0; i < m_blocks.size(); ++i)
    {
        auto block = m_blocks[i];
        if (block->m_state == BlockState::Moving)
        {
            block->Update(elapsed);
            if (block->m_state == BlockState::Idle)
            {
                needCreate = true;
                auto destBlock = m_board[block->m_destGrid.x][block->m_destGrid.y];
                if (destBlock && block != destBlock && destBlock->m_needGrow)
                {
                    destBlock->m_index++;
                    destBlock->m_state = BlockState::Growing;
                    destBlock->m_needGrow = false;
                    destBlock->m_scale = {0, 0};
                    m_canMove = false;

                    deleteLater.insert(i);
                }
            }
            else
            {
                DBG("Block is moving: {},{}", block->m_originGrid.x, block->m_originGrid.y);
                m_canMove = false;
            }
        }
        else if (block->m_state == BlockState::Growing)
        {
            block->Update(elapsed);
            if (block->m_state == BlockState::Idle)
            {
                m_score += static_cast<std::size_t>(std::pow(2, block->m_index + 1));
                if (block->m_index == m_blockInfos.size() - 1)
                {
                    // Win
                }

                if (m_callbacks.contains(GameSignal::ScoreChange))
                {
                    std::any param = std::make_any<std::size_t>(m_score);
                    for (auto& callback : m_callbacks[GameSignal::ScoreChange])
                    {
                        callback(param);
                    }
                }
            }
            else
            {
                m_canMove = false;
                DBG("Block is growing: {},{}", block->m_originGrid.x, block->m_originGrid.y);
            }
        }
        else if (block->m_state == BlockState::Borning)
        {
            block->Update(elapsed);
            if (block->m_state != BlockState::Idle)
            {
                m_canMove = false;
            }
        }
    }

    for (auto riter = deleteLater.rbegin(); riter != deleteLater.rend(); ++riter)
    {
        std::swap(m_blocks[*riter], m_blocks[m_blocks.size() - 1]);
        m_blockCache.push_back(m_blocks.back());
        m_blocks.pop_back();
    }

    // Check need create block: Every move over time
    if (needCreate)
    {
        _CreateNewBlock();
    }
}

void Game2048::Render(sf::RenderTarget* target)
{
    m_boardTexture.clear(sf::Color::Transparent);
    for (auto info : m_blocks)
    {
        m_blockInfos[info->m_index].SetScale(info->m_scale);
        m_blockInfos[info->m_index].SetPosition(info->m_pos);
        m_blockInfos[info->m_index].Render(m_boardTexture);
    }
    m_boardTexture.display();
    target->draw(m_boardSprite);
}

void Game2048::OnNewGame(const NewGameInfo& info)
{
    m_rowCount = info.m_rowCount;
    m_colCount = info.m_colCount;
    m_blockSize = info.m_blockSize;
    m_blockSpace = info.m_blockSpace;
    m_position = info.m_position;
    bool ret = m_boardTexture.resize(info.m_boardSize);
    if (!ret)
    {
        return;
    }
    m_boardSprite.setTexture(m_boardTexture.getTexture(), true);
    m_boardSprite.setPosition(m_position);

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
    m_boardSprite.setPosition(pos);
}

sf::Vector2f Game2048::GetGridPosition(const sf::Vector2<std::size_t>& grid)
{
    auto [row, col] = grid;
    auto xpos = (col + 1) * m_blockSpace + col * m_blockSize + m_blockSize / 2.f;
    auto ypos = (row + 1) * m_blockSpace + row * m_blockSize + m_blockSize / 2.f;
    return {xpos, ypos};
}

void Game2048::ConnectGameSignalCallback(GameSignal signal, CallbackType callback)
{
    m_callbacks[signal].push_back(callback);
}

void Game2048::OnMoveLeft()
{
    if (!m_canMove)
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
    if (!m_canMove)
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
    if (!m_canMove)
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
    if (!m_canMove)
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
        // Game Over
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
    Block* block = nullptr;
    if (!m_blockCache.empty())
    {
        block = m_blockCache.back();
        m_blockCache.pop_back();
    }
    else
    {
        block = new Block;
    }
    block->m_index = m_createBlockIndex == 3 ? 1 : 0;
    block->m_originGrid = block->m_destGrid = {row, col};
    block->m_pos = GetGridPosition({row, col});
    block->m_game = this;
    block->m_state = BlockState::Borning;
    block->m_scale = {0, 0};

    m_blocks.push_back(block);
    m_board[row][col] = block;

    DBG("Create new Block: {},{}", row, col);

    ++m_createBlockIndex;
    m_createBlockIndex %= 4;
}

void Game2048::_CheckMoveGrid(Block* block, const Vector2size& oGrid, const Vector2size& dGrid, const Vector2size& pGrid)
{
    auto realGrid = dGrid;
    bool needGrow = false;

    auto destBlock = m_board[dGrid.x][dGrid.y];
    if (destBlock)
    {
        if (block->m_index == destBlock->m_index && !destBlock->m_needGrow)
        {
            // Move to merge
            destBlock->m_needGrow = true;
            needGrow = true;
        }
        else
        {
            realGrid = pGrid;
        }
    }

    if (oGrid != realGrid)
    {
        m_board[oGrid.x][oGrid.y] = nullptr;
        if (!needGrow)
        {
            m_board[realGrid.x][realGrid.y] = block;
        }
        block->m_destGrid = realGrid;
        block->m_state = BlockState::Moving;
        DBG("Block Move: {},{} to {},{}", oGrid.x, oGrid.y, realGrid.x, realGrid.y);
    }
}
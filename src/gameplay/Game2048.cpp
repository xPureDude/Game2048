#include "Game2048.hpp"

#include "../common/Log.hpp"
#include "../core/Window.hpp"

BlockInfo::BlockInfo(sf::Texture* texture, const sf::IntRect& rect)
{
    m_block.setTexture(texture, true);
    m_block.setTextureRect(rect);
    m_block.setSize(sf::Vector2f(rect.size));
}

void BlockInfo::Render(sf::RenderTarget& target)
{
    target.draw(m_block);
}

void BlockInfo::SetScale(const sf::Vector2f& scale)
{
    m_block.setScale(scale);
}

void BlockInfo::SetPosition(const sf::Vector2f& pos)
{
    m_block.setPosition(pos);
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
        if (m_actionElapsed >= sf::milliseconds(100))
        {
            m_actionElapsed = sf::Time::Zero;
            m_pos = destPos;
            m_originGrid = m_destGrid;
            m_state = BlockState::Idle;
        }
        else
        {
            float rate = m_actionElapsed / sf::milliseconds(100);
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

Game2048::Game2048(std::shared_ptr<sf::Texture> blockTexture)
    : m_delayInput(DelayMove::None),
      m_boardSprite(m_boardTexture.getTexture()),
      m_blockSize(0),
      m_blockSpace(0),
      m_rowCount(0),
      m_colCount(0),
      m_blockTexture(blockTexture),
      m_totalState(BlockState::Borning),
      m_isContinue(false),
      m_score(0),
      m_createBlockIndex(0)
{
    sf::Vector2i blockSize{100, 100};
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{0, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{100, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{200, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{300, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{400, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{500, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{600, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{700, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{800, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{900, 0}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{0, 100}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{100, 100}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{200, 100}, blockSize});
    m_blockInfos.emplace_back(m_blockTexture.get(), sf::IntRect{{300, 100}, blockSize});
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
    BlockState prevState = m_totalState;
    for (auto block : m_blocks)
    {
        block->Update(elapsed);
    }

    switch (m_totalState)
    {
    case BlockState::Moving:
        _OnMovingState();
        break;
    case BlockState::Growing:
        _OnGrowingState();
        break;
    case BlockState::Borning:
        _OnBorningState();
        break;
    case BlockState::Idle:
    {
        break;
    }
    }

    if (prevState != m_totalState && m_totalState == BlockState::Idle)
    {
        if (_CheckLose())
        {
            DBG("Game Lose, total score: {}", m_score);
            std::any isWin = std::make_any<bool>(false);
            m_signalQueue.emplace_back(GameSignal::GameOver, isWin);
        }
    }

    for (auto& signal : m_signalQueue)
    {
        if (m_callbacks.contains(signal.m_signal))
        {
            for (auto& callback : m_callbacks[signal.m_signal])
            {
                callback(signal.m_param);
            }
        }
    }
    m_signalQueue.clear();

    _DelayMove();
}

void Game2048::Render(Window* window)
{
    m_boardTexture.clear(sf::Color::Transparent);
    for (auto info : m_blocks)
    {
        m_blockInfos[info->m_index].SetScale(info->m_scale);
        m_blockInfos[info->m_index].SetPosition(info->m_pos);
        m_blockInfos[info->m_index].Render(m_boardTexture);
    }
    m_boardTexture.display();
    window->Render(m_boardSprite);
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
        info.m_block.setSize({m_blockSize, m_blockSize});
        info.m_block.setOrigin({m_blockSize / 2.f, m_blockSize / 2.f});
    }
    m_createBlockIndex = 0;
    m_isContinue = false;
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
    if (m_totalState != BlockState::Idle)
    {
        m_delayInput = DelayMove::Left;
        return;
    }
    m_delayInput = DelayMove::None;

    bool actualMove = false;
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

            if (_CheckMoveGrid(block, {row, col}, {row, destCol}, {row, destCol + 1}))
            {
                actualMove = true;
            }
        }
    }

    if (actualMove)
    {
        m_totalState = BlockState::Moving;
    }
}

void Game2048::OnMoveRight()
{
    if (m_totalState != BlockState::Idle)
    {
        m_delayInput = DelayMove::Right;
        return;
    }
    m_delayInput = DelayMove::None;

    bool actualMove = false;
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

            if (_CheckMoveGrid(block, {row, col}, {row, destCol}, {row, destCol - 1}))
            {
                actualMove = true;
            }
        }
    }

    if (actualMove)
    {
        m_totalState = BlockState::Moving;
    }
}

void Game2048::OnMoveUp()
{
    if (m_totalState != BlockState::Idle)
    {
        m_delayInput = DelayMove::Up;
        return;
    }
    m_delayInput = DelayMove::None;

    bool actrualMoved = false;
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

            if (_CheckMoveGrid(block, {row, col}, {destRow, col}, {destRow + 1, col}))
            {
                actrualMoved = true;
            }
        }
    }

    if (actrualMoved)
    {
        m_totalState = BlockState::Moving;
    }
}

void Game2048::OnMoveDown()
{
    if (m_totalState != BlockState::Idle)
    {
        m_delayInput = DelayMove::Down;
        return;
    }
    m_delayInput = DelayMove::None;

    bool actrualMove = false;
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

            if (_CheckMoveGrid(block, {row, col}, {destRow, col}, {destRow - 1, col}))
            {
                actrualMove = true;
            }
        }
    }

    if (actrualMove)
        m_totalState = BlockState::Moving;
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

bool Game2048::_CheckLose()
{
    if (m_blocks.size() < m_rowCount * m_colCount)
        return false;

    for (std::size_t row = 0; row < m_rowCount; ++row)
    {
        for (std::size_t col = 0; col < m_colCount; ++col)
        {
            // Check right block
            if (col + 1 < m_colCount)
            {
                if (m_board[row][col]->m_index == m_board[row][col + 1]->m_index)
                    return false;
            }
            // Check bottom block
            if (row + 1 < m_rowCount)
            {
                if (m_board[row][col]->m_index == m_board[row + 1][col]->m_index)
                    return false;
            }
        }
    }

    return true;
}

bool Game2048::_CheckMoveGrid(Block* block, const Vector2size& oGrid, const Vector2size& dGrid, const Vector2size& pGrid)
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
        return true;
    }

    return false;
}

void Game2048::_DelayMove()
{
    if (m_delayInput != DelayMove::None)
    {
        switch (m_delayInput)
        {
        case DelayMove::None:
            break;
        case DelayMove::Left:
            OnMoveLeft();
            break;
        case DelayMove::Right:
            OnMoveRight();
            break;
        case DelayMove::Up:
            OnMoveUp();
            break;
        case DelayMove::Down:
            OnMoveDown();
            break;
        }
    }
}

void Game2048::_OnMovingState()
{
    bool needGrow = false;
    bool moveEnd = true;
    std::set<std::size_t> deleteLater;
    for (auto i = 0; i < m_blocks.size(); ++i)
    {
        auto block = m_blocks[i];
        if (block->m_state == BlockState::Idle)
        {
            auto destBlock = m_board[block->m_destGrid.x][block->m_destGrid.y];
            if (destBlock && block != destBlock && destBlock->m_needGrow)
            {
                needGrow = true;
                deleteLater.insert(i);
            }
        }
        else
        {
            DBG("Block is moving: {},{}", block->m_originGrid.x, block->m_originGrid.y);
            moveEnd = false;
        }
    }

    if (moveEnd)
    {
        for (auto riter = deleteLater.rbegin(); riter != deleteLater.rend(); ++riter)
        {
            std::swap(m_blocks[*riter], m_blocks[m_blocks.size() - 1]);
            m_blockCache.push_back(m_blocks.back());
            m_blocks.pop_back();
        }

        if (needGrow)
        {
            for (auto block : m_blocks)
            {
                if (block->m_needGrow)
                {
                    block->m_index++;
                    block->m_state = BlockState::Growing;
                    block->m_needGrow = false;
                    block->m_scale = {0, 0};
                    m_score += static_cast<std::size_t>(std::pow(2, block->m_index + 1));
                }
            }
            m_totalState = BlockState::Growing;
        }
        else
        {
            _CreateNewBlock();
            m_totalState = BlockState::Borning;
        }
    }
}

void Game2048::_OnGrowingState()
{
    bool growEnd = true;
    bool scoreChange = false;
    bool isWin = false;
    for (auto i = 0; i < m_blocks.size(); ++i)
    {
        auto block = m_blocks[i];

        if (block->m_state == BlockState::Idle)
        {
            if (!m_isContinue && block->m_index == m_colCount * m_rowCount - 6)
            {
                // Win
                isWin = true;
            }
            scoreChange = true;
        }
        else
        {
            growEnd = false;
            DBG("Block is growing: {},{}", block->m_originGrid.x, block->m_originGrid.y);
        }
    }

    if (scoreChange)
    {
        std::any param = std::make_any<std::size_t>(m_score);
        m_signalQueue.emplace_back(GameSignal::ScoreChange, param);
    }

    if (isWin)
    {
        m_signalQueue.emplace_back(GameSignal::GameOver, isWin);
        m_isContinue = true;
    }

    if (growEnd)
    {
        _CreateNewBlock();
        m_totalState = BlockState::Borning;
    }
}

void Game2048::_OnBorningState()
{
    bool bornEnd = true;
    for (auto block : m_blocks)
    {
        if (block->m_state != BlockState::Idle)
        {
            bornEnd = false;
        }
    }

    if (bornEnd)
    {
        m_totalState = BlockState::Idle;
    }
}

void Game2048::_OnIdleState()
{
}
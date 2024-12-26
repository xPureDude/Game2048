#include "ResourceManager.hpp"

template <>
bool ResourceInfo<sf::Font>::Load()
{
    if (m_res != nullptr)
        return false;
    m_res = new sf::Font;
    if (m_res->openFromFile(m_filePath) == false)
        return false;
    m_count = 0;
    return true;
}
#include "ResourceManager.hpp"

template <>
bool ResourceInfo<sf::Font>::_Load()
{
    if (m_res != nullptr)
        return false;
    m_res = std::make_shared<sf::Font>();

    if (m_res->openFromFile(m_filePath) == false)
        return false;

    return true;
}
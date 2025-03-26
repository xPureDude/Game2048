#include "ConfigManager.hpp"

#include "SFML/Window.hpp" // IWYU pragma: keep

ConfigManager::ConfigManager()
{
    InitDefaultConfig();
}

ConfigManager::~ConfigManager()
{
}

WindowConfig& ConfigManager::GetWindowConfig()
{
    return m_windowConfig;
}

VisualConfig& ConfigManager::GetVisualConfig()
{
    return m_visualConfig;
}

void ConfigManager::InitDefaultConfig()
{
    m_windowConfig.m_title = "PureGame2048";
    m_windowConfig.m_height = 700;
    m_windowConfig.m_width = 500;
    m_windowConfig.m_state = static_cast<std::int32_t>(sf::State::Windowed);
    m_windowConfig.m_style = sf::Style::Close | sf::Style::Titlebar;

    m_visualConfig.m_fps = 60;
    m_visualConfig.m_language = "Chinese";
}
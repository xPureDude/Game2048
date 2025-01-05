#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include <cstdint>
#include <string>

struct WindowConfig
{
    std::string m_title;
    std::uint32_t m_width;
    std::uint32_t m_height;
    std::int32_t m_style;
    std::int32_t m_state;
};

class ConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();

    WindowConfig& GetWindowConfig();

private:
    void InitDefaultConfig();

private:
    // Window
    WindowConfig m_windowConfig;
};

#endif // CONFIGMANAGER_HPP
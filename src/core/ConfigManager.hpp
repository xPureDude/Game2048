#pragma once

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

struct VisualConfig
{
    std::string m_language;
    std::int32_t m_fps;
};

struct KeyBindConfig
{
};

class ConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();

    WindowConfig& GetWindowConfig();
    VisualConfig& GetVisualConfig();

private:
    void InitDefaultConfig();

private:
    // Window
    WindowConfig m_windowConfig;
    VisualConfig m_visualConfig;
};

#ifndef PCH_HPP
#define PCH_HPP

// IWYU pragma: begin_keep
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "common/Log.hpp"
#include "tinyxml/tinyxml2.h"

#include <any>
#include <cassert>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>


// IWYU pragma: end_keep
using CallbackType = std::function<void(const std::any&)>;

#define BindCallback(callback) std::bind((callback), this, std::placeholders::_1)

class SharedContext;
class TextureManager;
class FontManager;
class InputManager;
class Window;
class SceneManager;
class GuiManager;

#endif // PCH_HPP
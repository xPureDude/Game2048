#ifndef TEXTSTRING_MANAGER_HPP
#define TEXTSTRING_MANAGER_HPP

#include "SFML/System/String.hpp"

#include <map>
#include <string>

enum class TextString : std::uint64_t
{
    BEGIN_GAME,
    QUIT_GAME,
    CONTINUE,
    REPLAY,
    YOU_WIN,
    YOU_LOSE,
    BACK_TO_MENU
};

class TextStringManager
{
public:
    TextStringManager();
    ~TextStringManager();

    bool LoadTextStringFromFile(const std::string_view& file, const std::string& local);

    sf::String FindTextString(TextString id);

private:
    std::map<TextString, sf::String> m_textStrings;
};

#endif // LOCALIZE_MANAGER_HPP
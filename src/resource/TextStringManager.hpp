#ifndef TEXTSTRING_MANAGER_HPP
#define TEXTSTRING_MANAGER_HPP

#include <string>

#include "SFML/System/String.hpp"
#include "event/EventListenable.hpp"

#include "unordered_map"

class TextStringManager : public EventListenable
{
public:
    TextStringManager();
    ~TextStringManager();

    bool LoadTextStringFromFile(const std::string_view& file, const std::string& local);

    sf::String FindTextString(const std::string& id);

    static sf::String PackFormatString(const sf::String& str, const std::vector<std::string>& args);

private:
    void _OnLanguageChanged(evt::Base* event);

private:
    std::string_view m_filePath;
    std::unordered_map<std::string, sf::String> m_textStrings;
};

#endif // LOCALIZE_MANAGER_HPP
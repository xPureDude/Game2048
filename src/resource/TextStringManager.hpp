#ifndef TEXTSTRING_MANAGER_HPP
#define TEXTSTRING_MANAGER_HPP

#include <map>
#include <string>

class TextStringManager
{
public:
    TextStringManager();
    ~TextStringManager();

    bool LoadTextStringFromFile(const std::string_view& file, const std::string& local);

    std::string FindTextString(std::size_t id);

private:
    std::map<std::size_t, std::string> m_textStrings;
};

#endif // LOCALIZE_MANAGER_HPP
#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include "ResourceManager.hpp"

class FontManager : public ResourceManager<sf::Font>
{
public:
    virtual bool LoadResourceInfoFromFile(const std::string& file) override;

private:
    bool _LoadResourceInfoFromXml(const std::string& file);
};

#endif // FONTMANAGER_HPP
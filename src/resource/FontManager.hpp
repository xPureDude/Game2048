#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include "ResourceManager.hpp"

class FontManager : public ResourceManager<sf::Font>
{
public:
    virtual bool LoadResourceInfo(const std::string& file) override;
};

#endif // FONTMANAGER_HPP
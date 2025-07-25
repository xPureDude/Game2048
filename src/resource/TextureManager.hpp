#pragma once

#include "ResourceManager.hpp"

class TextureManager : public ResourceManager<sf::Texture>
{
public:
    virtual bool LoadResourceInfoFromFile(const std::string& file);

private:
    bool _LoadResourceInfoFromXml(const std::string& file);
};

#pragma once

#include "ResourceManager.hpp"

class TextureManager : public ResourceManager<sf::Texture>
{
public:
    TextureManager();
    virtual ~TextureManager();

    virtual bool LoadResourceInfo(const std::string& file);

private:
    bool _LoadResourceInfoFromXml(const std::string& file);
};


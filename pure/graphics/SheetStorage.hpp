#pragma once

#include <map>
#include <string>

class TextureManager;
class AnimationSheet;

class SheetStorage final
{
public:
    SheetStorage();
    ~SheetStorage();

    void SetTextureManager(TextureManager* manager) { m_manager = manager; }

    AnimationSheet* GetAnimationSheet(const std::string& name);

    bool LoadAtlasPackageFromXml(const std::string& file);

private:
    TextureManager* m_manager;
    std::map<std::string, AnimationSheet*> m_sheets;
};
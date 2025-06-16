#ifndef SHEETSTORAGE_HPP
#define SHEETSTORAGE_HPP

#include <map>
#include <string>

class AnimationSheet;

class SheetStorage final
{
public:
    SheetStorage();
    ~SheetStorage();

    AnimationSheet* GetAnimationSheet(const std::string& name);

    bool LoadAtlasPackageFromXml(const std::string& file);

private:
    std::map<std::string, AnimationSheet*> m_sheets;
};

#endif // SHEETSTORAGE_HPP
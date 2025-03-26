#include "SheetStorage.hpp"

#include "AnimationSheet.hpp"
#include "AtlasPackage.hpp"
#include "common/Log.hpp"

SheetStorage::SheetStorage()
    : m_manager(nullptr)
{
}

SheetStorage::~SheetStorage()
{
    for (auto& it : m_sheets)
    {
        delete it.second;
    }
}

AnimationSheet* SheetStorage::GetAnimationSheet(const std::string& name)
{
    auto it = m_sheets.find(name);
    if (it == m_sheets.end())
        return nullptr;
    return it->second;
}

bool SheetStorage::LoadAtlasPackageFromXml(const std::string& file)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file.c_str()) != tinyxml2::XML_SUCCESS)
    {
        ERR("SheetStorage::LoadAnimationSheetFromXml, failed to load file: {}", file);
        return false;
    }

    std::size_t loadBefore = m_sheets.size();
    auto rootElem = doc.RootElement();
    for (auto e = rootElem->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (std::strcmp(e->Value(), "AtlasPackage") == 0)
        {
            AtlasPackage* sheet = new AtlasPackage(m_manager);
            if (sheet->LoadFromXmlElement(e) == false)
            {
                delete sheet;
                DBG("SheetStorage::LoadAnimationSheetFromXml, failed to load AtlasPackage");
                return false;
            }
            auto ret = m_sheets.try_emplace(sheet->GetSheetName(), sheet);
            if (ret.second == false)
            {
                delete sheet;
                DBG("SheetStorage::LoadAnimationSheetFromXml, failed to emplace AtlasPackage");
                return false;
            }
        }
    }

    INF("SheetStorage::LoadAnimationSheetFromXml, load sheet count: {}", m_sheets.size() - loadBefore);

    return true;
}

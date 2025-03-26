#ifndef RESOURCE_PACKER_HPP
#define RESOURCE_PACKER_HPP

#include <string>

class ResourcePacker
{
public:
    ResourcePacker();
    ~ResourcePacker();

    bool LoadResourceFromFile(const std::string& srcFile);
    bool SaveResourceToFile(const std::string& toFile);

    bool LoadResourceFromPack(const std::string& srcFile);
    bool DumpResourceToPack(const std::string& toFile);
};

#endif // RESOURCE_PACKER_HPP
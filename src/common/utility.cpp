#include "utility.hpp"

std::vector<std::string> SplitString(std::string& str, char c)
{
    std::vector<std::string> ret;
    uint32_t prevIdx = 0;
    for (uint32_t idx = 0; idx < str.size(); ++idx)
    {
        if (str[idx] == c)
        {
            ret.push_back(str.substr(prevIdx, idx - prevIdx));
            prevIdx = idx + 1;
        }
    }
    ret.push_back(str.substr(prevIdx));
    return ret;
}
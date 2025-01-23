#include "SharedContext.hpp"

SharedContext::SharedContext()
{
}

SharedContext::~SharedContext()
{
    for (auto& it : m_ctx)
    {
        it.second.Release();
    }
}
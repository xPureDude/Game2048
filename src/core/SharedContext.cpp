#include "SharedContext.hpp"

SharedContext SharedContext::s_obj;

SharedContext::~SharedContext()
{
    for (auto& it : m_ctxs)
    {
        it.second.Release();
    }
}

SharedContext& SharedContext::Instance()
{
    return s_obj;
}
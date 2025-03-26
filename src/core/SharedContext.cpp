#include "SharedContext.hpp"

SharedContext SharedContext::s_obj;

SharedContext::~SharedContext()
{
    ReleaseAll();
}

SharedContext* SharedContext::Instance()
{
    return &s_obj;
}

void SharedContext::ReleaseAll()
{
    for (auto it = m_ctxList.rbegin(); it != m_ctxList.rend(); ++it)
    {
        (*it)->Release();
    }
    m_ctxList.clear();
    m_ctxs.clear();
}
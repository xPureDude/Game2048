#ifndef SHAREDCONTEXTDEPENDENT_HPP
#define SHAREDCONTEXTDEPENDENT_HPP

class SharedContext;

class SharedContextDependent
{
public:
    SharedContextDependent(SharedContext* ctx)
        : m_ctx(ctx)
    {
    }

    SharedContext* GetSharedContext() { return m_ctx; }

protected:
    SharedContext* m_ctx;
};

#endif // SHAREDCONTEXTDEPENDENT_HPP
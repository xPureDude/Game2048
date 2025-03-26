#ifndef GAMEAPP_INCLUDE
#define GAMEAPP_INCLUDE

#include "core/PureFramework.hpp"

class GameApp : public PureFramework
{
public:
    GameApp();
    virtual ~GameApp();

    bool Init();

private:
    bool _LoadResources();
};

#endif // GAMEAPP_INCLUDE
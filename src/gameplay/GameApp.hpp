#include "../core/PureFramework.hpp"

class GameApp : public PureFramework
{
public:
    GameApp();
    virtual ~GameApp();

    bool Init();

private:
    bool _LoadResources();
};
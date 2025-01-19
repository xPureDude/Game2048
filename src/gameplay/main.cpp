#include "GameApp.hpp"

int main(int argc, char** argv)
{
    GameApp app;
    if (app.Init() == false)
    {
        app.UnInit();
        return -1;
    }

    app.Run();
    app.UnInit();

    return 0;
}
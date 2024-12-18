#include "GameApp.hpp"

int main(int argc, char** argv)
{
    GameApp app;
    if (app.Init() == false)
        return -1;

    app.Run();
    app.UnInit();

    return 0;
}
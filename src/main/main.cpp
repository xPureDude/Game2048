#include "../pure/core/PureFramework.hpp"

int main(int argc, char** argv)
{
    PureFramework* pure = new PureFramework;
    if (pure->Init() == false)
        return -1;
    pure->Run();

    return 0;
}
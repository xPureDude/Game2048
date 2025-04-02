#pragma once

namespace evt
{
class GlobalBase
{
};

class FullscreenToggle : public GlobalBase
{
public:
    FullscreenToggle() {}
};

class UIBase
{
};

class BeginGame : public UIBase
{
public:
    BeginGame() {}
};

class InputBase
{
};

class MouseMove : public InputBase
{
public:
    MouseMove() {}
};

class MouseButtonDown : public InputBase
{
public:
    MouseButtonDown() {}
};

class SceneBase
{
};

class MoveLeft : public SceneBase
{
public:
    MoveLeft() {}
};

class MoveRight : public SceneBase
{
public:
    MoveRight() {}
};

class MoveUp : public SceneBase
{
public:
    MoveUp() {}
};

class MoveDown : public SceneBase
{
public:
    MoveDown() {}
};

class PauseGame : public SceneBase
{
public:
    PauseGame() {}
};

} // namespace evt
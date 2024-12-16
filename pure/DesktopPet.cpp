#include "DesktopPet.hpp"

#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "state/KnightState.hpp"
#include "state/State.hpp"

DesktopPet::DesktopPet(SharedContext* ctx)
    : m_ctx(ctx),
      m_randomState(0),
      m_mouseLeftHold(false),
      m_jumpPressed(false)
{
    EventManager* manager = m_ctx->Get<EventManager>();
    manager->AddEventCallback(SceneType::Play, "Mouse_Move", &DesktopPet::_OnMouseMove, this);
    manager->AddEventCallback(SceneType::Play, "Mouse_Left_Pressed", &DesktopPet::_OnMouseLeftPressed, this);
    manager->AddEventCallback(SceneType::Play, "Mouse_Left_Release", &DesktopPet::_OnMouseLeftRelease, this);
    manager->AddEventCallback(SceneType::Play, "Move_Left", &DesktopPet::_OnMoveLeft, this);
    manager->AddEventCallback(SceneType::Play, "Move_Right", &DesktopPet::_OnMoveRight, this);
    manager->AddEventCallback(SceneType::Play, "Look_Up", &DesktopPet::_OnLookUp, this);
    manager->AddEventCallback(SceneType::Play, "Look_Up_End", &DesktopPet::_OnLookUpEnd, this);
    manager->AddEventCallback(SceneType::Play, "Look_Down", &DesktopPet::_OnLookDown, this);
    manager->AddEventCallback(SceneType::Play, "Look_Down_End", &DesktopPet::_OnLookDownEnd, this);
    manager->AddEventCallback(SceneType::Play, "Attack", &DesktopPet::_OnAttack, this);
    manager->AddEventCallback(SceneType::Play, "AttackEnd", &DesktopPet::_OnAttackEnd, this);
    manager->AddEventCallback(SceneType::Play, "Dash", &DesktopPet::_OnDash, this);
    manager->AddEventCallback(SceneType::Play, "Jump", &DesktopPet::_OnJump, this);
    manager->AddEventCallback(SceneType::Play, "JumpEnd", &DesktopPet::_OnJumpEnd, this);
    manager->AddEventCallback(SceneType::Play, "SuperDash", &DesktopPet::_OnSuperDash, this);
    manager->AddEventCallback(SceneType::Play, "SuperDashEnd", &DesktopPet::_OnSuperDashEnd, this);
    manager->AddEventCallback(SceneType::Play, "LookMap", &DesktopPet::_OnLookMap, this);

    m_windowSize = sf::Vector2f(m_ctx->Get<Window>()->GetSize());

    m_rigidbody.m_maxVelocity.y = 700;
    m_rigidbody.m_friction.x = 50;
    m_rigidbody.m_friction.y = 50;
}

DesktopPet::~DesktopPet()
{
    EventManager* manager = m_ctx->Get<EventManager>();
    manager->DelEventCallback(SceneType::Play, "Mouse_Move");
    manager->DelEventCallback(SceneType::Play, "Mouse_Left_Pressed");
    manager->DelEventCallback(SceneType::Play, "Mouse_Left_Release");
    manager->DelEventCallback(SceneType::Play, "Move_Left");
    manager->DelEventCallback(SceneType::Play, "Move_Right");
    manager->DelEventCallback(SceneType::Play, "Look_Up");
    manager->DelEventCallback(SceneType::Play, "Look_Up_End");
    manager->DelEventCallback(SceneType::Play, "Look_Down");
    manager->DelEventCallback(SceneType::Play, "Look_Down_End");
    manager->DelEventCallback(SceneType::Play, "Attack");
    manager->DelEventCallback(SceneType::Play, "AttackEnd");
    manager->DelEventCallback(SceneType::Play, "Dash");
    manager->DelEventCallback(SceneType::Play, "Jump");
    manager->DelEventCallback(SceneType::Play, "JumpEnd");
    manager->DelEventCallback(SceneType::Play, "SuperDash");
    manager->DelEventCallback(SceneType::Play, "SuperDashEnd");
    manager->DelEventCallback(SceneType::Play, "LookMap");
}

bool DesktopPet::Init()
{
    m_sheetStorage.SetTextureManager(m_ctx->Get<TextureManager>());
    if (m_sheetStorage.LoadAtlasPackageFromXml("Resource/Knight copy.xml") == false)
        return false;
    if (m_sheetStorage.LoadAtlasPackageFromXml("Resource/DustEffects.xml") == false)
        return false;
    return true;
}

void DesktopPet::Update(const sf::Time& elapsed)
{
    m_hitLeftWall = false;
    m_hitRightWall = false;
    Window* window = m_ctx->Get<Window>();
    auto windowSize = window->GetSize();
    auto movedPosition = m_rigidbody.m_velocity * (elapsed.asMicroseconds() / 1000000.f);
    m_position += movedPosition;

    auto curSize = m_curState->GetCurSize();
    if (m_position.x < curSize.width / 2)
    {
        m_position.x = curSize.width / 2;
    }
    if (m_position.x == curSize.width / 2.f)
        m_hitLeftWall = true;

    if (m_position.x > (windowSize.x - curSize.width / 2))
    {
        m_position.x = windowSize.x - curSize.width / 2;
    }
    if (m_position.x == windowSize.x - curSize.width / 2.f)
        m_hitRightWall = true;

    if (m_position.y < curSize.height / 2)
        m_position.y = curSize.height / 2;

    if (m_position.y > m_jumpBoard)
        m_position.y = m_jumpBoard;

    UpdateRandomState(elapsed);
    StateMachine::Update(elapsed);
}

void DesktopPet::AdjustJumpBoard()
{
    auto curSize = m_curState->GetCurSize();
    m_jumpBoard = m_position.y;
}

void DesktopPet::UpdateRandomState(const sf::Time& elapsed)
{
    if (m_randomState == 0)
        return;

    bool isEnd = false;
    m_randomElapsed += elapsed;
    switch (m_randomState)
    {
    case 1: // LookUp
    case 2: // LookDown
    {
        if (m_randomElapsed <= sf::seconds(3.0f))
        {
            if (m_randomState == 1)
                _OnLookUp(nullptr);
            else if (m_randomState == 2)
                _OnLookDown(nullptr);
        }
        else
        {
            isEnd = true;
        }
        break;
    }
    case 3: // Focus
    {
        if (m_randomElapsed >= sf::seconds(5.0f))
        {
            isEnd = true;
        }
        break;
    }
    case 4: // LookMap
    {
        if (m_randomElapsed <= sf::seconds(5.0f))
        {
            if (m_randomState == 4)
            {
                _OnLookMap(nullptr);
            }
        }
        else
        {
            isEnd = true;
        }
        break;
    }
    }

    if (isEnd)
    {
        m_randomState = 0;
        m_randomElapsed = sf::seconds(0.f);
    }
}

void DesktopPet::ChangeToRandomState()
{
    m_randomState = rand() % 5;
    auto direction = rand() % 2;
    if (direction == 0)
        m_curState->SetDirection(true);
    else if (direction == 1)
        m_curState->SetDirection(false);

    switch (m_randomState)
    {
    case 3:
    {
        ChangeToState<Knight::Focus>();
        break;
    }
    case 4:
    {
        ChangeToState<Knight::LookMap>();
        break;
    }
    }
}

bool DesktopPet::IsRandomStateEnd()
{
    if (m_randomState == 0)
        return true;
    return false;
}

void DesktopPet::_OnMouseMove(EventDetail* detail)
{
    if (m_mouseLeftHold)
    {
        sf::Vector2f posMove = detail->m_mousePos - m_prevMousePos;
        m_prevMousePos = detail->m_mousePos;

        m_position += posMove;
        AdjustJumpBoard();
    }
}

void DesktopPet::_OnMouseLeftPressed(EventDetail* detail)
{
    auto rect = m_curState->GetCurSize();
    if (rect.contains(detail->m_mousePos))
    {
        m_prevMousePos = detail->m_mousePos;
        m_mouseLeftHold = true;
    }
}

void DesktopPet::_OnMouseLeftRelease(EventDetail* detail) { m_mouseLeftHold = false; }
void DesktopPet::_OnMoveLeft(EventDetail* detail) { m_curState->OnMoveLeft(); }
void DesktopPet::_OnMoveRight(EventDetail* detail) { m_curState->OnMoveRight(); }
void DesktopPet::_OnLookUp(EventDetail* detail) { m_curState->OnLookUp(); }
void DesktopPet::_OnLookUpEnd(EventDetail* detail) { m_curState->OnLookUpEnd(); }
void DesktopPet::_OnLookDown(EventDetail* detail) { m_curState->OnLookDown(); }
void DesktopPet::_OnLookDownEnd(EventDetail* detail) { m_curState->OnLookDownEnd(); }
void DesktopPet::_OnAttack(EventDetail* detail)
{
    if (m_attackPressed)
        return;
    m_attackPressed = true;
    m_curState->OnAttack();
}
void DesktopPet::_OnAttackEnd(EventDetail* detail) { m_attackPressed = false; }
void DesktopPet::_OnDash(EventDetail* detail) { m_curState->OnDash(); }

void DesktopPet::_OnJump(EventDetail* detail)
{
    if (m_jumpPressed)
        return;
    m_jumpPressed = true;
    m_curState->OnJump();
}

void DesktopPet::_OnJumpEnd(EventDetail* detail)
{
    m_jumpPressed = false;
    m_curState->OnJumpEnd();
}

void DesktopPet::_OnSuperDash(EventDetail* detail) { m_curState->OnSuperDash(); }
void DesktopPet::_OnSuperDashEnd(EventDetail* detail) { m_curState->OnSuperDashEnd(); }
void DesktopPet::_OnLookMap(EventDetail* detail) { m_curState->OnLookMap(); }
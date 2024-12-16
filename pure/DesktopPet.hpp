#pragma once

#include "SharedContext.hpp"
#include "state/State.hpp"

class EventDetail;

class DesktopPet : public StateMachine
{
public:
    DesktopPet(SharedContext* ctx);
    ~DesktopPet();

    bool Init();
    virtual void Update(const sf::Time& elapsed) override;

    virtual void AdjustJumpBoard() override;

    void UpdateRandomState(const sf::Time& elapsed);
    void ChangeToRandomState();
    bool IsRandomStateEnd();

private:
    void _OnMouseMove(EventDetail* detail);
    void _OnMouseLeftPressed(EventDetail* detail);
    void _OnMouseLeftRelease(EventDetail* detail);
    void _OnMoveLeft(EventDetail* detail);
    void _OnMoveRight(EventDetail* detail);
    void _OnLookUp(EventDetail* detail);
    void _OnLookUpEnd(EventDetail* detail);
    void _OnLookDown(EventDetail* detail);
    void _OnLookDownEnd(EventDetail* detail);
    void _OnAttack(EventDetail* detail);
    void _OnAttackEnd(EventDetail* detail);
    void _OnDash(EventDetail* detail);
    void _OnJump(EventDetail* detail);
    void _OnJumpEnd(EventDetail* detail);
    void _OnSuperDash(EventDetail* detail);
    void _OnSuperDashEnd(EventDetail* detail);
    void _OnLookMap(EventDetail* detail);

private:
    SharedContext* m_ctx;
    sf::Time m_randomElapsed;
    int32_t m_randomState;
    bool m_mouseLeftHold;
    bool m_jumpPressed;
    bool m_attackPressed;
    sf::Vector2f m_prevMousePos;
};

#include "ScenePlay.hpp"

#include "../DesktopPet.hpp"
#include "../EventManager.hpp"
#include "../SharedContext.hpp"
#include "../Window.hpp"
#include "../state/KnightState.hpp"
#include "SceneManager.hpp"

ScenePlay::ScenePlay()
    : m_pet(nullptr)
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate(SceneManager* manager)
{
    if (Scene::OnCreate(manager) == false)
        return false;

    m_windowSize = m_sceneManager->GetSharedContext()->Get<Window>()->GetSize();

    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->AddEventCallback(SceneType::Play, "Play_Pause", &ScenePlay::_Pause, this);

    m_pet = new DesktopPet(manager->GetSharedContext());
    if (m_pet->Init() == false)
        return false;

    m_pet->ChangeToState<Knight::Prostrate>();
    m_pet->SetPosition(sf::Vector2f(m_windowSize.x / 2.f, m_windowSize.y / 2.f), true);

    return true;
}

void ScenePlay::OnDestroy()
{
    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->DelEventCallback(SceneType::Play, "Play_Pause");
}

void ScenePlay::Update(const sf::Time& elapsed)
{
    // Check Window Focus
    Window* window = m_sceneManager->GetSharedContext()->Get<Window>();
    if (!window->IsFocus())
    {
        m_lostElapsed += elapsed;
        if (m_lostElapsed >= sf::seconds(8.0f) && m_pet->IsRandomStateEnd())
        {
            m_lostElapsed = sf::seconds(0);
            m_pet->ChangeToRandomState();
        }
    }

    m_pet->Update(elapsed);
}

void ScenePlay::Render(Window* window) { m_pet->Render(window); }

void ScenePlay::OnEnter() {}

void ScenePlay::OnLeave() {}

void ScenePlay::_Pause(EventDetail* detail) { m_sceneManager->ChangeSceneTo(SceneType::Paused); }
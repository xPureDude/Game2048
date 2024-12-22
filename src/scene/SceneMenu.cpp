#include "SceneMenu.hpp"

SceneMenu::SceneMenu()
    : Scene()
{
}

SceneMenu::~SceneMenu()
{
}

bool SceneMenu::OnCreate(SceneManager* manager)
{
    if (!Scene::OnCreate(manager))
        return false;
    return true;
}

void SceneMenu::OnDestroy()
{
}

void SceneMenu::Update(const sf::Time& elasped)
{
}

void SceneMenu::Render(Window* window)
{
}

void SceneMenu::OnEnter()
{
}

void SceneMenu::OnLeave()
{
}
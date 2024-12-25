#pragma once

#include "SceneManager.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "SceneDependent.hpp"

SceneManager::SceneManager(SharedContext* ctx)
    : m_ctx(ctx)
{
}

SceneManager::~SceneManager()
{
    for (auto& item : m_scenes)
    {
        item.m_scene->OnDestroy();
    }
    m_scenes.clear();
}

bool SceneManager::Init(SharedContext* context)
{
    if (context == nullptr)
        return false;

    m_ctx = context;

    return true;
}

void SceneManager::Update(const sf::Time& elapsed)
{
    if (m_scenes.empty() == true)
        return;
    if (m_scenes.back().m_scene->IsUpdateTransparent() == true && m_scenes.size() > 1)
    {
        auto iter = m_scenes.end();
        --iter;
        while (iter != m_scenes.begin())
        {
            if (iter->m_scene->IsUpdateTransparent() == false && _IsInRemoveLater(iter->m_type) == false)
            {
                break;
            }
            --iter;
        }

        while (iter != m_scenes.end())
        {
            if (_IsInRemoveLater(iter->m_type) == false)
                iter->m_scene->Update(elapsed);
            ++iter;
        }
    }
    else
    {
        m_scenes.back().m_scene->Update(elapsed);
    }
}

void SceneManager::Render()
{
    if (m_scenes.empty() == true)
        return;

    auto window = m_ctx->Get<Window>();
    if (m_scenes.back().m_scene->IsRenderTransparent() == true && m_scenes.size() > 1)
    {
        auto iter = m_scenes.end();
        --iter;
        while (iter != m_scenes.begin())
        {
            if (iter->m_scene->IsRenderTransparent() == false && _IsInRemoveLater(iter->m_type) == false)
            {
                break;
            }
            --iter;
        }

        while (iter != m_scenes.end())
        {
            if (_IsInRemoveLater(iter->m_type) == false)
            {
                window->SetView(iter->m_scene->GetView());
                iter->m_scene->Render(window);
            }
            ++iter;
        }
    }
    else
    {
        m_scenes.back().m_scene->Render(window);
        window->SetView(m_scenes.back().m_scene->GetView());
    }
}

void SceneManager::ChangeScene(SceneType type)
{
    if (m_scenes.back().m_type == type)
        return;

    PopScene();
    PushScene(type);
}

void SceneManager::PushScene(SceneType type)
{
    auto iter = std::find(m_scenes.begin(), m_scenes.end(), SceneInfo({.m_type = type, .m_scene = nullptr}));
    if (iter != m_scenes.end())
    {
        m_scenes.back().m_scene->OnLeave();
        auto tempScene = iter->m_scene;
        m_scenes.erase(iter);
        m_scenes.emplace_back(type, tempScene);
    }
    else
    {
        if (_CreateScene(type) != true)
        {
            return;
        }
    }
    m_scenes.back().m_scene->OnEnter();

    // Inform SceneType related Manager
    SceneDependent::ChangeCurrentSceneType(type);
    m_ctx->Get<Window>()->SetView(m_scenes.back().m_scene->GetView());
}

void SceneManager::PopScene()
{
    if (m_scenes.empty())
        return;
    m_removeLater.insert(m_scenes.back().m_type);
    m_scenes.pop_back();
}

void SceneManager::ProcessRemoves()
{
    for (auto item : m_removeLater)
    {
        _DestroyScene(item);
    }
    m_removeLater.clear();
}

bool SceneManager::_CreateScene(SceneType type)
{
    std::shared_ptr<Scene> scene = m_factory.CreateScene(type);
    if (scene == nullptr)
    {
        std::cout << "SceneManager::_CreateScene failed, Type " << static_cast<int>(type) << " not found" << std::endl;
        return false;
    }

    if (scene->OnCreate(this) == false)
        return false;
    m_scenes.emplace_back(type, scene);
    return true;
}

void SceneManager::_DestroyScene(SceneType type)
{
    for (auto iter = m_scenes.begin(); iter != m_scenes.end(); ++iter)
    {
        if (iter->m_type == type)
        {
            iter->m_scene->OnDestroy();
            m_scenes.erase(iter);
            return;
        }
    }
}

bool SceneManager::_IsInRemoveLater(SceneType type)
{
    return m_removeLater.find(type) != m_removeLater.end();
}

#pragma once

#include "SceneManager.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../event/EventManager.hpp"
#include "ScenePlay.hpp"

#include <iostream>

SceneManager::SceneManager(SharedContext* ctx)
    : m_ctx(ctx)
{
    _RegisterScene<ScenePlay>(SceneType::Play);
}

SceneManager::~SceneManager()
{
    for (auto& item : m_scenes)
    {
        if (item.m_scene != nullptr)
        {
            item.m_scene->OnDestroy();
            delete item.m_scene;
            item.m_scene = nullptr;
        }
    }
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

void SceneManager::ChangeSceneTo(SceneType type)
{
    auto iter = m_scenes.begin();
    while (iter != m_scenes.end())
    {
        if (iter->m_type == type)
            break;
        ++iter;
    }
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
    m_ctx->Get<EventManager>()->SetCurrentSceneType(type);
    m_ctx->Get<Window>()->SetView(m_scenes.back().m_scene->GetView());
}

void SceneManager::RemoveScene(SceneType type)
{
    m_removeLater.insert(type);
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
    if (m_factory.find(type) == m_factory.end())
    {
        std::cout << "SceneManager::_CreateScene failed, Type " << static_cast<int>(type) << " not found" << std::endl;
        return false;
    }

    Scene* scene = m_factory[type]();
    if (scene->OnCreate(this) == false)
        return false;
    m_scenes.emplace_back(type, scene);
    m_ctx->Get<EventManager>()->SetCurrentSceneType(type);
    return true;
}

void SceneManager::_DestroyScene(SceneType type)
{
    for (auto iter = m_scenes.begin(); iter != m_scenes.end(); ++iter)
    {
        if (iter->m_type == type)
        {
            iter->m_scene->OnDestroy();
            delete iter->m_scene;
            m_scenes.erase(iter);
            return;
        }
    }
}

bool SceneManager::_IsInRemoveLater(SceneType type)
{
    return m_removeLater.find(type) != m_removeLater.end();
}

#pragma once

#include "SceneManager.hpp"

#include "Scene.hpp"
#include "SceneFactory.hpp"
#include "common/Log.hpp"
#include "core/SharedContext.hpp"
#include "core/Window.hpp"

SceneManager::~SceneManager()
{
    for (auto& item : m_sceneMap)
    {
        item.second->OnDestroy();
    }
    m_sceneVec.clear();
    m_sceneMap.clear();
}

void SceneManager::UpdateFrame(const sf::Time& elapsed)
{
    if (m_sceneVec.empty() == true)
        return;
    auto curScene = m_sceneMap[m_sceneVec.back()];
    if (curScene->IsUpdateTransparent() == true && m_sceneVec.size() > 1)
    {
        auto iter = m_sceneVec.end();
        --iter;
        while (iter != m_sceneVec.begin())
        {
            auto scene = m_sceneMap[*iter];
            if (scene->IsUpdateTransparent() == false && _IsInRemoveLater(*iter) == false)
            {
                break;
            }
            --iter;
        }

        while (iter != m_sceneVec.end())
        {
            auto scene = m_sceneMap[*iter];
            if (_IsInRemoveLater(*iter) == false)
                scene->Update(elapsed);
            ++iter;
        }
    }
    else
    {
        curScene->Update(elapsed);
    }
}

void SceneManager::Render()
{
    if (m_sceneVec.empty() == true)
        return;

    auto window = SharedContext::Instance()->Get<Window>();
    auto curScene = m_sceneMap[m_sceneVec.back()];
    if (curScene->IsRenderTransparent() == true && m_sceneVec.size() > 1)
    {
        auto iter = m_sceneVec.end();
        --iter;
        while (iter != m_sceneVec.begin())
        {
            auto scene = m_sceneMap[*iter];
            if (scene->IsRenderTransparent() == false && _IsInRemoveLater(*iter) == false)
            {
                break;
            }
            --iter;
        }

        while (iter != m_sceneVec.end())
        {
            auto scene = m_sceneMap[*iter];
            if (_IsInRemoveLater(*iter) == false)
            {
                window->SetView(scene->GetView());
                scene->Render(window);
            }
            ++iter;
        }
    }
    else
    {
        window->SetView(curScene->GetView());
        curScene->Render(window);
    }
}

void SceneManager::ChangeScene(SceneType sceneType, const std::any& param)
{
    if (m_sceneVec.back() == sceneType)
        return;

    PopScene();
    PushScene(sceneType, param);
}

void SceneManager::PushScene(SceneType sceneType, const std::any& param)
{
    if (m_sceneMap.contains(sceneType))
    {
        m_sceneVec.erase(std::find(m_sceneVec.begin(), m_sceneVec.end(), sceneType));
        m_sceneVec.push_back(sceneType);
    }
    else
    {
        if (_CreateScene(sceneType) != true)
        {
            return;
        }
    }
    auto scene = m_sceneMap[sceneType];
    scene->OnEnter(param);
    DBG("SceneManager::PushScene, SceneType: {} OnEnter", SceneType2String(sceneType));

    // Inform SceneType related Manager
    SharedContext::Instance()->Get<Window>()->SetView(scene->GetView());
}

void SceneManager::PopScene()
{
    if (m_sceneVec.empty())
        return;
    auto lastScene = m_sceneMap[m_sceneVec.back()];
    lastScene->OnLeave();

    DBG("SceneManager::PopScene, SceneType: {} OnLeave", SceneType2String(m_sceneVec.back()));
    m_removeLater.insert(m_sceneVec.back());
}

void SceneManager::ProcessRemoves()
{
    for (auto item : m_removeLater)
    {
        _DestroyScene(item);
    }
    m_removeLater.clear();
}

bool SceneManager::_CreateScene(SceneType sceneType)
{
    std::shared_ptr<Scene> scene = SceneFactory::CreateScene(sceneType);
    if (scene == nullptr)
    {
        DBG("SceneManager::_CreateScene failed, Type {} not found", SceneType2String(sceneType));
        return false;
    }

    if (scene->OnCreate() == false)
    {
        scene->OnDestroy();
        return false;
    }
    m_sceneVec.push_back(sceneType);

    m_sceneMap.emplace(sceneType, scene);

    return true;
}

void SceneManager::_DestroyScene(SceneType sceneType)
{
    if (m_sceneMap.contains(sceneType))
    {
        m_sceneMap[sceneType]->OnDestroy();
        m_sceneMap.erase(sceneType);
        m_sceneVec.erase(std::find(m_sceneVec.begin(), m_sceneVec.end(), sceneType));
    }
}

bool SceneManager::_IsInRemoveLater(SceneType sceneType)
{
    return m_removeLater.find(sceneType) != m_removeLater.end();
}

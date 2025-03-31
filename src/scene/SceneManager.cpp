#pragma once

#include "SceneManager.hpp"

#include "pch.hpp" // IWYU pragma: keep

#include "SceneDependent.hpp"
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

void SceneManager::Update(const sf::Time& elapsed)
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

void SceneManager::ChangeScene(SceneType type, const std::any& param)
{
    if (m_sceneVec.back() == type)
        return;

    PopScene();
    PushScene(type, param);
}

void SceneManager::PushScene(SceneType type, const std::any& param)
{
    if (m_sceneMap.contains(type))
    {
        m_sceneVec.erase(std::find(m_sceneVec.begin(), m_sceneVec.end(), type));
        m_sceneVec.emplace_back(type);
    }
    else
    {
        if (_CreateScene(type) != true)
        {
            return;
        }
    }
    auto scene = m_sceneMap[type];
    scene->OnEnter(param);
    DBG("SceneManager::PushScene, SceneType: {} OnEnter", TranslateSceneTypeToStringView(type));

    // Inform SceneType related Manager
    SceneDependent::ChangeCurrentSceneType(type);
    SharedContext::Instance()->Get<Window>()->SetView(scene->GetView());
}

void SceneManager::PopScene()
{
    if (m_sceneVec.empty())
        return;
    auto lastScene = m_sceneMap[m_sceneVec.back()];
    lastScene->OnLeave();

    DBG("SceneManager::PopScene, SceneType: {} OnLeave", TranslateSceneTypeToStringView(m_sceneVec.back()));
    m_removeLater.insert(m_sceneVec.back());

    if (m_sceneVec.size() < 2)
        return;
    SceneDependent::ChangeCurrentSceneType(m_sceneVec[m_sceneVec.size() - 2]);
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
    std::shared_ptr<Scene> scene = SceneFactory::CreateScene(type);
    if (scene == nullptr)
    {
        std::cout << "SceneManager::_CreateScene failed, Type " << static_cast<int>(type) << " not found" << std::endl;
        return false;
    }

    if (scene->OnCreate() == false)
    {
        scene->OnDestroy();
        return false;
    }
    m_sceneVec.push_back(type);
    m_sceneMap.emplace(type, scene);

    return true;
}

void SceneManager::_DestroyScene(SceneType type)
{
    if (m_sceneMap.contains(type))
    {
        m_sceneMap[type]->OnDestroy();
        m_sceneMap.erase(type);
        m_sceneVec.erase(std::find(m_sceneVec.begin(), m_sceneVec.end(), type));
    }
}

bool SceneManager::_IsInRemoveLater(SceneType type)
{
    return m_removeLater.find(type) != m_removeLater.end();
}

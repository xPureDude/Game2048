#ifndef SCENEDEPENDENT_HPP
#define SCENEDEPENDENT_HPP

#include "../pch.hpp" // IWYU pragma: keep
#include "Scene.hpp"

class SceneDependent
{
public:
    SceneDependent();
    virtual ~SceneDependent();

    static void ChangeCurrentSceneType(SceneType newType);

protected:
    static SceneType s_curSceneType;

private:
    static std::vector<SceneDependent*> s_instances;
};

#endif // BASEMANAGER_HPP
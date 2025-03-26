#ifndef SCENEDEPENDENT_HPP
#define SCENEDEPENDENT_HPP

#include "Scene.hpp"

class SceneDependent
{
public:
    SceneDependent();
    virtual ~SceneDependent();

    static void ChangeCurrentSceneType(SceneType newType);

protected:
    static SceneType s_curSceneType;

    virtual void _OnCurrentSceneChange() {};

private:
    static std::vector<SceneDependent*> s_instances;
};

#endif // BASEMANAGER_HPP
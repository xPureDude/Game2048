#pragma once

#include "gui/GuiLayer.hpp"

class SceneSettingLayer : public gui::Layer
{
public:
    SceneSettingLayer();
    virtual ~SceneSettingLayer();

private:
    void _OnLanguageChanged(const std::string& lang);
    void _OnBackToMainMenu();
};
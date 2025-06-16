#pragma once

#include "gui/GuiLayer.hpp"

class ScenePlayLayer : public gui::Layer
{
public:
    ScenePlayLayer();

private:
    void _InitLayer();

    void _OnScoreChange(evt::Base* event);

private:
    std::shared_ptr<gui::Element> m_scoreLabel;
};
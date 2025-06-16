#pragma once

#include "gui/GuiLayer.hpp"

class SceneGameOverLayer : public gui::Layer
{
public:
    SceneGameOverLayer();

private:
    void _InitLayer();

    void _OnGameOver(evt::Base* event);
    void _OnFirstButtonClick();

private:
    bool m_isWin;
    std::shared_ptr<gui::Element> m_tipText;
    std::shared_ptr<gui::Element> m_firstButton;
};
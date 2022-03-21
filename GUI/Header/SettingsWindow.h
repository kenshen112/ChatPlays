#pragma once
#include <iostream>

#include "Window.h"
//#include "Settings.h"


class SettingsWindow
{

private:
    Window window;
    //Settings settings;

    bool TwitchWindow;
    bool ControllerWindow;

    void XinputSettings();
    void EvdevSettings();
public:

    void DrawMainSettings();
    void DrawTwitchSettings();
    void DrawControllerSettings();
};
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
public:

    void DrawMainSettings();
    void DrawTwitchSettings();
    void DrawControllerSettings();
};
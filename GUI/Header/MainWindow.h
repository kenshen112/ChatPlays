#pragma once
#include <iostream>
#include "Window.h"
#include "SettingsWindow.h"
#include "TwitchWindow.h"

class MainWindow
{
    private:
    bool isOpen;
    Window* window;
    SettingsWindow* sWindow;
    TwitchWindow* tWindow;
    public:

    void Open();
    void Close();
    void Update();
};
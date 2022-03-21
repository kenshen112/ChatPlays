#include <iostream>
#include "Window.h"
#include "SettingsWindow.h"

class MainWindow
{
    private:
    Window* window;
    SettingsWindow* sWindow;

    bool twitch;
    bool settings;

    public:
    void Open();
    void DrawMainWindow();
    // This would be best for an OSD? Or, would this be better for the bot UI. This would be either debug info and chat/
    // Or, this would be the hijacked OSD context
    void DrawTwitch();
    void DrawPine();
    void Update();
};
#include "MainWindow.h"


void MainWindow::Open()
{
    window = new Window();
    window->CreateWindowGlContext("ChatPlays", 1920, 1080);

    ImGui::Begin("Main");
    window->Button("Twitch", twitch);

    window->Button("Settings", settings);
    ImGui::End();
}


void MainWindow::Update()
{
    
}
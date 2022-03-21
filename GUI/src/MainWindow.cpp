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

void MainWindow::DrawTwitch()
{
    ImGui::Begin("Twitch");
    // Something about connection stats and chatroom
    ImGui::End();
}

void MainWindow::DrawPine()
{
    ImGui::Begin("Pine");
    // Stats about Pine and game running
    ImGui::End();
}


void MainWindow::Update()
{
    
}
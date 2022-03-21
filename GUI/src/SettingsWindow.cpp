#include "SettingsWindow.h"

void SettingsWindow::DrawMainSettings()
{
    if (ImGui::Begin("Settings"))
    {
        if (ImGui::BeginCombo("", ""))
        {
            window.Button("Twitch", TwitchWindow);
            ImGui::EndCombo();
        }

        ImGui::End();
    }
}

void SettingsWindow::DrawTwitchSettings()
{
    ImGui::Begin("Twitch Settings");
    // Settings about UserName, channel to join and ouath
    // Settings about recognized commands and functions to run
    ImGui::End();
}

void SettingsWindow::DrawControllerSettings()
{
    ImGui::Begin("ControllerSettings");
    // Really should differentiate between linux and windows.
    // Windows: PlayerNumber, controller type etc
    // Linux: Controller Mapper and all fields and settings associated
    ImGui::End();
}
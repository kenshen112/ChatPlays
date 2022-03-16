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
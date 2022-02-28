#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include "settings.h"
#include "message.h"
#include "twitch.h"

#include "Window.h"

static Message* queue;
static std::vector<std::thread*> threadPool;

static TwitchInfo twitchSettings;
Emit controller;

void twitch()
{
    std::thread th(&Twitch::StartTwitchThread, Twitch(), queue, &twitchSettings, &controller);
    th.join();
}

void ManualControl()
{
    controller.AttachController();
    // Draw a GUI that let's buttons be pressed. A proto TAS gui so to speak
}

void StartBot()
{
    threadPool.push_back(new std::thread(&Twitch::StartTwitchThread, Twitch(), queue, &twitchSettings, &controller));
    threadPool[0]->join();
}

int main()
{
    //queue = new Message();
    //Settings* settings = new Settings(controller, twitchSettings);

    Window* win = new Window();
    win->StartFunc = StartBot;
    win->ManualCtrl = ManualControl;
    if (win->CreateWindowGlContext("Main", 1920, 1080))
    {
        ImGui::ShowDemoWindow();
        //win->Update();

        if (win->QuitFunc)
        {
            threadPool[0]->detach();
            threadPool.clear();
            controller.DisconnectController();
        }
    }

    return 0;
}
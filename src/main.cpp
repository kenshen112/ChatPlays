#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include "message.h"
#include "twitch.h"
#include "Controller.h"

static Message* queue;
static std::vector<std::thread*> threadPool;

static TwitchInfo twitchSettings;
Controller *controller;

void twitch()
{
    std::thread th(&Twitch::StartTwitchThread, Twitch(), queue, &twitchSettings, &controller);
    th.join();
}

void ManualControl()
{
    controller->AttachController();
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

    /*win->StartFunc = StartBot;
    win->ManualCtrl = ManualControl;
    if (win->CreateWindowGlContext("Main", 1920, 1080))
    {
        win->Update();

        if (win->QuitFunc)
        {
            threadPool[0]->detach();
            threadPool.clear();
            controller.DisconnectController();
        }
    }*/

    return 0;
}
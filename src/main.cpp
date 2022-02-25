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

void manualControl()
{
    controller.CreateController(true);
}

void startBot()
{
    threadPool.push_back(new std::thread(&Twitch::StartTwitchThread, Twitch(), queue, &twitchSettings, &controller));
    threadPool[0]->join();
}

int main()
{
    //queue = new Message();
    //Settings* settings = new Settings(controller, twitchSettings);

    controller.InitalConfig();

    Window* win = new Window();

    if (win->CreateWindowGlContext("Main", 1920, 1080))
    {
        win->Update();
    }

    return 0;
}
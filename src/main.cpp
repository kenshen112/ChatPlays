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
    std::string com;
    while (true)
    {
        std::cout << "Enter command: ";
        std::cin >> com;

        if (com == "A")
        {
            controller.SendInput(Buttons::A, ABS::CLEAR, 0, 100);
        }

        if (com == "B")
        {
            controller.SendInput(Buttons::B, ABS::CLEAR, 0, 100);
        }

        if (com == "Up")
        {
            controller.SendInput(Buttons::B_CLEAR, ABS::L_Y_AXIS, 32768, 100);
        }

        if (com == "Down")
        {
            controller.SendInput(Buttons::B_CLEAR, ABS::L_Y_AXIS, -32768, 100);
        }
    }
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
        win->Update();

        if (win->QuitFunc)
        {
            threadPool[0]->detach();
            threadPool.clear();
        }
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include "twitch.h"
#include "message.h"
#include "Controller.h"
#include "MainWindow.h"
#include "SettingsWindow.h"


static Message* queue;
static std::vector<std::thread*> threadPool;

static TwitchInfo twitchSettings;
Controller *controller;

void twitch()
{
    std::thread th(&Twitch::StartTwitchThread, Twitch(), queue, &twitchSettings);
    th.join();
}

void ManualControl()
{
    controller->AttachController();
    // Draw a GUI that let's buttons be pressed. A proto TAS gui so to speak
}

void StartBot()
{
    threadPool.push_back(new std::thread(&Twitch::StartTwitchThread, Twitch(), queue, &twitchSettings));
    threadPool[0]->join();
}

int main()
{
    queue = new Message();

    SettingsWindow* s;
    MainWindow* window;

    char command;

    std::cout << "Options: "
    << "S: Settings Window"
    << "T: Twitch Window"
    << "C: Controller Window"
    << std::endl;

    std::cout << "> ";
    std::cin >> command;

    switch(std::toupper(command))
    {
        case 'S':
            s = new SettingsWindow();
            s->DrawMainSettings();
            break;

        case 'T':
            break;

        case 'C':
            break;
    }

    return 0;
}
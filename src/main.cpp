#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include "settings.h"
#include "message.h"
#include "twitch.h"

static Message* queue;
static std::vector<std::thread*> threadPool;

static TwitchInfo* twitchSettings;
static Emit* controller;

void twitch()
{
    std::thread th(&Twitch::create, queue, twitchSettings);
    th.join();
}

void manualControl()
{
    std::thread th(&Emit::poll, Emit(), queue, true);
    th.join();
}

void startBot()
{
    threadPool.push_back(new std::thread(&Twitch::create, queue, twitchSettings));
    threadPool.push_back(new std::thread(&Emit::poll, Emit(), queue, false));
    threadPool[0]->join();
}

int main()
{
    twitchSettings = new TwitchInfo();
    queue = new Message();
    Settings* settings = new Settings(controller, twitchSettings);
    bool isActive = true;
    char command;

    std::cout << "Avalible Commands: " << std::endl
    << "t: Test Twitch" << std::endl
    << "c: Manually control bot" << std::endl
    << "s: Start Bot in chat play mode" << std::endl
    << "e: Edit Settings" << std::endl;

    while(isActive)
    {
        std::cout << "Enter Command: ";
        std::cin >> command;

        switch (command)
        {
            case 't':
                twitch();
                break;

            case 'c':
                manualControl();
                break;

            case 's':
                startBot();
                break;

            case 'e':
                settings->init();
                break;

            default:
                std::cerr << "Err: Unrecognized Command" << std::endl;
                break;
        }
    }
    return 0;
}
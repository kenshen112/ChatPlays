#pragma once
#include <iostream>
#include <stdio.h>
#ifdef __linux__ 
#include "Linux/connect.h"
#elif _WIN32
#include "Windows/winConnect.h"
#endif

#include "settings.h"

#include "message.h"

using json = nlohmann::json;

struct TwitchInfo
{
    std::string userName;
    std::string oauthToken;
    std::string channelName;

    TwitchInfo() = default;
    TwitchInfo(TwitchInfoSettings);
};

class Twitch
{
private:
    std::string buffer;
    std::string address = "irc.chat.twitch.tv";
    std::string pong = "PONG :tmi.twitch.tv\r\n";

    Connect connection;

    Message* queue;
    TwitchInfo settings;

    bool isJoined = false;
public:
    Twitch() = default;
    Twitch(Message* q)
    {
        queue = q;
    }
    bool login(Message* q, TwitchInfo* s);

    void StartTwitchThread(Message* q, TwitchInfo* s);

    bool ParseCommand(std::string command);

    bool update();
    //void exit();
};
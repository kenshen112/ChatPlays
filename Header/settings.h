#pragma once
#include <filesystem>
#include <control.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

struct TwitchInfo
{
    std::string userName;
    std::string oauthToken;
    std::string channelName;


    friend void to_json(nlohmann::json& j, const TwitchInfo& p);
    friend void from_json(const nlohmann::json& j, TwitchInfo& p);
};

struct Xinput
{
    int playerAmount;

};

struct PineSettings
{
    int portNumber;
    // Note this is probably going to be placed in the Imgui / GUI section. 
    // This is to tell the context grabber what program you want to draw your customized OSD / Twitch notification, Game UI stuffs onto.
    std::string executeableContext;
};


// The overall goal of this class should revolve around parsing of JSON file and passing it to UI or classes
class Settings
{
    private:
    fs::path filePath = "settings/settings.json";
    std::fstream file;
    json j;

    TwitchInfo* twitch;

    public:
    Settings() = default;

    friend std::ostream & operator << (std::ostream &out, const Settings &c);
    friend std::istream & operator >> (std::istream &in, Settings &c);

    void SetUser(std::string& str);
    void SetOauth(std::string& str);
    void SetChannel(std::string& str);

    TwitchInfo* GetTwitch()
    {
        if (twitch != nullptr)
            return twitch;
        else
        {
            throw("Twitch not initalized");
            return nullptr;
        }
    }

    bool Load();
    bool Save();
};
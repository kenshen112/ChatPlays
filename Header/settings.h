#pragma once
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#ifdef __linux__ 
#include "Linux/EvDev.h"
#include "Linux/connect.h"
#elif _WIN32
#include "Windows/winConnect.h"
#include "Windows/Xinput.h"
#endif

#include "twitch.h"
#include "json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

// The overall goal of this class should revolve around parsing of JSON file and passing it to UI or classes
class Settings
{
    private:
    // ToDo: Fix path handling to generate settings folder in the executeable's directory. Missing a gui libary makes finding that a challenge.
    fs::path filePath = "settings/settings.json";
    json j;

    public:
    Settings() = default;

    friend std::ostream & operator << (std::ostream &out, const Settings &c);
    //friend std::istream & operator >> (std::istream &in, Settings &c);

    bool Load();
    bool Save();
};
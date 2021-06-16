#pragma once
#include <map>
#include <queue>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <filesystem>
#include <linux/uinput.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "json.hpp"

enum Buttons {UP, DOWN, LEFT, RIGHT, A, B, X, Y, START, SELECT, L1, R1, L2, R2, L3, R3, EXIT};

using json = nlohmann::json;

namespace fs = std::filesystem;

struct Controller
{
    fs::path eventPath = "/dev/input";
    std::string controllerName;
    std::vector<int>buttonCodes;
    std::vector<input_absinfo*> abs;

    int fd;
    struct libevdev *dev;
};

struct ControlInfo
{
    std::map<std::string, Buttons> commands;

    Controller controller;

    ControlInfo();
    ControlInfo(json j);

    void config();

    json control;
    void save(json &j, bool isDefault = false);
    friend void to_json(nlohmann::json& j, const ControlInfo& p);
    friend void from_json(const nlohmann::json& j, ControlInfo& p);
};


class Control
{
    private:
    std::vector<int>buttonCodes = 
    {
        ABS_X,
        ABS_Y,
        ABS_RX,
        ABS_RY,
        ABS_RZ,
        ABS_Z,
        ABS_HAT0X,
        ABS_HAT0Y,
        BTN_SOUTH,
        BTN_EAST,
        BTN_C,
        BTN_NORTH,
        BTN_WEST,
        BTN_Z,
        BTN_TR,
        BTN_TL
    };
    struct std::vector<input_absinfo*> abs
    {
        // LStick. X, Y
        new input_absinfo
        {
            32768,
            0,
            65535,
            255,
            4095
        },
        new input_absinfo
        {
            32768,
            0,
            65535,
            255,
            4095
        },
        // RStick X, Y
        new input_absinfo
        {
            32768,
            0,
            65535,
            255,
            4095
        },
        new input_absinfo
        {
            32768,
            0,
            65535,
            255,
            4095
        },
        // RZ, Z
        new input_absinfo
        {
            63,
            0,
            1023,
            255,
            63
        },
        new input_absinfo
        {
            63,
            0,
            1023,
            255,
            63
        }
    };

    int fd = 0;
    int maxInput = 0;
    struct libevdev *dev;
    struct uinput_user_dev device;
    struct uinput_setup usetup;
    struct libevdev_uinput *uidev;
    struct input_absinfo* init;
    std::map<std::string, Buttons> commands
    {
        {"UP", UP},
        {"DOWN", DOWN},
        {"LEFT", LEFT},
        {"RIGHT", RIGHT},
        {"A", A},
        {"B", B},
        {"X", X},
        {"Y", Y},
        {"START", START},
        {"SELECT", SELECT},
        {"L1", L1},
        {"R1", R1},
        {"L2", L2},
        {"R2", R2},
        {"L3", L3},
        {"R3", R3},
        {"Exit", EXIT}
    };
    std::queue<Buttons>controlQueue;
    public:
    Buttons GetCommands(std::string key);
    bool CreateController();
    bool emit(Buttons cmd);
    int moveABS(int ABS, int moveAxis, int flat);
    int resetABS(int ABS, int flatAxis);
    int pressBtn(int btn);
    int releaseBtn(int btn);
    bool Close();
};
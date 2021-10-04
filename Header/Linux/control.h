#pragma once
#include <map>
#include <mutex>
#include <queue>
#include <vector>
#include <thread>
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
#include "message.h"

enum Buttons {UP, DOWN, LEFT, RIGHT, RUP, RDOWN, RLEFT, RRIGHT, DUP, DDOWN, DLEFT, DRIGHT, L2, R2, A, B, X, Y, START, SELECT, L1, R1, L3, R3, EXIT};

using json = nlohmann::json;

namespace fs = std::filesystem;


/*************************************************************************************************************************** 
* The actual linking bit between chat and what's being pressed
* All strings represent chat commands. Enum is what the system will parse and determine what to press from there.
***************************************************************************************************************************/
static std::map<Buttons, std::string> controlNames = 
{
    {UP, "UP"},
    {DOWN, "DOWN"},
    {LEFT, "LEFT"},
    {RIGHT, "RIGHT"},
    {RUP, "RStick Up"},
    {RDOWN, "RStick Down"},
    {RLEFT, "RStick Left"},
    {RRIGHT, "RStick Right"},
    {DUP, "Dpad Up"},
    {DDOWN, "Dpad Down"},
    {DLEFT, "Dpad Left"},
    {DRIGHT, "Dpad Right"},
    {L2, "L2"},
    {R2, "R2"},
    {A, "A"},
    {B, "B"},
    {X, "X"},
    {Y, "Y"},
    {START, "START"},
    {SELECT, "SELECT"},
    {L1, "L1"},
    {R1, "R1"},
    {L3, "L3"},
    {R3, "R3"}
};


// This is supposed to represent the actual controller in memory as it's created. 
// It can hold it's own assigned controller data and mappings
struct Controller
{
    Controller();
    Controller(const Controller& c);

    // Consider all of the below a default mapping
    std::map<Buttons, uint32_t>buttonCodes = 
    {
        {LEFT, ABS_X},
        {RIGHT, ABS_X},
        {UP, ABS_Y},
        {DOWN, ABS_Y},
        {RLEFT, ABS_RX},
        {RRIGHT, ABS_RX},
        {RUP, ABS_RY},
        {RDOWN, ABS_RY},
        {R2, ABS_RZ},
        {L2, ABS_Z},
        {DLEFT, ABS_HAT0X},
        {DRIGHT, ABS_HAT0X},
        {DUP, ABS_HAT0Y},
        {DDOWN, ABS_HAT0Y},
        {A, BTN_SOUTH},
        {B, BTN_EAST},
        {START, BTN_START},
        {SELECT, BTN_SELECT},
        {Y, BTN_NORTH},
        {X, BTN_WEST},
        {R1, BTN_TR},
        {L1, BTN_TL},
    };
    struct std::map<uint32_t, uinput_abs_setup*> abs
    {
        // LStick. X, Y
        {
            ABS_Y, 
            new uinput_abs_setup
            {
                ABS_Y,
                input_absinfo
                {
                    4095,
                    0,
                    65535,
                    255,
                    4095
                }
            }
        },
        {
            ABS_X,
            new uinput_abs_setup
            {
                ABS_X,
                input_absinfo
                {
                    4095,
                    0,
                    65535,
                    255,
                    4095
                }
            }
        },
        // RStick X, Y
        {
            ABS_RY, 
        new uinput_abs_setup
        {
            ABS_RY,
                input_absinfo
                {
                    4095,
                    0,
                    65535,
                    255,
                    4095
                }
            }
        },
        {
            ABS_RX, 
            new uinput_abs_setup
            {
                ABS_RX,
                input_absinfo
                {
                    4095,
                    0,
                    65535,
                    255,
                    4095
                }
            }
        },
        // RZ, Z
        {
            ABS_Z, 
            new uinput_abs_setup
            {
                ABS_Z,
                input_absinfo
                {
                    63,
                    0,
                    1023,
                    255,
                    63
                },
            }
        },
        {
            ABS_RZ,
            new uinput_abs_setup
            {
                ABS_RZ,
                input_absinfo
                {
                    63,
                    0,
                    1023,
                    255,
                    63
                },
            }
        }
    };

    // All of this data is intended to create as convincing a device as possible
    std::string uniqueID;
    std::string controllerName;

    fs::path eventPath = "/dev/input";

    int fd;
    int driverVersion;

    input_event ev;
    libevdev *dev;

    input_event pollEvent();
    std::map<Buttons, input_event> mappedControls;

    //friend void to_json(nlohmann::json& j, const Controller& p);
    //friend void from_json(const nlohmann::json& j, Controller& p);

};

class Emit
{
    private:
    int fd = 0;
    int maxInput = 0;

    libevdev_uinput* uidev;
    input_absinfo* init;

    Message* queue;

    Controller controller;
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

    std::vector<fs::path> controlSelect;
    public:

    Emit() = default;
    Emit(json j);

    void initalConfig();
    Controller selectController();
    void listControllers();

    Buttons GetCommands(std::string key);

    json control;
    void save(json &j, bool isDefault = false);
    //friend void to_json(nlohmann::json& j, const Emit& p);
    //friend void from_json(const nlohmann::json& j, Emit& p);

    bool CreateController(Message* queue, bool manualControl);
    bool emit(Buttons cmd);
    int moveABS(uint32_t ABS, int moveAxis, int flat);
    int resetABS(uint32_t ABS, int flatAxis);
    int pressBtn(uint32_t btn);
    int releaseBtn(uint32_t btn);
    bool Close();
};
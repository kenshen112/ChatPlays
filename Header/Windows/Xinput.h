#pragma once
#define WIN32_LEAN_AND_MEAN

#include <map>
#include <mutex>
#include <thread>
#include <cstdint>
#include <Xinput.h>
#include <iostream>
#include <windows.h>
#include <filesystem>

#include "json.hpp"
#include "Controller.h"
#include "ViGEm/Client.h"

#pragma comment(lib, "setupapi.lib")

using json = nlohmann::json;

namespace fs = std::filesystem;

static std::map<Buttons, uint16_t> buttonPos =
{
    {A, XUSB_GAMEPAD_A},
    {B, XUSB_GAMEPAD_B},
    {X, XUSB_GAMEPAD_X},
    {Y, XUSB_GAMEPAD_Y},
    {START, XUSB_GAMEPAD_START},
    {SELECT, XUSB_GAMEPAD_BACK},
    {L1, XUSB_GAMEPAD_LEFT_SHOULDER},
    {R1, XUSB_GAMEPAD_RIGHT_SHOULDER},
    {L3, XUSB_GAMEPAD_LEFT_THUMB},
    {R3, XUSB_GAMEPAD_RIGHT_THUMB}
};

struct axisData
{
private:
    short axis[6];
public:
    void set(short x, short y, short rx, short ry, short r2, short l2)
    {
        axis[0] = x;
        axis[1] = y;
        axis[2] = rx;
        axis[3] = ry;
        axis[4] = r2;
        axis[5] = l2;
    }

    short get(int i)
    {
        return axis[i];
    }
};

// This needs to be a thread safe blocking class!
class Emit : Controller
{
private:
    // I need Xinput
    PXUSB_REPORT report;
    PVIGEM_CLIENT driver;
    PVIGEM_TARGET xbox;

public:
    Emit() = default;
    Emit(json j);

    // All the action is in here m8
    bool isActive = false;

    // This says hey! I have data bitch! Stop!
    std::mutex m;

    bool AttachController() override;

    bool SendInput(Buttons cmd, ABS a, float axis = 0, int heldFor = 0) override;

    int MoveABS(ABS abs, float axis) override;
    int ResetABS(ABS abs) override;

    int PressButton(Buttons b) override;
    int ReleaseButton(Buttons b) override;

    void DisconnectController() override;
};